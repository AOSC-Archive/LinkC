#include "PackageList.h"
#include "../Protocol/LinkC_NetWorkProtocol.h"
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <pthread.h>

PackageList* BuildPackageList(void){
    PackageList *List = (PackageList*)malloc(sizeof(PackageList));  // 为链表分配内存

    List->TotalNode = 0;                        //  初始化TotalNode为0
    List->StartNode = NULL;                     //  挂空指针
    List->MutexLock = (pthread_mutex_t*)malloc(sizeof(pthread_mutex_t));    //  为互斥锁分配内存空间

    pthread_mutex_init(List->MutexLock,NULL);   //  默认形式初始化互斥锁
                                                //  --> 建立互斥锁的函数原型为
                                                //      --> int pthread_mutex_init(pthread_mutex_t *,__const pthread_mutexattr_t *);
                                                //      --> 参数:
                                                //          --> [1] pthread_mutex_t *       互斥锁的指针
                                                //          --> [2] pthread_mutexattr_t *   互斥锁参数
                                                //              --> 互斥锁参数我没啥研究，不过传入参数为NULL的话就是默认属性

    return List;                                //  返回指针
}

int DestroyPackageList(PackageList *List){
    if(List == NULL){                           //  如果指针为空
        printf("PackageList is NULL!\n");       //  输出出错
        return 1;                               //  返回错误
    }
    pthread_mutex_destroy(List->MutexLock);     //  销毁互斥锁
    PackageListNode *Node;                      //  用于保存当前节点
    PackageListNode *Next;                      //  用于保存下一个节点
    Node = List->StartNode;                     //  当前节点设置为链表开始节点
    while(Node){                                //  无限循环
        Next = Node->Next;                      //  保存下一个节点的地址
        free(Node->Package);                    //  删除信息
        free(Node);                             //  删除节点
        if(Next == NULL)    break;              //  如果下一个节点为空，则跳出循环
        Node = Next;                            //  将当前节点设置为下一个节点
    }
    return 0;
}


int InsertPackageListNode (PackageList* List, void *Package, uint32_t Count){
    if(List == NULL || Package == NULL){        //  如果指针为空
        printf("Argument is NULL!\n");          //  输出出错
        return 1;                               //  返回错误
    }
    PackageListNode *Node       = NULL;         //  声明节点[最好把变量声明在上锁之前,使上锁后执行时间最短]
    PackageListNode *NowNode    = NULL;         //  当前节点
    Node = (PackageListNode *)malloc(sizeof(PackageListNode));  //  分配内存

    pthread_mutex_lock  (List->MutexLock);      //  上锁互斥锁[上锁后才进行操作]

    if(List->TotalNode == 0){                   //  如果节点总数为0
        Node->Next          = NULL;             //  设置下一个节点为NULL
        Node->Perv          = NULL;             //  设置上一个节点为NULL
        Node->ResendTime    = 0;                //  现在重发次数为0
        Node->TimeToLive    = MAX_TIME_TO_LIVE; //  剩余生存时间为最大生存时间
        Node->Count         = Count;            //  计数次数为当前传入参数的计数次数
        pthread_mutex_unlock(List->MutexLock);  //  解锁互斥锁
        return 0;                               //  函数返回
    }else{
        NowNode = List->StartNode;              //  将当前节点设置为开始节点
        while(1){                               //  永久循环
            if(NowNode->Count < Count){         //  如果当前节点的计数次小于传入参数的计数次
                if(NowNode->Perv != NULL){      //  如果现在节点的前一个[一定比当前节点的计数次大]
                    Node->Perv      = NowNode->Perv;//  新建节点的上一个设置为当前节点的上一个
                    Node->Perv->Next= Node;         //  新建节点的上一个的下一个设置为新建节点
                    NowNode->Perv   = Node;         //  当前节点的上一个设置为新建节点
                    Node->Next      = NowNode;      //  新建节点的下一个设置为当前节点
                    Node->Package   = Package;      //  新建节点的数据包设置为传入参数的数据包
                    Node->ResendTime    = 0;                //  现在重发次数为0
                    Node->TimeToLive    = MAX_TIME_TO_LIVE; //  剩余生存时间为最大生存时间
                    Node->Count         = Count;            //  计数次数为当前传入参数的计数次数
                    pthread_mutex_unlock(List->MutexLock);  //  解锁互斥锁
                    return 0;                               //  退出函数
                }else{                                      //  如果当前节点的前一个节点为空[意味着当前节点为首节点]
                    Node->Next      = List->StartNode;      //  将新建的节点的下一个设置为链表的开始节点
                    Node->Perv      = NULL;                 //  将新建的节点的前一个设置为空
                    Node->TimeToLive= MAX_TIME_TO_LIVE;     //  设置剩余生存时间为最大生存时间
                    Node->ResendTime= 0;                    //  设置重发次数为0
                    Node->Package   = Package;
                    List->StartNode->Perv = Node;           //  将链表的开始节点的前一个个设置为新建节点
                    List->StartNode = Node;                 //  将链表的开始节点设置为新建节点
                    pthread_mutex_unlock(List->MutexLock);  //  解锁互斥锁
                    return 0;                               //  函数返回
                }
            }else if(NowNode->Count == Count){          //  当前节点的计数次等于传入参数的计数次
                free(Node);                             //  释放分配的内存
                pthread_mutex_unlock(List->MutexLock);  //  解锁互斥锁
                return 1;                               //  返回错误
            }else if(NowNode->Next == NULL){            //  如果这是最后一个节点[当前节点的下一个节点为空]
                Node->Perv      = NowNode;              //  新建节点的前一个节点为当前节点
                Node->Next      = NULL;                 //  新建节点的下一个节点为空
                Node->TimeToLive= MAX_TIME_TO_LIVE;     //  设置剩余生存时间为最大生存时间
                Node->ResendTime= 0;                    //  设置重发次数为0
                Node->Package   = Package;              //  数据包为当前传入数据的数据包
                pthread_mutex_unlock(List->MutexLock);  //  解锁互斥锁
                return 0;                               //  函数返回0
            }else   NowNode = NowNode->Next;            //  设置当前节点为当前节点的下一个节点
        }
    }
    pthread_mutex_unlock(List->MutexLock);              //  解锁互斥锁
    return 1;                                           //  按照上面的逻辑顺序是不可能到达这里的
}


