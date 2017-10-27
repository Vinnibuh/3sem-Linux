#include <sys/stat.h>
#include <unistd.h>
#include <stdio.h>
#include <sys/types.h>
#include <fcntl.h>
#include <string.h>

int main(int argc, char **argv)
{
	char example[] ="Something strange\n";
	int fd =open("FirstTry.txt",O_WRONLY | O_TRUNC | O_CREAT,0700);
	if (fd==-1)
	{
		perror("open");
		return (-1);
	};
	int wr_flag = write(fd, example, strlen(example));
	if (wr_flag == -1)
	{
		perror("write");
		return(-1);
	}
	close(fd);
	return 0;
}

