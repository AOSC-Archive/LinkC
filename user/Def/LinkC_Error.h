#ifndef LINKC_ERROR_H
#define LINKC_ERROR_H

void LinkC_Debug(const char *Target,int Status);

#define _DEBUG_MOD_     1

#define LINKC_DEBUG     5
#define LINKC_EXITED    4
#define LINKC_STARTED   3
#define LINKC_DONE      2
#define LINKC_WARNING   1
#define LINKC_SUCCESS   0
#define LINKC_FAILURE   -1

typedef unsigned int ip_t;
typedef unsigned int port_t;

#define STATUS_OK           3
#define STATUS_ONLINE       2
#define STATUS_HIDING       1
#define STATUS_OFFLINE      0
#define STATUS_ERROR        -1

#define FLAG_NODE           1
#define FLAG_ITEM           2

#endif
