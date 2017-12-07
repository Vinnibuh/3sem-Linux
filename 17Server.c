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
           mqd_t mqdes = mq_open(argv[1], O_RDONLY | O_CREAT, 0620);
           if (mqdes == (mqd_t) -1) {
               perror("open error\n");
		   }
	printf("server_name: %s\n", argv[1]);
	struct mq_attr attr;
           ssize_t msg_size;
           char *buf;
           unsigned msg_prio;
           /* Determine max. msg size; allocate buffer to receive msg */

           if (mq_getattr(mqdes, &attr) == -1) {
               handle_error("mq_getattr\n");
		   }
           buf =(char *) malloc(attr.mq_msgsize);
           if (buf == NULL) {
			   handle_error("malloc\n");
		   }
		   do {
			   msg_size = mq_receive(mqdes, buf, attr.mq_msgsize, &msg_prio);
			   if (msg_size == -1) {
				   handle_error("mq_receive\n");
			   }
			   printf("%s\n", buf);
		   } while ((strcmp(buf, "kill_query") != 0) && (msg_prio != 2));
		   mq_close(mqdes);
		   mq_unlink(argv[1]);
           free(buf);
	return 0;
}

