#include "stdio.h"
#include "stdlib.h"

extern char * readline(FILE *f)
{
    char * line = (char*)calloc(1,sizeof(char));;
    char c;
    int len = 0;
    while((c=fgetc(f))!=EOF && c!= '\n')
        {   while(c==' ')c=fgetc(f);
            line = (char*)realloc(line,sizeof(char)*(len+2));
            line[len++]=c;
            line[len]=0;

        }
    return line;
}
