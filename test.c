#include <stdio.h>
#include <linux/kernel.h>
#include <sys/syscall.h>
#include <unistd.h>
int main() {
  printf("1\n");
  long int amma = syscall(545, getpid());
  printf("2\n");
  printf("System call sys_init returned %ld\n", amma);
  return 0;
}
