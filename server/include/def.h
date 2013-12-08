#ifndef _DEF_H_
#define _DEF_H_

#include <stdlib.h>
#include <stdio.h>
#include <sys/socket.h>
#include <netinet/in.h>

/* define type */
typedef int errorcode;
typedef int flag_t;
typedef uint32_t ip_t;
typedef uint16_t port_t;


/*
  The node of the connection pool
  Src is the Source connection's address
  Dest is the Dest connection's address
*/
struct conn_info_t
{
	struct sockaddr_in Src;
	struct sockaddr_in Dest;
};
typedef struct conn_info_t conn_info;

#define SUCCESS 0
#ifndef LINKC_STATUS
#define LINKC_STATUS
#define LINKC_SUCCESS 0
#define LINKC_FAILURE -1
#endif
#define OK 0
#define NOT_OK -1
#define ERROR_TRACE

#define FLAG_NODE	1
#define FLAG_ITEM	2

#define FAILED(x) ((x < SUCCESS) ? 1 : 0)

#define CHECK_FAILED(x,y) ({ if (FAILED(x)) return y; })

#define CHECK_NOT_EQUAL(x,y,z) ({if (x==y) return z;})

#define CHECK_NOT_NULL(x,z) ({if (x==NULL) return z;})

#define CHECK_NOT_NEG(x,y) ({if (x<0) return y;})

#define CHECK_GREATER_THAN(x,y,z) ({if (x<=y) return z;})



#ifndef ERROR_TRACE
#define ERROR_BIND -10
#else
#define ERROR_BIND ((fprintf(stderr,"ERROR Bind Port!\n")==1) ? -10 : -10)
#endif

#ifndef ERROR_TRACE
#define ERROR_LISTEN -11
#else
#define ERROR_LISTEN ((fprintf(stderr,"ERROR Listen!\n")==1) ? -11 : -11)
#endif

#ifndef ERROR_TRACE
#define ERROR_SOCKET -12
#else
#define ERROR_SOCKET ((perror ("Socket") ? -12 : -12)
#endif

#ifndef ERROR_TRACE
#define ERROR_NULL_ARG -13
#else
#define ERROR_NULL_ARG ((fprintf(stderr,"ERROR argument is NULL!\n")==1) ? -13 : -13)
#endif

#ifndef ERROR_TRACE
#define ERROR_NOT_FOUND -14
#else
#define ERROR_NOT_FOUND ((fprintf(stderr,"ERROR Item Not Found!\n")==1) ? -14 : -14)
#endif

#ifndef ERROR_TRACE
#define ERROR_MALLOC_ERROR -15
#else
#define ERROR_MALLOC_ERROR ((fprintf(stderr,"ERROR Malloc Failed!\n")==1) ? -15 : -15)
#endif

#ifndef ERROR_TEACE
#define ERROR_FLAG -16
#else
#define ERROR_FALG ((fprintf(stderr,"ERROR Flag Set Error!\n")==1) ? -16 : -16)
#endif

#ifndef ERROR_TEACE
#define ERROR_MUTEX_LOCK -17
#else
#define ERROR_MUTEX_LOCK ((fprintf(stderr,"ERROR Mutex Lock Failed!\n")==1) ? -17 : -17)
#endif

#ifndef ERROR_TEACE
#define ERROR_MUTEX_UNLOCK -18
#else
#define ERROR_MUTEX_UNLOCK ((fprintf(stderr,"ERROR Mutex Unlock Failed!\n")==1) ? -18 : -18)
#endif

#ifndef ERROR_TEACE
#define ERROR_MUTEX_DESTORY -19
#else
#define ERROR_MUTEX_DESTORY ((fprintf(stderr,"ERROR Mutex Destory Failed!\n")==1) ? -19 : -19)
#endif

#ifndef ERROR_TEACE
#define ERROR_LIST_DESTORY -20
#else
#define ERROR_LIST_DESTORY ((fprintf(stderr,"ERROR List Destory Failed!\n")==1) ? -20 : -20)
#endif

#ifndef ERROR_TEACE
#define ERROR_ITEM_ADD -21
#else
#define ERROR_ITEM_ADD ((fprintf(stderr,"ERROR Item Add Failed!\n")==1) ? -21 : -21)
#endif

#ifndef ERROR_TEACE
#define ERROR_LIST_INIT -22
#else
#define ERROR_LIST_INIT ((fprintf(stderr,"ERROR List Init Failed!\n")==1) ? -22 : -22)
#endif

#ifndef ERROR_TEACE
#define ERROR_DB_INIT -23
#else
#define ERROR_DB_INIT ((fprintf(stderr,"ERROR DataBase Init Failed!\n")==1) ? -23 : -23)
#endif

#ifndef ERROR_TEACE
#define ERROR_NETWORK_INIT -24
#else
#define ERROR_NETWORK_INIT ((fprintf(stderr,"ERROR NetWork Init Failed!\n")==1) ? -24 : -24)
#endif

#ifndef ERROR_TEACE
#define ERROR_LOGIN -25
#else
#define ERROR_LOGIN ((fprintf(stderr,"ERROR Login Failed!\n")==1) ? -25 : -25)
#endif

#ifndef ERROR_TEACE
#define ERROR_SET_STATE -26
#else
#define ERROR_SET_STATE ((fprintf(stderr,"ERROR Set State Failed!\n")==1) ? -26 : -26)
#endif

#ifndef ERROR_TEACE
#define ERROR_RECV_MSG -27
#else
#define ERROR_RECV_MSG ((fprintf(stderr,"ERROR Recv Message Failed!\n")==1) ? -27 : -27)
#endif

#ifndef ERROR_TEACE
#define ERROR_SEND_MSG -28
#else
#define ERROR_SEND_MSG ((fprintf(stderr,"ERROR Send Message Failed!\n")==1) ? -28 : -28)
#endif

#endif
