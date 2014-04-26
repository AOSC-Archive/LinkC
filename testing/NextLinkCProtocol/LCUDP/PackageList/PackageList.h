#ifndef LINKC_PACKAGE_LIST
#define LINKC_PACKAGE_LIST

#include <time.h>
#include <pthread.h>
#include <stdint.h>

#define RE_VISIT_TIME       1   // 片轮查询List的时间间隔[单位为秒]
#define MAX_TIME_TO_LIVE    2   // 一个Node[包]在TIME_TO_LIVE次访问后还没有被消除
                                //      --> 这里是两秒后如果没有收到确认消息
                                //      --> 则被认定为发送失败，重发数据
#define MAX_RESEND_TIME     3   //  最大重发次数
                                //      --> 这里是指如果重发三次还是没有收到信息
                                //      --> 则认定断开链接

/* Error Code 定义 */
#ifndef LINKC_PACKAGE_LIST_ERROR_CODE
#define LINKC_PACKAGE_LIST_ERROR_CODE

#define LINKC_PACKAGE_LIST_OK           0
#define LINKC_PACKAGE_LIST_ERROR        1
#define LINKC_PACKAGE_LIST_NOT_FOUNT    2

#endif

struct PackageListNode_t{
    void                        *Package;       //  发送的数据
    struct PackageListNode_t    *Perv;          //  上一个节点
    struct PackageListNode_t    *Next;          //  下一个节点
    uint16_t                     MessageLength; //  数据长度
    uint8_t                       TimeToLive;   //  剩余生存次数
    uint8_t                      ResendTime;    //  目前重发次数
    uint32_t                     Count;         //  数据包的计数
};

struct PackageList_t{
    int                         TotalNode;      //  现在节点的总数  
    pthread_mutex_t             *MutexLock;     //  互斥锁
    struct PackageListNode_t    *StartNode;     //  起始节点
};

/* 重定义 */
typedef struct PackageListNode_t    PackageListNode;
typedef struct PackageList_t        PackageList;


/* 函数部分 */
PackageList*    BuildPackageList                (void);
int             DestroyPackageList              (PackageList *List);
int             InsertPackageListNode           (PackageList *List, void *Package, uint32_t Count);
int             FindPackageListNode             (PackageList *List, uint32_t Count, PackageListNode* Node);
int             RemovePackageListNode           (PackageList *List, uint32_t Count);



#endif
