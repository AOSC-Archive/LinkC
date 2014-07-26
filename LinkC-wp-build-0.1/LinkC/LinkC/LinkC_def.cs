using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace LinkC
{
    class LinkC_def
    {
        /*  ============ 系统 ============  */
/*  定义LinkC协议的版本号           */
 static int PROTOCOL_VERSION = 2;

/*  片轮发送链表的频率 次/秒        */
static int RE_VISIT_TIME = 1;

/*  定义单个包[已经发送的]在多少次  */
/*  片轮后被认定为对方没有收到      */
static int MAX_TIME_TO_LIVE = 2;

/*  定义最大重发次数                */
/*  超过最大重发次数则认定断线*/
static int MAX_RESEND_TIME = 3;

/*  定义标准缓冲区大小              */
static int STD_BUFFER_SIZE = 2048;

/*  定义最大缓冲区大小              */
static int MAX_BUFFER_SIZE = 10240;

/*  定义标准数据包大小              */
static int STD_PACKAGE_SIZE = 524;
/*  ============ 系统 ============  */



/*  ========= 错误代码 ===========  */
static int DIFF_VERSION = 1;   //  协议版本不一致
static int EXCEED_THE_LIMIT = 2;   //  大小超出限制
static int NOT_MESSAGE = 3;   //  非消息
static int MESSAGE_INCOMPLETE = 4;   //  数据不完整
/*  ========= 错误代码 ===========  */


/*  ========= 指示代码 ===========  */
static int      LOGIN_FAILURE = 0x1001;
static int      LOGIN_SUCCESS = 0x0001;
static int     LOGOUT_FAILURE = 0x1002;
static int     LOGOUT_SUCCESS = 0x0002;
static int SET_STATUS_FAILURE = 0x1003;
static int SET_STATUS_SUCCESS = 0x0003;
static int   GET_DATA_FAILURE = 0x1004;
static int   GET_DATA_SUCCESS = 0x0004;

static int     NO_DATA = 0x2000;
/*  ========= 指示代码 ===========  */

static int USER_LOGIN = 0x01;    //  登陆请求
static int USER_LOGOUT = 0x02;    //  登出请求
/*  ========= 操作类型 ===========  */
static int RQUEST_DATA = 0x10;	//  请求数据
static int UPLOAD_DATA = 0x20;    //  上传数据
static int RETURN_DATA = 0x30;    //  返回的数据
static int STATUS_ACTION = 0x40;    //  状态操作
/*  ========= 消息类型 ===========  */


/*  ========= 服务数据 ===========  */
static int SELF_DATA = 0x00;    //  自身数据
static int USER_DATA = 0x01;    //  用户数据
static int FRIENDS_DATA = 0x02;    //  全部好友数据
static int SELF_STATUS = 0x03;    //  自身状态
/*  ========= 服务数据 ===========  */



/*  ========= 用户状态 ===========  */
static int STATUS_ONLINE = 0x01;    //  在线
static int STATUS_OFFLINE = 0x02;    //  离线
static int STATUS_HIDDEN = 0x03;    //  隐身
/*  ========= 用户状态 ===========  */

    }
    /*  ========= 数据类型 ===========  */
    struct MessageHeader_t
    {             //  消息头
        Byte ActionType;         //  见 [操作类型]
        Byte ServiceType;        //  见 [服务数据]
        UInt16 StatusCode;         //  状态码
    };
    struct RequestUser_t{
    UInt32    UID;
    UInt32    Flag;
};

struct LoginData_t{                 //  登陆数据
        Char[] UserName = new char[16];
        Char[] PassWord = new char[16];
};

struct UserData_t{                  //  用户数据
    UInt32    UID;
    Byte     Status;             // 现在状态
    Char[]        NickName = new Char[21];       // 昵称
    Char[]        UserName = new Char[21];       // [登陆]用户名

    Byte     IsFriend;           //  是否为好友
    UInt16    GroupNumber;        //  属于哪一组的
    Char[]        PrivateNickName = new Char[21];//  你给对方取的昵称

    Char[]        Telephone = new Char[21];      // 电话
    Char[]        Company = new Char[21];        // 公司
    Char[]        Address = new Char[80];        // 地址
    Char[]        JoinTime = new Char[25];       // 创建时间
    UInt32    IP;                 // 现在IP地址
};



}
