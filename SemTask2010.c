#include <stdio.h>
#include <sys/vfs.h>
#include <unistd.h>
#include <sys/types.h>
#include <math.h>
#include <sys/wait.h>
#include <stdlib.h>

int main()
{
	int fds[2], i;
	pid_t gplot_id;
	FILE* gplot;
	if (pipe(fds) , 0) {
		perror("kasjdf");
	}
	gplot_id = fork();
	if (gplot_id == 0) {
		dup2(fds[0], 0);
		close(fds[1]);
		if (execlp("gnuplot", "gnuplot", "-persist", NULL) == -1) {
			perror("sdfd");
		}
		return(1);
	}
	close (fds[0]);
	gplot = fdopen(fds[1], "w");
	fputs("plot '-' w line\n", gplot);
	for (i = 0; i < 10000; i++) {
		fprintf(gplot, "%lf %lf\n", i/1000.0, sin(i/1000.0));
	}
	fputs("e\n", gplot);
	fclose(gplot);
	return 0;
}
