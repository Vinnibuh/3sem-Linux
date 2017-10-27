#include <unistd.h>
#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <time.h>

int main(int argc, char **argv)
{
	struct stat structure;
	int fd=open("FirstTry.txt",O_RDWR | O_CREAT,0700);
	if (fstat(fd,&structure)==-1)
	{
		perror("stat");
		return(-1);
	}
	printf("Size of file: %ld bytes \n",structure.st_size);
	printf("Protection: %03o\n",structure.st_mode & -S_IFMT);
	if (S_ISSOCK(structure.st_mode))
		printf("Socket\n");
	if (S_ISLNK(structure.st_mode))
		printf("Symbolic link\n");
	if (S_ISREG(structure.st_mode))
		printf("Regular\n");
	if (S_ISBLK(structure.st_mode))
		printf("Block device\n");
	if (S_ISDIR(structure.st_mode))
		printf("Directory\n");
	if (S_ISCHR(structure.st_mode))
		printf("Character device\n");
	if (S_ISFIFO(structure.st_mode))
		printf("FIFO\n");
	printf("Owner ID: %u \n",structure.st_uid);
	printf("Time of last modification: %s\n", asctime(localtime(&structure.st_mtime)));
	close(fd);
	return 0;
}

