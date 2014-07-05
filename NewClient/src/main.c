#include "../include/linkc_def.h"
#include "../include/linkc_error.h"
#include "../include/linkc_client.h"
#include <stdio.h>
#include <string.h>

int main(int argc,char **argv){
    if(argc == 2){
        if((strcmp(argv[1],"-NonUi")) == 0)
            NonUiMode();
    }else{
        //  start ui version
    }
    return 0;
}
