#include <sys/types.h>
#include <sys/wait.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

int main() {
	
	pid_t pid;

	if(pid < 0) {
		fprintf(stderr, "Fork Failed");
		return 1;
	}

	else if (pid == 0) {
	execlp("/bin/ls", "ls", NULL);
	}

	else {
	wait(NULL);
	printf("Child Complete");
	printf("\n");
	}

	return 0;
}
