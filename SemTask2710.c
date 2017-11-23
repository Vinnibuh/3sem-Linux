#include <stdio.h>
#include <stdint.h>
#include <pthread.h>
#include <errno.h>
#include <unistd.h>

void* thr_worker(void* arg) 
{
	uint64_t *p = arg;
	while (1) {
		(*p)++;
	}
	return NULL;
}

int main()
{
	uint64_t i = 0;
	pthread_t worker;
	int j;
	if ((errno = pthread_create(&worker, NULL, thr_worker, &i))) {
		perror("Error in open");
		return(-1);
	}
	for(j = 0; j < 10; j++) {
		sleep(1);
		printf("i = %llu\n", (unsigned long long)i);
	}
	return 0;
}

