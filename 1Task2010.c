#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <stdlib.h>
#include <fcntl.h>
#include <mqueue.h>
#include <sys/stat.h>
#include <string.h>

/*void Stats(void) {
	printf("Current Process ID = %d\n", getpid());
	printf("Current Process Parent ID = %d\n", getppid());
	printf("Current Process Session ID = %d\n", getsid(getpid()));
	printf("Current Process Group ID = %d\n", getpgid(getpid()));
}*/

int main(int argc, char **argv)
{
	struct mq_attr structure;
	char mq_name[] = "/testmq";
	mqd_t mqdesc = mq_open(mq_name, O_RDONLY | O_CREAT | O_NONBLOCK, 0700);
	if (mqdesc == -1) {
		perror("error in open");
		return(0);
	}
	int flag = mq_getattr(mqdesc, &structure);
	if (flag == -1) {
		perror("error in mq_getattr");
	} else {
		printf("Stats by mq_getattr:\n");
		printf("mq_flags = %ld\n", structure.mq_flags);
		printf("mq_maxmsg = %ld\n", structure.mq_maxmsg);
		printf("mq_msgsize = %ld\n", structure.mq_msgsize);
		printf("mq_curmsgs = %ld\n", structure.mq_curmsgs);
	}
	mq_close(mqdesc);
	mq_unlink(mq_name);
	return 0;
}

/*Stats();
	pid_t child_id = fork();
	switch (child_id) {
		case -1 :
			perror("failde in fork");
			return 1;
		case 0 :
			printf("Child process info:\n");
			Stats();
			int status = 0; 
			pid_t id = waitpid(getppid(), &status, 0);
			printf("%d\n", id);
			printf("Child process info:\n");
			Stats();
			break;
		default :
		printf("Parent process info:\n");
		Stats();
		sleep(3);
		exit(1);
	} */
