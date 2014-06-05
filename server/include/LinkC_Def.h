#ifndef LINKC_DEF_H
#define LINKC_DEF_H


/* 系统 */
#define PROTOCOL_VERSION        2
#define MAX_MESSAGE_POOL_SIZE   15      //  最大缓冲区保存数据报的数量
#define LINKC_MESSAGE_VERSION   2       //  协议版本
#define ERROR_OUTPUT_TYPE       1       //  表示直接输出[2]为输出到指定文件
#define RE_VISIT_TIME           1       // 片轮查询List的时间间隔[单位为秒]
#define MAX_TIME_TO_LIVE        2       // 一个Node[包]在TIME_TO_LIVE次访问后还没有被消除
                                        //      --> 这里是两秒后如果没有收到确认消息
                                        //      --> 则被认定为发送失败，重发数据
#define MAX_RESEND_TIME         3       //  最大重发次数
                                        //      --> 这里是指如果重发三次还是没有收到信息
                                        //      --> 则认定断开链接
#define STD_BUFFER_SIZE         2048    //  标准缓冲区大小
#define STD_PACKAGE_SIZE        524         //  标准包大小
#define MAX_BUFFER_SIZE         10240	// 最大缓冲区大小

/* Error_Code */
#define DIFF_VERSION            1       // 协议版本不一致
#define EXCEED_THE_LIMIT        2       // 大小超出限制
#define NOT_MESSAGE             3       // 非消息
#define MESSAGE_INCOMPLETE      4       // 数据不完整
#define OVER_RECV               5       // 收到的数据大于一个包，并且前面的是一个完整的包


#define ALL_FRIEND              0
#define HEART_BEATS_TIME        5       // 心跳时间[感觉有歧义]


/* 消息头[LinkC_Message_Header] */
#define EXIT                    1	// 退出
#define CONNECTION              2	// 连接
#define SYS_MESSAGE_PUSHING     3	// 系统消息推送
#define SYS_ACTION_STATUS       4	// 系统操作状态
#define SYS_FRIEND_DATA         5	// 好友资料
#define SYS_FRIENDS_DATA        6
#define SYS_USER_INFO           7	// 用户自己的资料
#define LOGIN                   8	// 登录
//-------分割--------//
#define USER_REQUEST            9	// 用户请求 [扩展中]
#define USER_MESSAGE            10	// 好友之间的信息[扩展中]
#define USER_CHAT_MESSAGE       11	//

/* 用户请求[LinkC_User_Request] */
#define USER_LOGOUT             1	// 登出
#define USER_STATUS_HIDDEN      2	// 状态-隐身
#define USER_STATUS_ONLINE      3	// 状态-在线
#define USER_FRIEND_DATA        4	// 请求好友数据
#define USER_FRIENDS_DATA       5
#define USER_CHAT_REQUEST       6	//
#define USER_INFO_REQUEST      	7   // 用户请求自身的资料

/* 用户消息[LinkC_User_Message]*/
#define USER_CHAT               1	// 聊天
#define USER_ACCEPT             2	// 接受
#define USER_REFUSE             3	// 拒绝
#define USER_ADD_FRIEND         4	// 添加好友
#define USER_DEL_FRIEND         5	// 删除好友
#define USER_CONNECT_READY      6	// P2P连接准备完毕

#endif // LINKC_DEF_H
