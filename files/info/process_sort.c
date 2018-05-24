#include "process_sort.h"‪

struct process_struct *plist_head;

int inc_pid_pred(void *priv, struct list_head *a, struct list_head *b) {
    struct process_struct *temp_a, *temp_b;
    temp_a = kmalloc(sizeof(*temp_a), GFP_KERNEL);
    temp_b = kmalloc(sizeof(*temp_b), GFP_KERNEL);
    temp_a = list_entry(a,
    struct process_struct, list);
    temp_b = list_entry(b,
    struct process_struct, list);
    if (temp_a->pid > temp_b->pid)
        return 1;
    else if (temp_a->pid < temp_b->pid)
        return -1;
    else
        return 0;
}

int dec_mxfd_pred(void *priv, struct list_head *a, struct list_head *b) {
    struct process_struct *temp_a, *temp_b;
    temp_a = kmalloc(sizeof(*temp_a), GFP_KERNEL);
    temp_b = kmalloc(sizeof(*temp_b), GFP_KERNEL);
    temp_a = list_entry(a,
    struct process_struct, list);
    temp_b = list_entry(b,
    struct process_struct, list);
    if (temp_a->max_fd > temp_b->max_fd)
        return -1;
    else if (temp_a->max_fd < temp_b->max_fd)
        return 1;
    else
        return 0;
}

void produce_file_list(struct process_struct *plist, struct task_struct *task) {
    struct file_list *file;
    struct files_struct *current_files;
    struct fdtable *files_table;
    int i = 0, count_files = 0;

    file = (struct file_list *) kmalloc(sizeof(struct file_list), GFP_KERNEL);
    current_files = task->files;
    files_table = files_fdtable(current_files);
    printk("files_table->max_fds = %d\n", files_table->max_fds);
    while (i < files_table->max_fds) {
        if (files_table->fd[i] != NULL) {
            file->fd = i;
            plist->max_fd = i;
            list_add(&file->list, &plist->file.list);
            printk("i = %d,", i);
            count_files++;
        }
        i++;
    }
    plist->len_of_file_list = count_files;
    printk("max_fd = %d\nlen_of_file_list = %d\n", plist->max_fd, plist->len_of_file_list);
}

void dfs(struct task_struct *task) {
    struct process_struct *new_plist;
    struct task_struct *child_task;
    struct list_head *pos, *m;
    printk("-------------\n");
    printk("task->pid = %d\n", task->pid);
    list_for_each(pos, &task->children)
    {
        new_plist = (struct process_struct *) kmalloc(sizeof(struct process_struct), GFP_KERNEL);
        child_task = list_entry(pos,
        struct task_struct, sibling);
        new_plist->pid = child_task->pid;
        printk("child_task->pid: %d\n", child_task->pid);
        INIT_LIST_HEAD(&new_plist->file.list);
        produce_file_list(new_plist, task);
        list_add(&new_plist->list, &plist_head->list);
        printk("plist_head->pid: %d\n", plist_head->pid);
        struct process_struct *x = list_entry(plist_head->list.next,
        struct process_struct, list);
        printk("x->pid: %d\n", x->pid);
        dfs(child_task);
    }
    printk("-------------\n");
}

asmlinkage long sys_init_process_list(pid_t p) {
    struct pid *pid;
    struct task_struct *task;
    struct list_head *m;
    int ret = 0;
    pid = find_get_pid(p);
    task = pid_task(pid, PIDTYPE_PID);
    plist_head = (struct process_struct *) kmalloc(sizeof(struct process_struct), GFP_KERNEL);
    if (p == -1 || task == NULL) {
        task = &init_task;
        ret = 1;
    }
    plist_head->pid = task->pid;
    printk("The pid(task->pid) is %d\n", task->pid);
    printk("The pid is %d\n", plist_head->pid);
    INIT_LIST_HEAD(&plist_head->list);
    INIT_LIST_HEAD(&plist_head->file.list);

    produce_file_list(plist_head, task);
    dfs(task);

    return ret;
}

asmlinkage long sys_sort_process_list(void) {
    int init = 0;
    if (plist_head == NULL) {
        sys_init_process_list(-1);
        init = 1;
    }
    list_sort(NULL, &plist_head->list, inc_pid_pred);
    return init;
}

asmlinkage long sys_print_process_list(void) {
    int init = 0;
    if (plist_head == NULL) {
        sys_init_process_list(-1);
        init = 1;
    }
    struct list_head *p;
    printk("(%d, %d)\n", plist_head->pid, plist_head->len_of_file_list);
    printk("--------------- The Holy Results ---------------\n");
    list_for_each(p, &plist_head->list)
    {
        struct process_struct *pr = list_entry(p,
        struct process_struct, list);
        printk("(%d,%d)\n", pr->pid, pr->len_of_file_list);
    }
    printk("--------------- End of That ---------------\n");
    return init;
}

asmlinkage long sys_clear_process_list(void) {
    struct process_struct *ptr, *next;
    printk(KERN_INFO
    "Removing process list...\n");
    list_for_each_entry_safe(ptr, next, &plist_head->list, list)
    {
        list_del(&ptr->list);
        kfree(ptr);
    }
    plist_head = NULL;
    printk("after cleanup.\n");
    return 0;
}

asmlinkage long sys_sort_file_descriptor_list(void) {
    if (plist_head == NULL)
        printk("plist is null");

    list_sort(NULL, &plist_head->list, dec_mxfd_pred);
    return 0;
}
