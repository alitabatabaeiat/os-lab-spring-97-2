#include "process_sort.h"‪
struct process_struct* plist_head;

void dfs(struct task_struct* task) {
  struct process_struct* new_plist;
  struct task_struct *child_task;
  struct list_head *pos;
  printk("-------------\n");
  printk("task->pid = %d\n", task->pid);
  list_for_each(pos, &task->children) {
    new_plist = (struct process_struct*)kmalloc(sizeof(struct process_struct), GFP_KERNEL);
    child_task = list_entry(pos, struct task_struct, sibling);
    new_plist->pid = child_task->pid;
    printk("child_task->pid: %d\n", child_task->pid);
    list_add(&new_plist->list, &plist_head->list);
    printk("plist_head->pid: %d\n", plist_head->pid);
    struct process_struct* x = list_entry(plist_head->list.next, struct process_struct, list);
    printk("x->pid: %d\n", x->pid);
    dfs(child_task);
    // kfree(new_plist)
  }
  printk("-------------\n");
}

asmlinkage long sys_init_process_list(pid_t p) {
  struct list_head *pos;
  struct pid* pid;
  struct task_struct *task, *child_task;
  struct process_struct* new_plist;
  pid = find_get_pid(p);
  task = pid_task(pid, PIDTYPE_PID);
  plist_head = (struct process_struct*)kmalloc(sizeof(struct process_struct), GFP_KERNEL);
  if (task == NULL) {
    printk("5-1\n");
    printk("not found!\n");
    printk("5-2\n");
    task = &init_task;
    printk("5-3\n");
    return 1;
  }
  plist_head->pid = task->pid;
  printk("The pid(task->pid) is %d\n", task->pid);
  printk("The pid is %d\n", plist_head->pid);
  INIT_LIST_HEAD(&plist_head->list);

  dfs(task);
  printk("16\n");

  return 0;
}
