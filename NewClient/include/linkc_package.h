#ifndef LINKC_PACKAGE
#define LINKC_PACKAGE
#include <stdint.h>
#include "linkc_def.h"
#include <time.h>

struct PackageHeader_t{
    uint8_t     MessageType;    //  消息类型
    uint8_t     ProtocolVersion;//  协议版本
    uint16_t    MessageLength;  //  从MessageBody到Data总计长度
    uint32_t    MessageCounts;  //  当前包计数次
};                              //  8 Byte Total
                                //  这一段的信息将以明文传输，后面的则要加密[如果支持的话]

struct PackageBody_t{
    uint8_t     Total;          //  如果为连续几个包，则指明连续的总包数
    uint8_t     Current;        //  如果为连续几个包，则指明当前包的计数次
    uint16_t    ServiceType;    //  服务类型
    time_t      Time;           //  发送时间
};

struct ConfirmationMessage_t{
    uint32_t    isRecved;       //  是否收到
    uint32_t    Count;          //  包的编号[计数次]
};                              //  8byte total

typedef struct PackageHeader_t          PackageHeader;
typedef struct PackageBody_t            PackageBody;

typedef struct ConfirmationMessage_t    ConfirmationMessage;

//  ####    MessageHeader   ####    MessageType ####
#define DEBUG_MESSAGE           0x00        //  测试时用的数据
#define NORMAL_MESSAGE          0x01        //  一般的数据
#define HEART_BEATS             0x02        //  心跳包
#define RESEND_MESSAGE          0x03        //  重发的数据
#define SSL_KEY_MESSAGE         0x04        //  交换SSL证书
#define CONFIRMATION_MESSAGE    0x05        //  收到确认消息
#define CONNECTION_MESSAGE      0x06        //  连接包
//  ####    MessageHeader   ####    MessageType ####    结束    ####

//  ####    定义区域    ####    结束    ####




#endif

