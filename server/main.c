#include "linkc_network.h"
#include "p2p_helper.h"
#include <stdlib.h>
#include <stdio.h>
#include <sys/types.h>
#include <unistd.h>

int main(int argc, char* argv[])
{
	pid_t LCServer = 0;
	if ((LCServer = fork()) < -1)
	{
		perror("fork");
		exit (EXIT_FAILURE);
	}
	else if (LCServer == 0)
	{
		start_connect();
	}
	pid_t P2PServer = 0;
	if ((P2PServer = fork()) < -1)
	{
		perror("fork");
		exit (EXIT_FAILURE);
	}
	else if (P2PServer == 0)
	{
		p2p_helper();
	}
	else		getchar();
	printf("Quit!\n");
	exit (0);
}
