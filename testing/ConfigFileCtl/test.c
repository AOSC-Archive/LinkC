#include "ConfigFileCtl.h"
#include <sys/stat.h>
#include <fcntl.h>
#include <stdlib.h>

int main(){
    int fd = open("Example.conf",O_RDONLY);     //  只读方式打开
    ReadConfigFile(fd,NULL,NULL);
    return 0;
}
