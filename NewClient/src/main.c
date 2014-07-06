#include "../include/linkc_def.h"
#include "../include/linkc_error.h"
#include "../include/linkc_client.h"
#include <stdio.h>
#include <curses.h>
#include <string.h>
#include <stdlib.h>
#include <locale.h>

int main(int argc,char **argv){
    if(argc == 2){
        setlocale(LC_ALL,"");
        if((strcmp(argv[1],"-NonUi")) == 0){
            NonUiMode();
            endwin();
            exit(0);
        }
    }else{
        //  start ui version
    }
    return 0;
}
