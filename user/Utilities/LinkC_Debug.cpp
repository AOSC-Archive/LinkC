/*
 * Author		： Junfeng Zhang <564691478@qq.com>
 * Last-Change		： March 22, 2014
 */
#include "LinkC_Utilities.h"
#include "def.h"
#include <stdio.h>
#include <string.h>

void LinkC_Debug(const char *DebugMessage){
#if _DEBUG_MOD_
    printf("[\033[35mDEBUG\033[0m]\t%s\n",DebugMessage);
#endif
}

void LinkC_Debug(const char *Target,int Status){
#if _DEBUG_MOD_
    if(Status == LINKC_SUCCESS)
#ifdef linux
        printf("[\033[32mSUCCESS\033[0m]\t%s\n",Target);
#else
        printf("[SUCCESS]\t%s\n",Target);
#endif
    else if(Status == LINKC_EXITED)
#ifdef linux
        printf("[\033[33mEXITED \033[0m]\t%s\n",Target);
#else
        printf("[EXITED ]\t%s\n",Target);
#endif
    else if(Status == LINKC_WARNING)
#ifdef linux
        printf("[\033[34mWARNING\033[0m]\t%s\n",Target);
#else
        printf("[WARNING]\t%s\n",Target);
#endif
#endif

    if(Status == LINKC_FAILURE)
#ifdef linux
            printf("[\033[31mFAILURE\033[0m]\t%s\n",Target);
#else
            printf("[FAILURE]\t%s\n",Target);
#endif
}

/*
void LinkC_Debug(const char *Action,const char *Status){
    void *DebugMessage;
    int len = strlen(Action);
    int i;
    if(len < _DEFAULT_ACTION_LENGTH){
        DebugMessage = new char[_DEFAULT_ACTION_LENGTH + 4 + strlen(Status)];
        strcpy((char*)DebugMessage,Action);
        for(i=0;i<_DEFAULT_ACTION_LENGTH - len;i++){
            (char *)DebugMessage+(len+i) = ' ';
        }
        (char *)DebugMessage+(i+len) = '=';
        strcpy((char *)DebugMessage+(i+len+1),Status);
        (char *)DebugMessage+(i+len+1+strlen(Status)+1) = 0;
    }

}
*/
