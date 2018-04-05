#ifndef _PROCESS_SORT_H
#define _PROCESS_SORT_H

#include <linux/kernel.h>
#include <linux/init.h>
#include <linux/sched.h>
#include <linux/syscalls.h>
#include <linux/list.h>
#include <linux/slab.h>

struct file_list {
  int fd;
  struct list_head list;
};

struct process_struct {
  struct list_head list;
  struct file_list files;
  pid_t pid;
};

asmlinkage long dfs(struct task_struct* task);
asmlinkage long sys_init_process_list(pid_t p);

#endif
