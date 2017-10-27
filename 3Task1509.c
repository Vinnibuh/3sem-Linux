#include <stdio.h>
#include <sys/stat.h>
#include <unistd.h>
#include <sys/types.h>
#include <fcntl.h>
#include <time.h>
#include <string.h>

int main(int argc, char **argv)
{
	char buffer[1000];
	int fd_read=open("FirstTry.txt",O_RDONLY,0700);
	if (fd_read==-1)
	{
		perror("Open error\n");
		return -1;
	}
	struct stat structure;
	if (fstat(fd_read,&structure)==-1)
	{
		perror("stat error");
		return(-1);
	}
	printf("Size of file: %ld bytes \n",structure.st_size);
	printf("Protection: %03o\n",structure.st_mode & -S_IFMT);
	//printf("Owner ID: %u \n",structure.st_uid);
	//printf("Time of last modification: %s\n", asctime(localtime(&structure.st_mtime)));
	if (S_ISSOCK(structure.st_mode)) {
		printf("Incorrect type of file: Socket\n");
		return(-1);
	}		
	if (S_ISLNK(structure.st_mode)) {
		printf("Incorrect type of file: Symbolic link\n");
		return(-1);
	}
	if (S_ISBLK(structure.st_mode)) {
		printf("Incorrect type of file: Block device\n");
		return(-1);
	}
	if (S_ISDIR(structure.st_mode)) {
		printf("Incorrect type of file: Directory\n");
		return(-1);
	}
	if (S_ISCHR(structure.st_mode)) {
		printf("Incorrect type of file: Character device\n");
		return(-1);
	}
	if (S_ISFIFO(structure.st_mode)) {
		printf("Incorrect type of file: FIFO\n");
		return(-1);
	}
	if (S_ISREG(structure.st_mode))
	{
		printf("Regular, copying allowed\n");
		int fd_wr=open("BackUp.txt",O_WRONLY | O_CREAT | O_TRUNC,0700);
		if (fd_wr==-1)
		{
			perror("Error at opening backup files\n");
			return(-1);
		}
		int x=0;
		size_t done;
		while (x != 1) {
			ssize_t read_size=read(fd_read, &buffer,sizeof(buffer));
			if (read_size == -1)
			{
				perror("Read error\n");
					return(-1);
			}
			if (read_size == 0)
			{
				x=1;
			}
			while (done < read_size) {
				ssize_t write_size=write(fd_wr, &buffer+done, read_size-done);
				if (write_size == -1)
				{
					perror("Write error\n");
					return(-1);
				}
				done += write_size;
			}
		}
		close(fd_wr);
		close(fd_read);
	}
	return 0;
}

