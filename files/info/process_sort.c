#include "process_sort.h"‪
struct process_struct* plist_head;

asmlinkage long __sys_init_process_list(pid_t p‬‬, struct process_struct*) {

}

asmlinkage long sys_init_process_list(pid_t p) {
  struct list_head *pos;
  struct pid* pid;
  struct task_struct* task;
  printk("hello\n");
  printk("1\n");
  printk("2\n");
  pid = find_get_pid(p);
  printk("3\n");
  task = pid_task(pid, PIDTYPE_PID);
  printk("4\n");
  plist_head = (struct process_struct*)kmalloc(sizeof(struct process_struct), GFP_KERNEL);
  printk("5\n");
  if (task == NULL) {
    printk("5-1\n");
    printk("not found!\n");
    printk("5-2\n");
    task = &init_task;
    printk("5-3\n");
    return 1;
  }
  plist_head->pid = task->pid;
  printk("6\n");
  printk("The pid(task->pid) is %d\n", task->pid);
  printk("7\n");
  printk("The pid is %d\n", plist_head->pid);
  printk("8\n");
  INIT_LIST_HEAD(&plist_head->list);
  printk("9\n");

  list_for_each(pos, &task->children) {
    printk("10\n");
    struct process_struct* new_plist = (struct process_struct*)kmalloc(sizeof(struct process_struct), GFP_KERNEL);
    printk("11\n");
    struct task_struct *child_task = list_entry(pos, struct task_struct, sibling);
    printk("12\n");
    new_plist->pid = child_task->pid;
    printk("13\n");
    printk("child_task->pid: %d\n", child_task->pid);
    printk("14\n");

    list_add(&new_plist->list, &plist_head->list);
    printk("15\n");
    printk("plist_head->pid: %d\n", plist_head->pid);
    struct process_struct* x = list_entry(plist_head->list.next, struct process_struct, list);
    printk("x->pid: %d\n", x->pid);
    __sys_init_process_list((long)task_pid_nr(child_task));
    // kfree(new_plist)
  }
  printk("16\n");

  return 0;
}
