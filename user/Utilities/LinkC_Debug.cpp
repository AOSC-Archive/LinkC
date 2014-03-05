#include "LinkC_Utilities.h"
#include <stdio.h>

void LinkC_Debug(const char *DebugMessage){
#if _DEBUG_MOD_
    printf("Debug >> %s\n",DebugMessage);
#endif
}
