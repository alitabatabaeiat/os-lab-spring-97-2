#include "process_sort.h"‪

struct process_struct* process_list;

asmlinkage long sys_init_process_list(pid_t p‬‬) {
  struct pid* pid = find_get_pid(p);
  struct task_struct* task = pid_task(pid, PIDTYPE_PID);

  if (task == NULL) {
    task = init_task;
    return 1;
  }
  process_list->pid = task->pid;
  printk("The pid is %d\n", process_list->pid);
  process_list->list->next = NULL;
  process_list->list->prev = NULL;

  list_for_each(process_list->list, &(task->children)) {
    struct process_struct* new_process_list;
    struct task_struct *child_task = list_entry(process_list->list, struct task_struct, sibling);
    new_process_list->pid = child_task->pid;
    new_process_list->list->next = NULL;
    new_process_list->list->prev = NULL;
    process_list->list->next = new_process_list;
    process_list = new_process_list;
    sys_init_process_list((long)task_pid_nr(child_task));
  }

  return 0;
}
