#ifndef LINKC_PACKAGE_LIST
#define LINKC_PACKAGE_LIST

#include <time.h>

#define RE_VISIT_TIME   1       // 片轮查询List的时间间隔[单位为秒]
#define TIME_TO_LIVE    2       // 一个Node[包]在TIME_TO_LIVE次访问后还没有被消除
                                //  --> 这里是两秒后如果没有收到确认消息
                                //  --> 则被认定为发送失败，重发数据

/* Error Code 定义 */
#ifndef LINKC_PACKAGE_LIST_ERROR_CODE
#define LINKC_PACKAGE_LIST_ERROR_CODE

#define LINKC_PACKAGE_LIST_OK           0
#define LINKC_PACKAGE_LIST_ERROR        1
#define LINKC_PACKAGE_LIST_NOT_FOUNT    2

#endif

struct PackageNode_t{
    void                    *Package;       //  发送的数据
    struct PackageNode_t    *Perv;          //  上一个节点
    struct PackageNode_t    *Next;          //  下一个节点
    int                     TimeToLive;     //  剩余生存次数
};

struct PackageList_t{
    int                     TotalNode;      //  现在节点的总数  
    struct PackageNode_t    *StartNode;     //  起始节点
};

/* 重定义 */
typedef struct PackageNode_t    PackageNode;
typedef struct PackageList_t    PackageList;


/* 函数部分 */
PackageList*    BuildPackageList        (void);
int             DestroyPackageList      (PackageList* List);
int             InsertPackageListNode   (PackageList* List, void *Package);
int             FindPackageListNode     (time_t Time);
int             RemovePackageListNode   (time_t Time);



#endif
