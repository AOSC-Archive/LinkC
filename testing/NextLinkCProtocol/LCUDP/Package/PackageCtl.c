#include "PackageCtl.h"
#include "Package.h"
#include <stdio.h>
#include <string.h>

int	PackMessage(void* Message, size_t Length, void* Output){
    if(Output == NULL){                                                     //  如果传出指针为空[这是不允许的]
        printf("Package Message Error [Argument [Output[ is NULL]\n");      //  打印错误信息
        return 1;
    }
    if(Length == 0){                                                        //  如果指明传入数据为空
        ((MessageHeader*)Output)->MessageType       = NORMAL_MESSAGE;       //  设置传出数据包头为标准数据
        ((MessageHeader*)Output)->MessageLength     = 0;                    //  设置后面数据长度为0
        ((MessageHeader*)Output)->ProtocolVersion   = PROTOCOL_VERSION;     //  设置协议版本号
        //  当前包的计数次并不在这里设置，而是在send函数中进行设置
        //  因为这里是仅发送数据包头的消息，又因为数据包头不进行加密，所以这里就不加密
        return sizeof(MessageHeader);                                       //  返回长度
    }
    if(Message == NULL){            //  如果传入数据为空
        printf("Package Message Error [Argument [Message] is NULL]\n");     //  打印错误信息
        return 1;
    }
    return 0;
}

int	EncryptPackage(void* Message, size_t Length, void* Output){
    if(Output == NULL){                                                     //  如果传出指针为空[这是不允许的]
        printf("Emcrypt Message Error [Argument [Output] is NULL]\n");      //  打印错误信息
        return 1;
    }
    if(Length == 0){                                                        //  如果指明传入数据为空
        return 0;                                                           //  返回0
    }else if(Message == NULL){                                              //  如果传入数据为空
        printf("Emcrypt Message Error [Argument [Message] is NULL]\n");     //  打印错误信息
        return 1;                                                           //  返回错误
    }
    memcpy(Output,Message,Length);                                          //  暂时不加密
    return 0;                                                               //  返回函数
}
