#include <stdio.h>
#include <linux/kernel.h>
#include <sys/syscall.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>

#define INIT 545
#define SORT_P 546
#define PRINT 547
#define CLEAR 548
#define SORT_F 549

int main() {
    int count = 0;

    printf("1\n");
    int pid1 = fork();
    int pid2 = fork();
    int pid3 = fork();
    printf("pid1 = %d\n", pid1);
    printf("pid2 = %d\n", pid2);
    printf("pid3 = %d\n", pid3);
    if (pid1 > 0 && pid2 == 0 && pid3 > 0) {
        int fd1 = open("README.md", O_RDONLY);
    }
    if (pid1 > 0 && pid2 == 0 && pid3 == 0) {
//        int fd1 = open("mallaki.txt", O_RDONLY);
        int fd3 = open("palaki.txt", O_RDONLY);
    }
    if (pid1 > 0 && pid2 > 0 && pid3 == 0) {
        int _fd1 = open("README.md", O_RDONLY);
        int _fd2 = open("alaki.txt", O_RDONLY);
        close(_fd1);
    }
    if (pid1 > 0 && pid2 > 0 && pid3 > 0) {
        while (count < 1000000000) count++;
        printf("main: %d\n", getpid());
        long int amma = syscall(INIT, getpid());
        printf("2\n");
        printf("System call sys_init returned %ld\n", amma);


        syscall(PRINT);

//    syscall(SORT_P);
//
//    syscall(PRINT);
//
//    syscall(SORT_F);
//
//    syscall(PRINT);
//
//    syscall(CLEAR);
//
//    syscall(PRINT);


    } else {
        while (1) {}
    }
    return 0;
}
