#include <stdio.h>
#include <linux/kernel.h>
#include <sys/syscall.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
int main() {
  int count = 0;
  int fd1 = open("README.md", O_RDONLY);
  int fd2 = open("alaki.txt", O_RDONLY);
  printf("fd1 = %d\nfd2 = %d\n", fd1, fd2);
  close(fd1);
  printf("main: %d\n", getpid());
  printf("1\n");
  int pid1 = fork();
  int pid2 = fork();
  int pid3 = fork();

  if (pid1 > 0 && pid2 > 0 && pid3 > 0) {
    printf("---\npid1 = %d\npid2= %d\n", pid1, pid2);
    while(count < 1000000000) count++;
    long int amma = syscall(545, getpid());
    printf("2\n");
    printf("System call sys_init returned %ld\n", amma);


    syscall(547);

    syscall(546);

    syscall(547);

    syscall(548);

    syscall(547);


  } else {
    while(1){}
  }
  return 0;
}
