#include "ConfigFileCtl.h"
#include <string.h>
#include <unistd.h>
#include <stdlib.h>

size_t ReadLine(int Fd, void *Vptr, int MaxSize){
    char Now  = 0;                  //  当前读到的字节  [初始化为0]
    int  Byte = 0;                  //  保存读取到的数据长度
    int  Result;                    //  保存read返回值
    char *ptr= (char *)Vptr;        //  读取的字符串,指向传入的指针指向的内存段
    int i;                          //  循环变量
    memset(ptr,0,MaxSize);          //  清空传入的指针指向的内存段
    for(i = 1;i < MaxSize;i++){     //  循环[这里为什么初始化为1呢，因为为了不读取超过MaxSize-1字节的数据[最后一字节为为终止符保留]]
        Result = read(Fd,&Now,1);   //  读取一个字节的数据
        if(Result == 0){            //  如果读取的数据为0[文件结尾]
            return Byte;            //  返回已经读取到的数据长度
        }else if(Result < 0){       //  如果小于0[出错了]
            perror("Read");         //  打印错误信息
            return 1;               //  返回错误
        }
        if(Now == ' ')  continue;   //  如果为空格就重新读一次吧[加速循环，不执行后面语句，直接跳转到循环开头]
        if(Now == '#'){             //  如果为井号[后面的全部舍弃吧舍弃吧]
            while(1){               //  永久循环
                Result = read(Fd,&Now,1);   //  读取一个字节的数据
                if(Result == 0){            //  如果读取的数据为0[文件结尾]
                    return Byte;            //  返回已经读取到的数据长度
                }else if(Result < 0){       //  如果小于0[出错了]
                    perror("Read");         //  打印错误信息
                    return 1;               //  返回错误
                }
                if(Now == '\n'){            //  如果遇到了换行符
                    if(Byte == 0){          //  如果之前读取到的长度为0[也就是开头就注视掉这一行了]
                        break;              //  跳出循环
                    }else{                  //  否则[也就是之前有数据][那就不读取了吧，返回]
                        return Byte ;       //  返回读取到的数据
                    }
                }
            }
            continue;               //  加速循环吧
        }
        if(Now == '\n'){            //  如果是换行
            if(Byte == 0){          //  如果之前没读取到任何数据
                continue;           //  再读取一行吧
            }else{                  //  否则
                return Byte;        //  返回读取到的长度
            }
        }
        ptr[Byte] = Now;             //  保存数据
        Byte ++;                    //  读取到的数据量自增加1
    }
    return -1;                      //  返回错误
}

int ReadConfigFile(int fd,char *Key,char *Value)
{
    char *Buffer = (char *)malloc(512);     //  分配内存
    int Byte;
    while(1){
        Byte = ReadLine(fd,Buffer,512);
        if(Byte < 0){
            printf("Read Error\n");
            return -1;
        }else if(Byte == 0){
            printf("Reach the END OF FILE\n");
            return 0;
        }
        printf("I Read [%s]\n",Buffer);
    }
    free(Buffer);
    return 0;
}
