#include "process_sort.h"‪

struct process_struct* plist_head;

// asmlinkage long sys_init_process_list(pid_t p‬‬, struct process_struct*) {
//
// }

asmlinkage long sys_init_process_list(pid_t p) {
  struct list_head *pos;
  struct pid* pid = find_get_pid(p);
  struct task_struct* task = pid_task(pid, PIDTYPE_PID);

  if (task == NULL) {
    printk("not found!");
    task = init_task;
    return 1;
  }
  plist_head->pid = task->pid;
  printk("The pid(task->pid) is %d\n", task->pid);
  printk("The pid is %d\n", plist_head->pid);
  INIT_LIST_HEAD(plist_head->list);

  list_for_each(pos, &task->children) {
    struct process_struct* new_plist;
    struct task_struct *child_task = list_entry(pos, struct task_struct, sibling);
    new_plist->pid = child_task->pid;
    printk("child_task->pid: %d\n", child_task->pid);

    list_add(new_plist, plist_head);
    printk("plist_head->pid: %d\n", plist_head->pid);
    printk("plist_head->next->pid: %d\n", plist_head->next->pid);
    // __sys_init_process_list((long)task_pid_nr(child_task));
  }

  return 0;
}
