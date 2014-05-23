#ifndef LINKC_ERROR_H
#define LINKC_ERROR_H

void LinkC_Debug(const char *Target,int Status);

#define _DEBUG_MOD_     1

#define LINKC_EXITED    4
#define LINKC_STARTED   3
#define LINKC_DONE      2
#define LINKC_WARNING   1
#define LINKC_SUCCESS   0
#define LINKC_FAILURE   -1

#endif
