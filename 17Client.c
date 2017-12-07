#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <stdlib.h>
#include <fcntl.h>
#include <mqueue.h>
#include <sys/stat.h>
#include <string.h>

#define handle_error(msg) \
           do { perror(msg); exit(EXIT_FAILURE); } while (0)

int main(int argc, char *argv[])
{
	int i;
	for(i=0; i<argc; i++) {
		printf("%d '%s'\n", i, argv[i]);
	}
	
	mqd_t mqdes = mq_open(argv[1], O_WRONLY, 0220);
    if (mqdes == (mqd_t) -1) {
        handle_error("open error\n");
	}
	unsigned prio = (unsigned)*argv[3];
	if (mq_send(mqdes, argv[2], strlen(argv[2]), prio) == -1) {
		handle_error("mq_send");
	}
	mq_close(mqdes);
	return 0;
}

