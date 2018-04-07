#ifndef _PROCESS_SORT_H
#define _PROCESS_SORT_H

#include <linux/kernel.h>
#include <linux/init.h>
#include <linux/sched.h>
#include <linux/syscalls.h>
#include <linux/list.h>
#include <linux/slab.h>
#include <linux/fdtable.h>
#include <linux/list_sort.h>

struct file_list {
  int fd;
  struct list_head list;
};

struct process_struct {
  struct list_head list;
  struct file_list file;
  pid_t pid;
  int len_of_file_list;
  int max_fd;
};

void produce_file_list(struct process_struct* plist, struct task_struct* task);
void dfs(struct task_struct* task);
asmlinkage long sys_init_process_list(pid_t p);
asmlinkage long sys_sort_process_list(void);
asmlinkage long sys_print_process_list(void);
asmlinkage long sys_clear_process_list(void);
#endif
