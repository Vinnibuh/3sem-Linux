#include <stdio.h>
#include <pthread.h>
#include <stdlib.h>
#include <errno.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <signal.h>
#include <stdatomic.h>

#define handle_error_en(en, msg) \
	do { errno = en; perror(msg); exit(EXIT_FAILURE); } while (0)
               
#define handle_error(msg) \
	do {perror(msg); exit(EXIT_FAILURE); } while(0)

atomic_long ans;

void*
start_thread()
{
	int c;
	for (c = 0; c != 10000000; c++) {
		atomic_fetch_add_explicit(&ans, 1, memory_order_relaxed);
	}
	sleep(1);
	pthread_exit(NULL);
}

int main()
{
	int s, c;
	void *res;
	pthread_t id_arr[10];
	for (c = 0; c != 10; c++) {
		s = pthread_create(&id_arr[c], NULL, start_thread, NULL);
		if (s != 0)
		handle_error_en(s, "pthread_create\n");
	}
	sleep(1);
	for (c = 0; c!= 10; c++) {
		s = pthread_join(id_arr[c], &res);
		if (s != 0)
		handle_error_en(s, "pthread_join\n");
		free(res);
	}
	sleep(1);
	printf("%ld\n", ans);
	sleep(1);
	return 0;
}
 
