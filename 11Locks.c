#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <stdlib.h>
#include <stdio.h>
#include <errno.h>
#include <stdint.h>

#define handle_error(msg) \
	do{ perror(msg); exit(EXIT_FAILURE);} while(0)

int main() {
	const size_t size = sizeof(long long int);
	struct flock fl;
	fl.l_type = F_RDLCK;
	fl.l_whence = SEEK_SET;
	fl.l_start = 0;
	fl.l_len = 0;
	long long int *buf;
	buf = (long long int *)malloc(size);
	int fd, flag;
	flag = 0;
	fd = open("counter.txt", O_RDWR, 0600);
	//Проверка, надо ли создавать файл, или он существует
	while (fd == -1) {
		perror("open\n");
		if (flag == 1) 
			exit(EXIT_FAILURE);
		flag = 1;
		fd = open("counter.txt", O_RDWR | O_CREAT, 0600);
	}
	ssize_t count;
	printf("flag %d, fd %d\n", flag, fd);
	//Запись начальной единицы, если файл был создан
	if (flag == 1) {
		*buf = 1;
		count= pwrite(fd, buf, size, 0);
		if ((size_t)count != size)
			handle_error("fatal error with 1 write\n");
		}
	//Цикл считывания и записи с lock
	do {
		if (fcntl(fd, F_SETLK, &fl) == -1) {
			if (errno == (EACCES||EAGAIN||EBADF)) {
				perror("RDLCK\n");
				continue;
			}
			else 
				handle_error("fatal error RDLCK\n");
		}
		count = pread(fd, buf, size, 0);
		if ((size_t)count != size) {
			perror("count != size\n");
			continue;
		}
		fl.l_type = F_WRLCK;
		(*buf)++;
		if (fcntl(fd, F_SETLK, &fl) == -1) {
			perror("WRLCK\n");
			continue;
		}
		count = pwrite(fd, buf, size, 0);
		printf("%lld\n", *buf);
		if ((size_t)count != size) {
			if (count > 0)
				handle_error("fatal write error\n");
			else {
				perror("writesize error\n");
				continue;
			}
		}
		fl.l_type = F_UNLCK;
		if (fcntl(fd, F_SETLK, &fl) == -1)
			handle_error("UNLCK\n");
	} while (1);
	return(1);
}

	
