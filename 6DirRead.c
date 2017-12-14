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

int DirOutput(char* dir_path) {
	int dir_fd = open(dir_path, O_RDONLY | O_DIRECTORY, 0400);
	if (dir_fd == -1)
		handle_error("open\n");
	DIR *dirpoint = fdopendir(dir_fd);
	if (dirpoint == NULL)
		handle_error("fdopendir\n");
	struct dirent *dir_ent;
	char *path;
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
				if (DirOutput(path) == -1)
					handle_error("DirOutput\n");
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
	if (argc == 2) {
		if (DirOutput(argv[1]) == -1)
			handle_error("main function\n");
	}
	return (0);
}
