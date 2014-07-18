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


/*  ========= 指示代码 ===========  */
#define      LOGIN_FAILURE          0x1001
#define      LOGIN_SUCCESS          0x0001
#define     LOGOUT_FAILURE          0x1002
#define     LOGOUT_SUCCESS          0x0002
#define SET_STATUS_FAILURE          0x1003
#define SET_STATUS_SUCCESS          0x0003
#define   GET_DATA_FAILURE          0x1004
#define   GET_DATA_SUCCESS          0x0004

#define     NO_DATA                 0x2000
/*  ========= 指示代码 ===========  */

#define CONNECTION                  0x00    //  连接
#define USER_LOGIN                  0x01    //  登陆请求
#define USER_LOGOUT                 0x02    //  登出请求
/*  ========= 操作类型 ===========  */
#define RQUEST_DATA                 0x10	//  请求数据
#define UPLOAD_DATA                 0x20    //  上传数据
#define RETURN_DATA                 0x30    //  返回的数据
#define STATUS_ACTION               0x40    //  状态操作
/*  ========= 消息类型 ===========  */


/*  ========= 服务数据 ===========  */
#define SELF_DATA                   0x00    //  自身数据
#define USER_DATA                   0x01    //  用户数据
#define FRIENDS_DATA                0x02    //  全部好友数据
#define SELF_STATUS                 0x03    //  自身状态
#define NAT_READY                   0x04    //  NAT打洞完成
/*  ========= 服务数据 ===========  */



/*  ========= 用户状态 ===========  */
#define STATUS_ONLINE               0x01    //  在线
#define STATUS_OFFLINE              0x02    //  离线
#define STATUS_HIDDEN               0x03    //  隐身
/*  ========= 用户状态 ===========  */


/*  ========= 数据类型 ===========  */
struct MessageHeader_t{             //  消息头
    uint8_t     ActionType;         //  见 [操作类型]
    uint8_t     ServiceType;        //  见 [服务数据]
    uint16_t    StatusCode;         //  状态码
};

struct RequestUser_t{
    uint32_t    UID;
    uint32_t    Flag;
};

struct LoginData_t{                 //  登陆数据
        char UserName[16];
        char PassWord[16];
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

struct P2PInfo_t{
    struct sockaddr_in Dest;
    int is_server;
};

struct PthreadData_t{
    struct sockaddr_in  Addr;
    int                 Sockfd;
};

typedef struct MessageHeader_t  MessageHeader;
typedef struct RequestUser_t    RequestUser;
typedef struct LoginData_t      LoginData;
typedef struct UserData_t       UserData;
typedef struct P2PInfo_t        P2PInfo;
typedef struct PthreadData_t    PthreadData;

/*  ========= 数据类型 ===========  */

#endif // LINKC_DEF_H
