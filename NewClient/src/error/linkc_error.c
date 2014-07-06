#include "../../include/linkc_error.h"
#include "../../include/linkc_client_nonui.h"
#include <stdio.h>
#include <curses.h>

void LinkC_Debug(WINDOW* Console,const char *Target,int Status){
    if(Console != NULL) wrefresh(Console);
#if _DEBUG_MOD_
    if(Status == LINKC_SUCCESS)
#ifdef linux
        if(Console == NULL)
            printf("[\033[32mSUCCESS\033[0m]\t%s\n",Target);
        else{
            wprintw(Console,"[SUCCESS]\t%s\n",Target);
            wrefresh(Console);
        }
#else
        printf("[SUCCESS]\t%s\n",Target);
#endif
    else if(Status == LINKC_EXITED)
#ifdef linux
        if(Console == NULL)
            printf("[\033[33mEXITED \033[0m]\t%s\n",Target);
        else{
            wprintw(Console,"[EXITED ]\t%s\n",Target);
            wrefresh(Console);
        }
#else
        printf("[EXITED ]\t%s\n",Target);
#endif
    else if(Status == LINKC_WARNING)
#ifdef linux
        if(Console == NULL)
            printf("[\033[34mWARNING\033[0m]\t%s\n",Target);
        else{
            wprintw(Console,"[WARNING]\t%s\n",Target);
            wrefresh(Console);
        }
#else
        printf("[WARNING]\t%s\n",Target);
#endif
    else if(Status == LINKC_DONE)
#ifdef linux
        if(Console == NULL)
            printf("[\033[32mDONE   \033[0m]\t%s\n",Target);
        else{
            wprintw(Console,"[DONE   ]\t%s\n",Target);
            wrefresh(Console);
        }
#else
        printf("[DONE   ]\t%s\n",Target);
#endif
    else if(Status == LINKC_STARTED)
#ifdef linux
        if(Console == NULL)
            printf("[\033[32mSTARTED\033[0m]\t%s\n",Target);
        else{
            wprintw(Console,"[STARTED]\t%s\n",Target);
            wrefresh(Console);
        }
#else
        printf("[STARTED]\t%s\n",Target);
#endif
    else if(Status == LINKC_DEBUG){
#ifdef linux
        if(Console == NULL)
            printf("[\033[34mDEBUG  \033[0m  ]\t%s\n",Target);
        else{
            wprintw(Console,"[DEBUG  ]\t%s\n",Target);
            wrefresh(Console);
        }
#else
        printf("[DEBUG  ]\t%s\n",Target);
#endif
    }
#endif

    if(Status == LINKC_FAILURE){
#ifdef linux
        if(Console == NULL){
            printf("[\033[31mFAILURE\033[0m]\t%s\n",Target);
        }else{
            wprintw(Console,"[FAILURE]\t%s\n",Target);
            wrefresh(Console);
        }
#else
            printf("[FAILURE]\t%s\n",Target);
#endif
    }
}
