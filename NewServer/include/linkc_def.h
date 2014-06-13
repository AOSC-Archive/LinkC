#if     LINKC_DEF_H < 2
#define LINKC_DEF_H 2

#include <stdio.h>
#include <arpa/inet.h>

/*  ============ 系统 ============  */
/*  定义LinkC协议的版本号           */
#define PROTOCOL_VERSION        2

/*  片轮发送链表的频率 次/秒        */
#define RE_VISIT_TIME           1

/*  定义单个包[已经发送的]在多少次  */
/*  片轮后被认定为对方没有收到      */
#define MAX_TIME_TO_LIVE        2

/*  定义最大重发次数                */
/*  超过最大重发次数则认定断线*/
#define MAX_RESEND_TIME         3

/*  定义标准缓冲区大小              */
#define STD_BUFFER_SIZE         2048

/*  定义最大缓冲区大小              */
#define MAX_BUFFER_SIZE         10240

/*  定义标准数据包大小              */
#define STD_PACKAGE_SIZE        524
/*  ============ 系统 ============  */



/*  ========= 错误代码 ===========  */
#define DIFF_VERSION            1   //  协议版本不一致
#define EXCEED_THE_LIMIT        2   //  大小超出限制
#define NOT_MESSAGE             3   //  非消息
#define MESSAGE_INCOMPLETE      4   //  数据不完整
/*  ========= 错误代码 ===========  */



/*  ========= 消息类型 ===========  */
#define USER_LOGIN              0   //  登陆请求
#define RQUSET_DATA             1	//  请求数据
#define UPLOAD_DATA             2   //  上传数据
#define RETURN_DATA             3   //  返回的数据
#define ACTION_STATUS           4   //  操作状态
/*  ========= 消息类型 ===========  */



/*  ========= 服务数据 ===========  */
#define USER_DATA               1   //  用户数据
/*  ========= 服务数据 ===========  */


/*  ========= 数据类型 ===========  */
struct MessageHeader_t{             //  消息头
    uint8_t     MessageType;        //  见 [消息类型]
    uint8_t     ServiceType;        //  见 [服务数据]
    uint16_t    UnuserdArg;         //  未使用的参数
};

struct LoginData_t{                 //  登陆数据
        char user_name[12];
        char pass_word[20];
};

struct UserData_t{                  //  用户数据
    uint32_t    UID;
    uint8_t     Status;             // 现在状态
    char        NickName[21];       // 昵称
    char        UserName[21];       // [登陆]用户名

    uint8_t     IsFriend;           //  是否为好友
    uint16_t    GroupNumber;        //  属于哪一组的
    char        PrivateNickName[21];//  你给对方取的昵称

    char        Telephone[21];      // 电话
    char        Company[21];        // 公司
    char        Address[80];        // 地址
    char        JoinTime[25];       // 创建时间
    uint32_t    IP;                 // 现在IP地址
};

struct FriendData_t{
    char        NickName[21];
    struct UserData_t Info;
};

struct PthreadData_t{
    struct sockaddr_in  Addr;
    int                 Sockfd;
};

typedef struct MessageHeader_t MessageHeader;
typedef struct LoginData_t     LoginData;
typedef struct UserData_t      UserData;
typedef struct PthreadData_t   PthreadData;

/*  ========= 数据类型 ===========  */

#endif // LINKC_DEF_H
