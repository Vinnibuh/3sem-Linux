#include <stdio.h>
#include <sys/vfs.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>

void Stats(void) 
{
	printf("Current Process ID = %d\n", getpid());
	printf("Current Process Parent ID = %d\n", getppid());
	printf("Current Process Session ID = %d\n", getsid(getpid()));
	printf("Current Process Group ID = %d\n", getpgid(getpid()));
}

int main()
{
	int inf;
	pid_t child_id;
	Stats();
	child_id = fork();
	switch (child_id) {
		case -1 :
			perror("failde in fork");
			return 1;
		case 0 :
			printf("Child process info:\n");
			Stats();
			sleep(3);
			break;
		default :
		printf("Parent process info:\n");
		Stats();
		wait(&inf);
		printf("Parent process info:\n");
		Stats();
	}
	return 0;
}

