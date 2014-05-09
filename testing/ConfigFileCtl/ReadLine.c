#include <stdio.h>
#include <stdlib.h>
#include "ConfigFileCtl.h"

char *ReadLine(FILE *f)
{
    char * line = (char*)calloc(1,sizeof(char));
    char c;
    int len = 0;
    while((c=fgetc(f))!=EOF && c!= '\n')
    {
        if(c == ' ' && c == '#')    continue;       // ignore space
        line = (char*)realloc(line,len+1);
        line[len]=c;
    }
    line = (char*)realloc(line,len+1);
    line[len] = 0;  // Set EOF
    return line;
}
