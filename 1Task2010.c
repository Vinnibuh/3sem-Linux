#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <stdlib.h>
#include <fcntl.h>
#include <mqueue.h>
#include <sys/stat.h>
#include <string.h>

int main()
{
	struct mq_attr structure;
	char mq_name[] = "/testmq";
	int flag;
	mqd_t mqdesc = mq_open(mq_name, O_RDONLY | O_CREAT | O_NONBLOCK, 0700);
	if (mqdesc == -1) {
		perror("error in open");
		return(0);
	}
	flag = mq_getattr(mqdesc, &structure);
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

