#include <stdio.h>
#include <errno.h>
#include <unistd.h>
#include <stdio.h>
#include <fcntl.h>

int main()
{
	int fds[2], sum, retval;
	char buf[1000];
	size_t n = 1000;
	ssize_t size = 1;
	sum = 0;
	if (pipe(fds), 0) {
		perror("Pipe error\n");
	}
	retval = fcntl( fds[1], F_SETFL, fcntl(fds[0], F_GETFL) | O_NONBLOCK);
    if (retval == -1) {
		perror("fcntl");
		return(-1);
	}
    do {
		size = write(fds[1], &buf, n);
		if (size == -1) {
			n = n/2;
		} else {
			sum = sum + size;
		}
	} while (size != 0);
	printf("%d\n", sum);
	/*printf("%d\n", fcntl(fds[1], F_GETPIPE_SZ));*/		//since Linux 2.6.35 
	return 0;
}

