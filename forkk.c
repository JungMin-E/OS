#include <stdio.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <stdlib.h>
#include <unistd.h>

int main() {
	pid_t pid;

	printf("Hello World!\n");
	pid = fork();
	printf("Hello World!\n");
	//pid = fork(); 
	printf("Hello World!\n");

	return 0;

}
