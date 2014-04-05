/*
 * Author		： Junfeng Zhang <564691478@qq.com>
 * Last-Change		： March 26, 2014
 */

#include "linkc_network.h"
#include "p2p_helper.h"
#include <stdlib.h>
#include <stdio.h>
#include <sys/types.h>
#include <unistd.h>

int main(int argc, char* argv[])
{
#if PER_USER_TEST == 1
	start_connect();
#else
	pid_t LCServer = 0;		// LinkC Server
	pid_t P2PServer = 0;		// P2P Helper
	if ((LCServer = fork()) < -1)
	{
		perror("fork");
		exit (EXIT_FAILURE);
	}
	else if (LCServer == 0)
	{
		start_connect();
	}
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
#endif
	printf("Quit!\n");
	exit (0);
}
