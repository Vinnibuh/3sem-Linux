#define _DEFAULT_SOURCE
#include <stdio.h>
#include <sys/stat.h>
#include <unistd.h>
#include <sys/types.h>
#include <fcntl.h>
#include <string.h>
#include <dirent.h>

int CopyFile(char* FileName, char* DestPath) {
	char buffer[1000] ={0};
	int fd_read = open(FileName, O_RDONLY, 0700);
	if (fd_read==-1)
	{
		perror("Open error\n");
		return -1;
	}
	int fd_wr=open(DestPath, O_WRONLY | O_CREAT | O_TRUNC, 0700);
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
	int dir_fd = open("TestFolder", O_RDONLY | O_DIRECTORY, 0700);
	if (dir_fd == -1) {
		perror("Open error");
		return(-1);
	}
	DIR *dirpoint = fdopendir(dir_fd);
	struct dirent *new;
	while((new = readdir(dirpoint)) != 0) {
		printf("%ld - %s [%d]\n", new->d_ino, new->d_name, new->d_type);
		char path[100]={"TestFolder/"};
		char dest[100]={"BackUpFolder/"};
		strcat(path, new->d_name);
		strcat(dest, new->d_name);
		if (new->d_type == DT_REG) {
			int err_flag = CopyFile(path, dest);
			if (err_flag == -1) {
				printf("Error in CopyFile\n");
			}
		}   
	}  
	closedir(dirpoint);
	return (0);
}
