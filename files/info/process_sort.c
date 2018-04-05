#include "process_sort.h"‪
struct process_struct* plist_head;

void produce_file_list(struct process_struct* plist, struct task_struct* task) {
  struct file_list *file;
  struct files_struct *current_files;
  struct fdtable *files_table;
  int i = 0;

  file = (struct file_list *)kmalloc(sizeof(struct file_list), GFP_KERNEL);
  current_files = task->files;
  files_table = files_fdtable(current_files);
  while (files_table->fd[i] != NULL) {
    file->fd = files_table->fd[i];
    list_add(&file->list, &plist->file.list);
    i++;
  }
}

void dfs(struct task_struct* task) {
  struct process_struct* new_plist;
  struct task_struct *child_task;
  struct list_head *pos, *m;
  printk("-------------\n");
  printk("task->pid = %d\n", task->pid);
  list_for_each(pos, &task->children) {
    new_plist = (struct process_struct*)kmalloc(sizeof(struct process_struct), GFP_KERNEL);
    child_task = list_entry(pos, struct task_struct, sibling);
    new_plist->pid = child_task->pid;
    printk("child_task->pid: %d\n", child_task->pid);
    produce_file_list(new_plist, task);
    list_for_each(m, &new_plist->file.list) {
      struct file_list *file = list_entry(m, struct file_list, list);
      printk("fd = %d", file->fd);
    }
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
  struct pid* pid;
  struct task_struct *task;
  struct list_head* m;
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
  INIT_LIST_HEAD(&plist_head->file.list);

  produce_file_list(plist_head, task);
  list_for_each(m, &plist_head->file.list) {
    struct file_list *file = list_entry(m, struct file_list, list);
    printk("fd = %d", file->fd);
  }
  dfs(task);
  printk("16\n");

  return 0;
}
