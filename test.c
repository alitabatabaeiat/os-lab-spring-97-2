#include <stdio.h>
#include <linux/kernel.h>
#include <sys/syscall.h>
#include <unistd.h>
int main() {
  printf("1\n");
  int pid1 = fork();
  int pid2 = fork();
  if (pid1 == 0 && pid2 > 0) {
    printf("---\npid1 = %d\npid2= %d\n", pid1, pid2);
  }
  if (pid1 > 0 && pid2 > 0) {
    printf("---\npid1 = %d\npid2= %d\n", pid1, pid2);
    long int amma = syscall(545, getpid());
    printf("2\n");
    printf("System call sys_init returned %ld\n", amma);
  }
  return 0;
}
