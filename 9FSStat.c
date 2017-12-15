#include <sys/statvfs.h>
#include <sys/types.h>
#include <stdlib.h>
#include <stdio.h>
#include <sys/vfs.h>
#include <math.h>

#define handle_error(msg) \
	do {perror(msg); exit(EXIT_FAILURE); } while(0)

int main() {
	struct statfs stat;
	if (statfs(".", &stat))
		handle_error("statfs\n");
	printf("total space: %luGB\n", (((stat.f_blocks * stat.f_bsize) >> 29) + 1) >> 1);
	printf("free space: %luGb\n",  (((stat.f_bfree * stat.f_bsize) >> 29) + 1) >> 1);
	printf("free inodes: %lu\n", stat.f_ffree);
	printf("maximum filename length: %lu\n", stat.f_namelen);
	return(0);
}
