#include "../include/linkc_basic_network.h"
#include "../include/linkc_error.h"
#include "../include/linkc_db.h"
#include "../include/linkc_p2p_helper.h"
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/resource.h>
#include <syslog.h>
#include <fcntl.h>
#include <signal.h>
#include <sys/stat.h>

/*
 * 初始化为守护进程
 *
 * 《UNIX环境高级编程》中文第三版375页
 *
 * 略加修改
 */
static void daemonize(const char *name);

int main(){
    pid_t pid;
    pid = fork();
    if(pid != 0){
        daemonize("LinkC Server");
        WaitForConnect();
    }else{
        p2p_helper();
    }
	return 0;                       //  返回
}



#define ERROR(err)	do{\
	perror(err);\
	exit(EXIT_FAILURE);\
}while(0);
static void daemonize(const char *name){
	int i,fd0,fd1,fd2;
	pid_t pid;
	struct rlimit rl;
	struct sigaction sa;

	printf("Initializing %s...\n",name);

	/*
	 * Clear file create mask
	 */
	umask(0);

	/*
	 * Get the maximum number of file descriptor
	 */
	if(getrlimit(RLIMIT_NOFILE,&rl)<0){
		ERROR("Fail to get file limit");
	}

	/*
	 * Become a session leader to lose controlling TTY.
	 */
	if((pid=fork())<0){
		ERROR("Can't fork");
	}else if(pid!=0){
		exit(0);	/* parent */
	}
	setsid();

	/*
	 * Ensure future opens won't allocate controlling TTYs.
	 */
	sa.sa_handler=SIG_IGN;
	sigemptyset(&sa.sa_mask);
	sa.sa_flags=0;
	if(sigaction(SIGHUP,&sa,NULL)<0){
		ERROR("Can't ignore SIGHUP");
	}
	if((pid=fork())<0){
		ERROR("Can't fork");
	}else if(pid!=0){
		exit(0);
	}

	/*
	 * Change the current working directory to the root so
	 * we won't prevent file systems from being umounted
	 */

	printf("%s started\n",name);

	/*
	 * Close all open file descriptors
	 */
	if(rl.rlim_max==RLIM_INFINITY){
		rl.rlim_max=1024;
	}
	for(i=0;i<(int)rl.rlim_max;i++){
		close(i);
	}

	/*
	 * Attach file descriptors 0,1 and 2 to /dev/null
	 *
	 * 因为所有文件描述符都已经被关闭，所以fd0=0,fd1=1,fd2=2
	 */
	fd0=open("/tmp/linkc_server.log",O_RDWR|O_CREAT|O_TRUNC,0666);
	fd1=dup(0);
	fd2=dup(0);
	if(fd0!=0 || fd1!=1 || fd2!=2){
		exit(-1);
	}
    if(InitSqliteDb()==LINKC_FAILURE){
        LinkC_Debug("Initializing Database",LINKC_FAILURE);
        ERROR("Init Sqlite Db");
        exit(1);
    }
	/*if(chdir("/")<0){
		ERROR("Fail to change directory to /");
	}*/
}
#undef ERROR
