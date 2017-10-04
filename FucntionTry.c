#include <stdio.h>
#include <sys/stat.h>
#include <unistd.h>
#include <sys/types.h>
#include <fcntl.h>
#include <string.h>

int CopyFile(char* FileName, char* DestPath) {
	char buffer[1000] ={0};
	int fd_read = open(FileName, O_RDONLY,0700);
	if (fd_read==-1)
	{
		perror("Open error\n");
		return -1;
	}
	int fd_wr=open(DestPath,O_WRONLY | O_CREAT | O_TRUNC,0700);
	if (fd_wr==-1)
	{
		perror("Error at opening backup files\n");
		return(-1);
	}
	int x=0;
	while (x != 1) {
			ssize_t read_size=read(fd_read, &buffer,sizeof(buffer));
			if (read_size == -1)
			{
				perror("Read error\n");
				return(-1);
			}
			if (read_size != sizeof(buffer))
			{
				x=1;
			}
			ssize_t write_size=write(fd_wr, &buffer,read_size);
			if (write_size == -1)
			{
				perror("Write error\n");
				return(-1);
			}
			if (write_size != read_size)
			{
				perror("Writing with error\n");
				return(-1);
			}
	}
	close(fd_wr);
	close(fd_read);
	return(0);
}

int main(int argc, char **argv)
{
	int flag = CopyFile("FirstTry.txt", "BackUp.txt");
	printf("%d", flag);
	return 0;
}

