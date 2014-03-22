/*
 * Author		： Junfeng Zhang <564691478@qq.com>
 * Last-Change		： March 22, 2014
 */
#include "LinkC_Utilities.h"
#include <stdio.h>
#include <string.h>

void LinkC_Debug(const char *DebugMessage){
#if _DEBUG_MOD_
    printf("Debug >> %s\n",DebugMessage);
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
