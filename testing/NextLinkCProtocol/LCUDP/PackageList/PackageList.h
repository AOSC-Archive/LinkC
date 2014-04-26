#ifndef LINKC_PACKAGE_LIST
#define LINKC_PACKAGE_LIST

#include <time.h>
#include <pthread.h>
#include <stdint.h>


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
