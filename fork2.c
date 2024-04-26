#include <stdio.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <stdlib.h>
#include <unistd.h>

int main() {
	pid_t pid;

	for(int i = 0; i < 2; i++) {
	
	pid = fork(); //fork : System Call creates new Process

	if(pid < 0) {
		fprintf(stderr, "Fork Failed\n"); //Error Message: Fork Failed
		return 1;
	}
	
	else {
	wait(NULL); //wait: Parent Process Waiting until Child Process exit
	printf("Hello\n");
	}
}
	return 0;
}
