#include <pmm.h>
#include <list.h>
#include <string.h>
#include <buddy.h>

free_area_t free_area;

#define free_list (free_area.free_list)
#define nr_free (free_area.nr_free)

// Global block
static size_t buddy_physical_size;
static size_t buddy_virtual_size;
static size_t buddy_segment_size;
static size_t buddy_alloc_size;
static size_t *buddy_segment;
static struct Page *buddy_physical;
static struct Page *buddy_alloc;

#define MIN(a,b)                ((a)<(b)?(a):(b))

// Buddy operate
#define BUDDY_ROOT              (1)
#define BUDDY_LEFT(a)           ((a)<<1)
#define BUDDY_RIGHT(a)          (((a)<<1)+1)
#define BUDDY_PARENT(a)         ((a)>>1)
#define BUDDY_LENGTH(a)         (buddy_virtual_size/UINT32_ROUND_DOWN(a))
#define BUDDY_BEGIN(a)          (UINT32_REMAINDER(a)*BUDDY_LENGTH(a))
#define BUDDY_END(a)            ((UINT32_REMAINDER(a)+1)*BUDDY_LENGTH(a))
#define BUDDY_BLOCK(a,b)        (buddy_virtual_size/((b)-(a))+(a)/((b)-(a)))
#define BUDDY_EMPTY(a)          (buddy_segment[(a)] == BUDDY_LENGTH(a))

// Bitwise operate
#define UINT32_SHR_OR(a,n)      ((a)|((a)>>(n)))   
#define UINT32_MASK(a)          (UINT32_SHR_OR(UINT32_SHR_OR(UINT32_SHR_OR(UINT32_SHR_OR(UINT32_SHR_OR(a,1),2),4),8),16))    
#define UINT32_REMAINDER(a)     ((a)&(UINT32_MASK(a)>>1))
#define UINT32_ROUND_UP(a)      (UINT32_REMAINDER(a)?(((a)-UINT32_REMAINDER(a))<<1):(a))
#define UINT32_ROUND_DOWN(a)    (UINT32_REMAINDER(a)?((a)-UINT32_REMAINDER(a)):(a))

static void
buddy_init_size(size_t n) {
    assert(n > 1);
    buddy_physical_size = n;
    if (n < 512) {
        buddy_virtual_size = UINT32_ROUND_UP(n-1);
        buddy_segment_size = 1;
    } else {
        buddy_virtual_size = UINT32_ROUND_DOWN(n);
        buddy_segment_size = buddy_virtual_size*sizeof(size_t)*2/PGSIZE;
        if (n > buddy_virtual_size + (buddy_segment_size<<1)) {
            buddy_virtual_size <<= 1;
            buddy_segment_size <<= 1;
        }
    }
    buddy_alloc_size = MIN(buddy_virtual_size, buddy_physical_size-buddy_segment_size);
}

static void
buddy_init_segment(struct Page *base) {
    // Init address
    buddy_physical = base;
    buddy_segment = KADDR(page2pa(base));
    buddy_alloc = base + buddy_segment_size;
    memset(buddy_segment, 0, buddy_segment_size*PGSIZE);
    // Init segment
    nr_free += buddy_alloc_size;
    size_t block = BUDDY_ROOT;
    size_t alloc_size = buddy_alloc_size;
    size_t virtual_size = buddy_virtual_size;
    buddy_segment[block] = alloc_size;
    while (alloc_size > 0 && alloc_size < virtual_size) {
        virtual_size >>= 1;
        if (alloc_size > virtual_size) {
            // Add left to free list
            struct Page *page = &buddy_alloc[BUDDY_BEGIN(block)];
            page->property = virtual_size;
            list_add(&(free_list), &(page->page_link));
            buddy_segment[BUDDY_LEFT(block)] = virtual_size;
            // Switch ro right
            alloc_size -= virtual_size;
            buddy_segment[BUDDY_RIGHT(block)] = alloc_size;
            block = BUDDY_RIGHT(block);
        } else {
            // Switch to left
            buddy_segment[BUDDY_LEFT(block)] = alloc_size;
            buddy_segment[BUDDY_RIGHT(block)] = 0;
            block = BUDDY_LEFT(block);
        }
    }
    if (alloc_size > 0) {
        struct Page *page = &buddy_alloc[BUDDY_BEGIN(block)];
        page->property = alloc_size;
        list_add(&(free_list), &(page->page_link));
    }
}

static void
buddy_init(void) {
    list_init(&free_list);
    nr_free = 0;
}

static void
buddy_init_memmap(struct Page *base, size_t n) {
    assert(n > 0);
    // Init pages
    for (struct Page *p = base; p < base + n; p++) {
        assert(PageReserved(p));
        p->flags = p->property = 0;
    }
    // Init size
    buddy_init_size(n);
    // Init segment
    buddy_init_segment(base);
}

