#include "process_sort.h"‪
struct process_struct* plist_head;

int cmp(void *priv, struct list_head *a, struct list_head *b)
{
  printk("Entered to predicate...\n");
  struct process_struct *ela, *elb;
  printk("before retriving objects");
  ela = container_of(a, struct process_struct, list);
  elb = container_of(b, struct process_struct, list);

  printk("ela->pid:%d, elb>pid:%d",ela->pid,elb->pid);
  return ela->pid - elb->pid;
}

void produce_file_list(struct process_struct* plist, struct task_struct* task) {
  struct file_list *file;
  struct files_struct *current_files;
  struct fdtable *files_table;
  int i = 0;

  file = (struct file_list *)kmalloc(sizeof(struct file_list), GFP_KERNEL);
  current_files = task->files;
  files_table = files_fdtable(current_files);
  printk("files_table->max_fds = %d\n", files_table->max_fds);
  while (i <= files_table->max_fds) {
    file->fd = i;
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
    // INIT_LIST_HEAD(&new_plist->file.list);
    // produce_file_list(new_plist, task);
    // list_for_each(m, &new_plist->file.list) {
    //   struct file_list *file = list_entry(m, struct file_list, list);
    //   printk("fd = %d\n", file->fd);
    // }
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
  /*list_for_each(m, &plist_head->file.list) {
    struct file_list *file = list_entry(m, struct file_list, list);
    printk("fd = %d\n", file->fd);
  }*/
  dfs(task);
  printk("16\n");

  return 0;
}
asmlinkage long sys_sort_process_list(void){
  list_sort(NULL, &plist_head->list, cmp);
  return 0;
}
asmlinkage long sys_print_process(void){
  struct list_head* p;
  printk("------------------------ The Holy Results ------------------------------------\n");
  list_for_each(p, &plist_head->list) {
    struct process_struct *pr = list_entry(p, struct process_struct, list);
    printk("pid = %d\n", pr->pid);
  }
  printk("---------------------------End of That ---------------------------------------\n");
  return 0;
}
