#define _DEFAULT_SOURCE
#include <stdio.h>
#include <sys/stat.h>
#include <unistd.h>
#include <sys/types.h>
#include <fcntl.h>
#include <string.h>
#include <dirent.h>
#include <stdlib.h>

#define handle_error(msg) \
	do {perror(msg); return(-1);} while(1)

int CopyFile(char* FileName, char* DestPath) {
	printf("I'm here, %s\n", FileName);
	char buffer[1000] = {0};
	int fd_read = open(FileName, O_RDONLY, 0700);
	if (fd_read == -1) {
		perror("Open error\n");
		return -1;
	}
	int fd_wr = open(DestPath, O_WRONLY | O_CREAT | O_TRUNC, 0007);
	if (fd_wr == -1) {
		perror("Error at opening backup files\n");
		return(-1);
	}
	int x = 0;
	size_t done = 0;
	while (x != 1) {
		ssize_t read_size = read(fd_read, &buffer, sizeof(buffer));
		if (read_size == -1)
		{
			perror("Read error\n");
			return(-1);
		}
		if (read_size != sizeof(buffer))
		{
			x = 1;
		}
		while (done < (size_t)read_size) {
			ssize_t write_size = write(fd_wr, &buffer + done, read_size - done);
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
	return(0);
}

int DirCpOutput(char* dir_path, char* dest_path_dir) {
	printf("%s\n", dir_path);
	int dir_fd = open(dir_path, O_RDONLY | O_DIRECTORY, 0400);
	if (dir_fd == -1)
		handle_error("open\n");
	DIR *dirpoint = fdopendir(dir_fd);
	if (dirpoint == NULL)
		handle_error("fdopendir\n");
	struct dirent *dir_ent;
	char *path, *dest_path, *mid_path;
	mid_path = (char *)malloc(1000*sizeof(char));
	path = (char *)malloc(1000*sizeof(char));
	while((dir_ent = readdir(dirpoint)) != 0) {
		strcpy(path, dir_path);
		strcat(path, "/");
		strcat(path, dir_ent->d_name);
		switch (dir_ent->d_type) {
			case DT_DIR :
				if ((0 == strcmp(dir_ent->d_name, "."))||(0 == strcmp(dir_ent->d_name, ".."))||(0 == strcmp(dir_ent->d_name, ".git")))
					continue;
				printf("%ld - %s [%d]\n", dir_ent->d_ino, path, dir_ent->d_type);
				dest_path = strchr(path, '/');
				strcpy(mid_path, dest_path_dir);
				if (mkdir(strcat(mid_path, dest_path), 0777) == -1)
					handle_error("mk_dir\n");
				if (DirCpOutput(path, dest_path_dir) == -1)
					handle_error("DirOutput\n");	
				break;
			case DT_REG :
				printf("%ld - %s [%d]\n", dir_ent->d_ino, path, dir_ent->d_type);
				dest_path = strchr(path, '/');
				strcpy(mid_path, dest_path_dir);
				if (CopyFile(path, strcat(mid_path, dest_path)) == -1)
					handle_error("Dir_Copy\n");
				break;
			default :
				printf("%ld - %s [%d]\n", dir_ent->d_ino, path, dir_ent->d_type);
				break;
		}
	}
	closedir(dirpoint);
	return(1);
}

int main(int argc, char *argv[])
{
	if (argc != 3)
		handle_error("Wrong number of arguments\n");
	if (mkdir(argv[2], 0777) == -1)
		handle_error("mkdir\n");
	if (DirCpOutput(argv[1], argv[2]) == -1)
		handle_error("DirCpOut\n");
	return(1);
}

