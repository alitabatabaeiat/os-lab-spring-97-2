#include "process_sort.h"‪

struct process_struct* plist_head;

// asmlinkage long sys_init_process_list(pid_t p‬‬, struct process_struct*) {
//
// }

asmlinkage long sys_init_process_list(pid_t p) {
  printk("hello");
  printk("1");
  struct list_head *pos;
  printk("2");
  struct pid* pid = find_get_pid(p);
  printk("3");
  struct task_struct* task = pid_task(pid, PIDTYPE_PID);
  printk("4");
  plist_head = (struct process_struct*)kmalloc(sizeof(struct process_struct),GFP_KERNEL)
  printk("5");
  if (task == NULL) {
    printk("5-1");
    printk("not found!");
    printk("5-2");
    task = init_task;
    printk("5-3");
    return 1;
  }
  plist_head->pid = task->pid;
  printk("6");
  printk("The pid(task->pid) is %d\n", task->pid);
  printk("7");
  printk("The pid is %d\n", plist_head->pid);
  printk("8");
  INIT_LIST_HEAD(plist_head->list);
  printk("9");

  list_for_each(pos, &task->children) {
    printk("10");
    struct process_struct* new_plist = (struct process_struct*)kmalloc(sizeof(struct process_struct),GFP_KERNEL)
    printk("11");
    struct task_struct *child_task = list_entry(pos, struct task_struct, sibling);
    printk("12");
    new_plist->pid = child_task->pid;
    printk("13");
    printk("child_task->pid: %d\n", child_task->pid);
    printk("14");

    list_add(new_plist, plist_head);
    printk("15");
    printk("plist_head->pid: %d\n", plist_head->pid);
    printk("plist_head->next->pid: %d\n", plist_head->next->pid);
    // __sys_init_process_list((long)task_pid_nr(child_task));
    // kfree(new_plist)
  }
  printk("16");

  return 0;
}
