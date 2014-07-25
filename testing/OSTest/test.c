#include <sys/types.h>
#include <sys/sem.h>
#include <sys/ipc.h>
#include <string.h> /* For strerror(3c) */
#include <errno.h> /* For errno */
#include <unistd.h> /* rand(3c) */
#include <stdio.h>
#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <signal.h>
#include <termios.h>
#include <fcntl.h>
#include <sys/wait.h>
#include <errno.h>
#include <sys/resource.h>
#include <stdint.h>
#include "../../NewServer/include/linkc_def.h"
#include "../../NewServer/include/linkc_package.h"

int main(){
    printf("Int's Size = %ld\nLong's Size = %ld\n",sizeof(int),sizeof(long));
    uint16_t k = 0x1234;
    uint8_t *p = (uint8_t*)&k;
    printf("Value = %lX\n",*p);
    p=p+1;
    printf("Value = %lX\n",*p);
    printf("Size Test------------------------\n");
    printf("PackageHeader's size = %ld\n",sizeof(PackageHeader));
    printf("MessageHeader's size = %ld\n",sizeof(MessageHeader));
    printf("UserData's size      = %ld\n",sizeof(UserData));
    return 0;
}
