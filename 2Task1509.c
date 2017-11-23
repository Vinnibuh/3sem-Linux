#include <sys/stat.h>
#include <unistd.h>
#include <stdio.h>
#include <sys/types.h>
#include <fcntl.h>
#include <string.h>

int main()
{
	char example[] = "Something strange\n";
	int wr_flag, fd;
	fd = open("FirstTry.txt", O_WRONLY | O_TRUNC | O_CREAT, 0700);
	if (fd == -1) {
		perror("open");
		return (-1);
	}
	wr_flag = write(fd, example, strlen(example));
	if (wr_flag == -1) {
		perror("write");
		return(-1);
	}
	close(fd);
	return 0;
}

