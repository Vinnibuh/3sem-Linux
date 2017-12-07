#include <stdlib.h>
#include <sys/types.h>
#include <unistd.h>
#include <stdio.h>
#include <pwd.h>

int main()
{
	pid_t id;
	uid_t uid;
	id = getpid();
	printf("Process ID: %d\n", id);
	id = getppid();
	printf("Parent Process ID: %d\n", id);
	id = getsid(0);
	printf("Session ID: %d\n", id);
	id = getpgrp();
	printf("Process Group ID: %d\n", id);
	uid = getuid();
	printf("User ID: %d\n", uid);
	struct passwd* pwd;
	pwd = getpwuid(uid);
	printf("User Name: %s\n", pwd->pw_name);
	uid = geteuid();
	printf("Effective User ID: %d\n", uid);
	pwd = getpwuid(uid);
	printf("Effective User Name: %s\n", pwd->pw_name);
	return(1);
}
