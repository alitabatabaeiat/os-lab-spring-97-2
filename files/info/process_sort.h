#ifndef _PROCESS_SORT_H
#define _PROCESS_SORT_H

#include <linux/kernel.h>
#include <linux/init.h>
#include <linux/sched.h>
#include <linux/syscalls.h>

struct process_struct {
  struct list_head* list;
  struct file_list* files;
  pid_t p‬‬id;
};

struct file_list {
  int fd;
  struct list_head* list;
};

‪asmlinkage long sys_init_process_list(pid_t p‬‬);

#endif
