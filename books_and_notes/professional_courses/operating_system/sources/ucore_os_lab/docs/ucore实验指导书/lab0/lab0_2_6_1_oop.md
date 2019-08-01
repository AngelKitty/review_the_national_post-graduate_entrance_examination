
#### 2.6.1 面向对象编程方法

uCore设计中采用了一定的面向对象编程方法。虽然C 语言对面向对象编程并没有原生支持，但没有原生支持并不等于我们不能用 C 语言写面向对象程序。需要注意，我们并不需要用 C语言模拟出一个常见 C++ 编译器已经实现的对象模型。如果是这样，还不如直接采用C++编程。

uCore的面向对象编程方法，目前主要是采用了类似C++的接口（interface）概念，即是让实现细节不同的某类内核子系统（比如物理内存分配器、调度器，文件系统等）有共同的操作方式，这样虽然内存子系统的实现千差万别，但它的访问接口是不变的。这样不同的内核子系统之间就可以灵活组合在一起，实现风格各异，功能不同的操作系统。接口在 C 语言中，表现为一组函数指针的集合。放在 C++ 中，即为虚表。接口设计的难点是如果找出各种内核子系统的共性访问/操作模式，从而可以根据访问模式提取出函数指针列表。

比如对于uCore内核中的物理内存管理子系统，首先通过分析内核中其他子系统可能对物理内存管理子系统，明确物理内存管理子系统的访问/操作模式，然后我们定义了pmm_manager数据结构（位于lab2/kern/mm/pmm.h）如下： 

	// pmm_manager is a physical memory management class. A special pmm manager - XXX_pmm_manager
	// only needs to implement the methods in pmm_manager class, then XXX_pmm_manager can be used
	// by ucore to manage the total physical memory space.
	struct pmm_manager {
		// XXX_pmm_manager's name
		const char *name;  
		// initialize internal description&management data structure
		// (free block list, number of free block) of XXX_pmm_manager 
		void (*init)(void); 
		// setup description&management data structcure according to
		// the initial free physical memory space 
		void (*init_memmap)(struct Page *base, size_t n); 
		// allocate >=n pages, depend on the allocation algorithm 
		struct Page *(*alloc_pages)(size_t n);  
		// free >=n pages with "base" addr of Page descriptor structures(memlayout.h)
		void (*free_pages)(struct Page *base, size_t n);   
		// return the number of free pages 
		size_t (*nr_free_pages)(void);                     
		// check the correctness of XXX_pmm_manager
		void (*check)(void);                               
	};
这样基于此数据结构，我们可以实现不同连续内存分配算法的物理内存管理子系统，而这些物理内存管理子系统需要编写算法，把算法实现在此结构中定义的init（初始化）、init_memmap（分析空闲物理内存并初始化管理）、alloc_pages（分配物理页）、free_pages（释放物理页）函数指针所对应的函数中。而其他内存子系统需要与物理内存管理子系统交互时，只需调用特定物理内存管理子系统所采用的pmm_manager数据结构变量中的函数指针即可
 