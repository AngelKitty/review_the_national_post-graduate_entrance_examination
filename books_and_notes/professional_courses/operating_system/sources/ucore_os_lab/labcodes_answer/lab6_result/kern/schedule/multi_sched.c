#include <defs.h>
#include <list.h>
#include <proc.h>
#include <assert.h>
#include <multi_sched.h>

static void
multi_init(struct run_queue *rq) {
    for (int i = 0; i < MULTI_QUEUE_NUM; i++)
        list_init(&(rq->multi_run_list[i]));
    rq->proc_num = 0;
}

static void
multi_enqueue(struct run_queue *rq, struct proc_struct *proc) {
    int level = proc->multi_level;
    if (proc->time_slice == 0 && level < (MULTI_QUEUE_NUM-1))
        level ++;
    proc->multi_level = level;
    list_add_before(&(rq->multi_run_list[level]), &(proc->run_link));
    if (proc->time_slice == 0 || proc->time_slice > (rq->max_time_slice << level))
        proc->time_slice = (rq->max_time_slice << level);
    proc->rq = rq;
    rq->proc_num ++;
}

static void
multi_dequeue(struct run_queue *rq, struct proc_struct *proc) {
    list_del_init(&(proc->run_link));
    rq->proc_num --;
}

static struct proc_struct *
multi_pick_next(struct run_queue *rq) {
    for (int i = 0; i < MULTI_QUEUE_NUM; i++)
        if (!list_empty(&(rq->multi_run_list[i])))
            return le2proc(list_next(&(rq->multi_run_list[i])), run_link);
    return NULL;
}

static void
multi_proc_tick(struct run_queue *rq, struct proc_struct *proc) {
    if (proc->time_slice > 0) {
        proc->time_slice --;
    }
    if (proc->time_slice == 0) {
        proc->need_resched = 1;
    }
}

struct sched_class multi_sched_class = {
     .name = "multi_level_feedback_queue_scheduler",
     .init = multi_init,
     .enqueue = multi_enqueue,
     .dequeue = multi_dequeue,
     .pick_next = multi_pick_next,
     .proc_tick = multi_proc_tick,
};
