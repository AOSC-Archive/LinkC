#include <stdio.h>
#include "re_cfg.h"
#include "../../server/include/def.h"


int main(void){
  int i;
  char * l,*x;
  int len = re_cfg("/tmp/test_hl.data",3,l);
  for(i = 0;i < len;i++){
   x=(char*) l[i]];
    printf("%s",x);
                          }
  return 0;
}
