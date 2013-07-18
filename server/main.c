#include "include/linkc_network.h"
#include <stdlib.h>
#include <stdio.h>
#include <sys/types.h>
#include <unistd.h>

int main(int argc, char* argv[])
{
	pid_t pid = 0;
	if ((pid = fork()) < -1)
	{
		perror("fork");
		exit (EXIT_FAILURE);
	}
	if (pid == 0)
	{
		start_connect();
	}
	else		getchar();
	printf("Quit!\n");
	exit (0);
}