int FindPackageListNode (PackageList *List, uint32_t Count, PackageListNode *Node){
    if(List == NULL){                               //  如果链表指针为空
        printf("Argument is NULL\n");               //  打印错误信息
        return LINKC_PACKAGE_LIST_ERROR;            //  返回错误
    }
    Node = List->StartNode;                         //  设置节点为链表起始节点
    while(Node){                                    //  如果节点不为空
        if(Node->Count == Count){                   //  如果找到
            return LINKC_PACKAGE_LIST_OK;           //  返回成功
        }
        Node = Node->Next;                          //  查找下一个
    }
    return LINKC_PACKAGE_LIST_NOT_FOUNT;            //  返回未找到
}

int RemovePackageListNode (PackageList *List, uint32_t Count){
    if(List == NULL){                           //  如果链表指针为空
        printf("Argument is NULL\n");           //  打印错误信息
        return LINKC_PACKAGE_LIST_ERROR;        //  返回错误
    }
    PackageListNode *Node;                      //  声明一个节点指针
    Node = List->StartNode;                     //  设置节点为链表起始节点
    pthread_mutex_lock  (List->MutexLock);      //  上锁互斥锁
    if(FindPackageListNode(List,Count,Node) == LINKC_PACKAGE_LIST_OK){    //  如果找到
        if(Node->Next != NULL){                 //  如果节点后面还有节点
            Node->Next->Perv = Node->Perv;      //  则节点后面一个节点的前一个为当前节点的前一个
        }
        if(Node->Perv != NULL){                 //  如果节点前面还有节点
            Node->Perv->Next = Node->Next;      //  设置当前需要删除的节点的钱一个节点的后一个节点设置为当前需要删除的节点的下一个节点
        }else{                                  //  如果前面没有节点了[也就是说是首节点]
            List->StartNode = NULL;             //  链表的开始节点挂空
        }
        List->TotalNode --;                     //  总节点减一
        free(Node->Package);                    //  释放内存
        free(Node);                             //  释放内存
        pthread_mutex_unlock(List->MutexLock);  //  解锁互斥锁
        return LINKC_PACKAGE_LIST_OK;           //  返回成功
    }
    pthread_mutex_unlock(List->MutexLock);      //  解锁互斥锁
    return LINKC_PACKAGE_LIST_NOT_FOUNT;
}
