#include "PackageCtl.h"
#include "Package.h"
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <openssl/rsa.h>
#include <openssl/pem.h>
#include <openssl/err.h>
#include "../Protocol/LinkC_NetWorkProtocol.h"
#include "../Def/LinkC_Error.h"

int	PackMessage(void* Message, size_t Length, LinkC_Socket *Socket, void* Output){
    if(Output == NULL){                                                     //  如果传出指针为空[这是不允许的]
        printf("Package Message Error [Argument [Output[ is NULL]\n");      //  打印错误信息
        return 1;
    }
    if(Length == 0){                                                        //  如果指明传入数据为空
        ((MessageHeader*)Output)->MessageType       = NORMAL_MESSAGE;       //  设置传出数据包头为标准数据
        ((MessageHeader*)Output)->MessageLength     = 0;                    //  设置后面数据长度为0
        ((MessageHeader*)Output)->ProtocolVersion   = PROTOCOL_VERSION;     //  设置协议版本号
        //  当前包的计数次并不在这里设置，而是在send函数中进行设置
        return sizeof(MessageHeader);                                       //  返回长度
    }
    if(Message == NULL){            //  如果传入数据为空
        printf("Package Message Error [Argument [Message] is NULL]\n");     //  打印错误信息
        return 1;
    }
    ((MessageHeader*)Output)->MessageType       = NORMAL_MESSAGE;       //  设置传出数据包头为标准数据
    ((MessageHeader*)Output)->ProtocolVersion   = PROTOCOL_VERSION;     //  设置协议版本号
    if(Socket->IsSecurity == 1){
        int len = 0;
        len = EncryptPackage(Message,Length,(void *)(((char *)Output)+8),Socket->PublicKey);
        if(len < 0){
            LinkC_Debug("EncryptPackage",LINKC_FAILURE);
            return -1;
        }
        ((MessageHeader*)Output)->MessageLength = len;
        return len+8;
    }
    ((MessageHeader*)Output)->MessageLength     = Length;
    memcpy((char *)Output+8,Message,Length);
    return Length + 8;
}

int	EncryptPackage(void* Message, int Length, void* Output, RSA* PublicKey){
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
    return RSA_public_encrypt(Length,(unsigned char *)Message,(unsigned char*)Output,PublicKey,RSA_PKCS1_PADDING);
}

int	UnPackMessage(void* Message, LinkC_Socket *Socket, void* Output){
    int Length = ((MessageHeader*)Message)->MessageLength;                  //  保存长度
    if(Socket->IsSecurity == 1){                                            //  如果是安全套接字
        int len = 0;                                                        //  临时长度
        len = DecryptPackage((char *)Message+8,Length,Output,Socket->PrivateKey);   //  解密
        if(len < 0){                                                        //  如果失败
            LinkC_Debug("DecryptPackage",LINKC_FAILURE);
            return -1;
        }
        return len;                                                         //  返回长度
    }
    memcpy((char*)Message+8,Output,Length);
    return Length;
}

int DecryptPackage(void* Message, int Length ,void* Output, RSA* PrivateKey){
    if(Output == NULL){                                                     //  如果传出指针为空[这是不允许的]
        printf("Decrypt Message Error [Argument [Output] is NULL]\n");      //  打印错误信息
        return 1;
    }
    if(Length == 0){                                                        //  如果指明传入数据为空
        return 0;                                                           //  返回0
    }else if(Message == NULL){                                              //  如果传入数据为空
        printf("Decrypt Message Error [Argument [Message] is NULL]\n");     //  打印错误信息
        return 1;                                                           //  返回错误
    }
    return RSA_private_decrypt(Length,(unsigned char *)Message,(unsigned char*)Output,PrivateKey,RSA_PKCS1_PADDING);
}
