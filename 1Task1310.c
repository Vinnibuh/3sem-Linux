#include <stdio.h>
#include <errno.h>
#include <unistd.h>
#include <fcntl.h>
#include <stdlib.h>
#include <string.h>

int main(int argc, char **argv)
{
	int fds[2];
	char buf[1000];
		size_t n=1000;
	if (pipe(fds), 0) {
		perror("Pipe error\n");
	}
	ssize_t size = 1;
	int sum=0;
	int retval = fcntl( fds[1], F_SETFL, fcntl(fds[0], F_GETFL) | O_NONBLOCK);
        printf("Ret from fcntl: %d\n", retval);
        do {
			printf("begin\n");
			size = write(fds[1], &buf, n);
			printf("size %ld\n", size);
			if (size == -1) {
			n=n/2;
			printf("n %ld\n", n);
			} else {
			sum =sum + size;
			printf("sum %d\n", sum);
			}
		} while (size != 0);
		printf("%d\n", sum);
		printf("%d\n", fcntl(fds[1], F_GETPIPE_SZ));
	return 0;
}

