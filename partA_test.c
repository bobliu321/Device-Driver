// Test partA syscall (#335)

#define _GNU_SOURCE
#include <unistd.h>
#include <sys/syscall.h>
#include <stdio.h>

#define SYS_partA 335

int main(int argc, char **argv) 
{
	if(argc <= 1) {
		printf("Provde a string for partA system call. \n");
		return -1;
	}

	const char *arg = argv[1];
	long res = syscall(SYS_partA, arg);
	printf("System call returned %ld \n", res);
	return res;
}
