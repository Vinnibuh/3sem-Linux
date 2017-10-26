#include <stdio.h>
#include <sys/vfs.h>
#include <unistd.h>
#include <sys/types.h>
#include <math.h>
#include <sys/wait.h>
#include <stdlib.h>

int main(int argc, char **argv)
{
	int fds[2];
	if (pipe(fds) , 0) {
		perror("kasjdf");
	}
	pid_t gplot_id=fork();
	if (gplot_id == 0) {
		dup2(fds[0], 0);
		close(fds[1]);
		if (execlp("gnuplot", "gnuplot", "-persist", NULL) == -1) {
			perror("sdfd");
			}
		return(1);
	}
	close (fds[0]);
	FILE* gplot = fdopen(fds[1], "w");
	fputs("plot '-' w line\n", gplot);
	int i;
	for (i=0; i< 10000; i++) {
		fprintf(gplot, "%lf %lf\n", i/1000.0, sin(i/1000.0));
	}
	fputs("e\n", gplot);
	fclose(gplot);
	return 0;
}

/*Stats();
	pid_t child_id = fork();
	switch (child_id) {
		case -1 :
			perror("failde in fork");
			return 1;
		case 0 :
			printf("Child process info:\n");
			Stats();
			int status = 0; 
			pid_t id = waitpid(getppid(), &status, 0);
			printf("%d\n", id);
			printf("Child process info:\n");
			Stats();
			break;
		default :
		printf("Parent process info:\n");
		Stats();
		sleep(3);
		exit(1);
	} */
