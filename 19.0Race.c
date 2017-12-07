#include <stdio.h>
#include <pthread.h>
#include <stdlib.h>
#include <errno.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/stat.h>

#define handle_error_en(en, msg) \
	do { errno = en; perror(msg); exit(EXIT_FAILURE); } while (0)
               
#define handle_error(msg) \
	do {perror(msg); exit(EXIT_FAILURE); } while(0)

void*
start_thread()
{
	int *n, c, fd;
	ssize_t size;
	n = (int *)malloc(sizeof(int));
	*n = 0;
	for (c = 0; c != 100; c++) {
		fd = open("Example.txt", O_RDWR, 0600);
		if (fd == -1) {
			handle_error("open thread");
		}
		size = pread(fd, n, sizeof(int), 0);
		if ((size_t)size < sizeof(int)) {
			handle_error("pread thread");
		}
		(*n)++;
		size = pwrite(fd, n, sizeof(int), 0);
		if ((size_t) size < sizeof(int)) {
			handle_error("pwrite thread");
		}
		close(fd);
	}
	free(n);
	sleep(1);
	pthread_exit(NULL);
}

int main()
{
	int s, c, ans, fd;
	void *res;
	pthread_t id_arr[10];
	fd = open("Example.txt", O_RDWR | O_CREAT | O_TRUNC, 0600);
	if (fd == -1) {
		handle_error("open\n");
	}
	ans = 0;
	s = pwrite(fd, &ans, sizeof(int), 0);
	if (s == -1) {
	handle_error("write\n");
	}
	for (c = 0; c != 10; c++) {
		s = pthread_create(&id_arr[c], NULL, start_thread, NULL);
		if (s != 0)
		handle_error_en(s, "pthread_create\n");
	}
	for (c = 0; c!= 10; c++) {
		s = pthread_join(id_arr[c], &res);
		if (s != 0)
		handle_error_en(s, "pthread_join\n");
		free(res);
	}
	sleep(1);
	s = pread(fd, &ans, sizeof(int), 0);
	if (s == -1) {
		handle_error("read main\n");
	}
	printf("%d\n", ans);
	close(fd);
	sleep(1);
	return 0;
}