static struct Page *
buddy_alloc_pages(size_t n) {
    assert(n > 0);
    struct Page *page;
    size_t block = BUDDY_ROOT;
    size_t length = UINT32_ROUND_UP(n);
    // Find block
    while (length <= buddy_segment[block] && length < BUDDY_LENGTH(block)) {
        size_t left = BUDDY_LEFT(block);
        size_t right = BUDDY_RIGHT(block);
        if (BUDDY_EMPTY(block)) {                   // Split
            size_t begin = BUDDY_BEGIN(block);
            size_t end = BUDDY_END(block);
            size_t mid = (begin+end)>>1;
            list_del(&(buddy_alloc[begin].page_link));
            buddy_alloc[begin].property >>= 1;
            buddy_alloc[mid].property = buddy_alloc[begin].property;
            buddy_segment[left] = buddy_segment[block]>>1;
            buddy_segment[right] = buddy_segment[block]>>1;
            list_add(&free_list, &(buddy_alloc[begin].page_link));
            list_add(&free_list, &(buddy_alloc[mid].page_link));
            block = left;
        } else if (length & buddy_segment[left]) {  // Find in left (optimize)
            block = left;
        } else if (length & buddy_segment[right]) { // Find in right (optimize)
            block = right;
        } else if (length <= buddy_segment[left]) { // Find in left
            block = left;
        } else if (length <= buddy_segment[right]) {// Find in right
            block = right;
        } else {                                    // Shouldn't be here
            assert(0);
        }
    }
    // Allocate
    if (length > buddy_segment[block])
        return NULL;
    page = &(buddy_alloc[BUDDY_BEGIN(block)]);
    list_del(&(page->page_link));
    buddy_segment[block] = 0;
    nr_free -= length;
    // Update buddy segment
    while (block != BUDDY_ROOT) {
        block = BUDDY_PARENT(block);
        buddy_segment[block] = buddy_segment[BUDDY_LEFT(block)] | buddy_segment[BUDDY_RIGHT(block)];
    }
    return page;
}

static void
buddy_free_pages(struct Page *base, size_t n) {
    assert(n > 0);
    struct Page *p = base;
    size_t length = UINT32_ROUND_UP(n);
    // Find buddy id 
    size_t begin = (base-buddy_alloc);
    size_t end = begin + length;
    size_t block = BUDDY_BLOCK(begin, end);
    // Release block
    for (; p != base + n; p ++) {
        assert(!PageReserved(p));
        p->flags = 0;
        set_page_ref(p, 0);
    }
    base->property = length;
    list_add(&(free_list), &(base->page_link));
    nr_free += length;
    buddy_segment[block] = length;
    // Upadte & merge
    while (block != BUDDY_ROOT) {
        block = BUDDY_PARENT(block);
        size_t left = BUDDY_LEFT(block);
        size_t right = BUDDY_RIGHT(block);
        if (BUDDY_EMPTY(left) && BUDDY_EMPTY(right)) {  // Merge
            size_t lbegin = BUDDY_BEGIN(left);
            size_t rbegin = BUDDY_BEGIN(right);
            list_del(&(buddy_alloc[lbegin].page_link));
            list_del(&(buddy_alloc[rbegin].page_link));
            buddy_segment[block] = buddy_segment[left]<<1;
            buddy_alloc[lbegin].property = buddy_segment[left]<<1;
            list_add(&(free_list), &(buddy_alloc[lbegin].page_link));
        } else {                                        // Update
            buddy_segment[block] = buddy_segment[BUDDY_LEFT(block)] | buddy_segment[BUDDY_RIGHT(block)];
        }
    }
}

static size_t
buddy_nr_free_pages(void) {
    return nr_free;
}

static void
macro_check(void) {

    // Block operate check
    assert(BUDDY_ROOT == 1);
    assert(BUDDY_LEFT(3) == 6);
    assert(BUDDY_RIGHT(3) == 7);
    assert(BUDDY_PARENT(6) == 3);
    assert(BUDDY_PARENT(7) == 3);
    size_t buddy_virtual_size_store = buddy_virtual_size;
    size_t buddy_segment_root_store = buddy_segment[BUDDY_ROOT];
    buddy_virtual_size = 16;
    buddy_segment[BUDDY_ROOT] = 16;
    assert(BUDDY_LENGTH(6) == 4);
    assert(BUDDY_BEGIN(6) == 8);
    assert(BUDDY_END(6) == 12);
    assert(BUDDY_BLOCK(8, 12) == 6);
    assert(BUDDY_EMPTY(BUDDY_ROOT));
    buddy_virtual_size = buddy_virtual_size_store;
    buddy_segment[BUDDY_ROOT] = buddy_segment_root_store;

    // Bitwise operate check
    assert(UINT32_SHR_OR(0xCC, 2) == 0xFF);
    assert(UINT32_MASK(0x4000) == 0x7FFF);
    assert(UINT32_REMAINDER(0x4321) == 0x321);
    assert(UINT32_ROUND_UP(0x2321) == 0x4000);
    assert(UINT32_ROUND_UP(0x2000) == 0x2000);
    assert(UINT32_ROUND_DOWN(0x4321) == 0x4000);
    assert(UINT32_ROUND_DOWN(0x4000) == 0x4000);

}

static void
size_check(void) {

    size_t buddy_physical_size_store = buddy_physical_size;
    buddy_init_size(200);
    assert(buddy_virtual_size == 256);
    buddy_init_size(1024);
    assert(buddy_virtual_size == 1024);
    buddy_init_size(1026);
    assert(buddy_virtual_size == 1024);
    buddy_init_size(1028);    
    assert(buddy_virtual_size == 1024);
    buddy_init_size(1030);    
    assert(buddy_virtual_size == 2048);
    buddy_init_size(buddy_physical_size_store);   

}

static void
segment_check(void) {

    // Check buddy segment
    size_t total = 0, count = 0;
    for (size_t block = BUDDY_ROOT; block < (buddy_virtual_size<<1); block++)
        if (BUDDY_EMPTY(block))
            total += BUDDY_LENGTH(block);
        else if (block < buddy_virtual_size)
            assert(buddy_segment[block] == (buddy_segment[BUDDY_LEFT(block)] | buddy_segment[BUDDY_RIGHT(block)]));
    assert(total == nr_free_pages());

    // Check free list 
    total = 0, count = 0;
    list_entry_t *le = &free_list;
    while ((le = list_next(le)) != &free_list) {
        struct Page *p = le2page(le, page_link);
        count ++, total += p->property;
    }
    assert(total == nr_free_pages());

}

static void
alloc_check(void) {

    // Build buddy system for test
    size_t buddy_physical_size_store = buddy_physical_size;
    for (struct Page *p = buddy_physical; p < buddy_physical + 1026; p++)
        SetPageReserved(p);
    buddy_init();
    buddy_init_memmap(buddy_physical, 1026);

    // Check allocation
    struct Page *p0, *p1, *p2, *p3;
    p0 = p1 = p2 = NULL;
    assert((p0 = alloc_page()) != NULL);
    assert((p1 = alloc_page()) != NULL);
    assert((p2 = alloc_page()) != NULL);
    assert((p3 = alloc_page()) != NULL);

    assert(p0 + 1 == p1);
    assert(p1 + 1 == p2);
    assert(p2 + 1 == p3);
    assert(page_ref(p0) == 0 && page_ref(p1) == 0 && page_ref(p2) == 0 && page_ref(p3) == 0);

    assert(page2pa(p0) < npage * PGSIZE);
    assert(page2pa(p1) < npage * PGSIZE);
    assert(page2pa(p2) < npage * PGSIZE);
    assert(page2pa(p3) < npage * PGSIZE);

    list_entry_t *le = &free_list;
    while ((le = list_next(le)) != &free_list) {
        struct Page *p = le2page(le, page_link);
        assert(buddy_alloc_pages(p->property) != NULL);
    }

    assert(alloc_page() == NULL);

    // Check release
    free_page(p0);
    free_page(p1);
    free_page(p2);
    assert(nr_free == 3);

    assert((p1 = alloc_page()) != NULL);
    assert((p0 = alloc_pages(2)) != NULL);
    assert(p0 + 2 == p1);

    assert(alloc_page() == NULL);

    free_pages(p0, 2);
    free_page(p1);
    free_page(p3);

    struct Page *p;
    assert((p = alloc_pages(4)) == p0);
    assert(alloc_page() == NULL);

    assert(nr_free == 0);

    // Restore buddy system
    for (struct Page *p = buddy_physical; p < buddy_physical + buddy_physical_size_store; p++)
        SetPageReserved(p);
    buddy_init();
    buddy_init_memmap(buddy_physical, buddy_physical_size_store);

}

static void
default_check(void) {

    // Check buddy system
    macro_check();
    size_check();
    segment_check();
    alloc_check();
    
}

const struct pmm_manager buddy_pmm_manager = {
    .name = "buddy_pmm_manager",
    .init = buddy_init,
    .init_memmap = buddy_init_memmap,
    .alloc_pages = buddy_alloc_pages,
    .free_pages = buddy_free_pages,
    .nr_free_pages = buddy_nr_free_pages,
    .check = default_check,
};

