#include "PackageList.h"
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <pthread.h>

PackageList* BuildPackageList(void){
    PackageList *List = (PackageList*)malloc(sizeof(PackageList));  // 为链表分配内存

    List->TotalNode = 0;                        //  初始化TotalNode为0
    List->StartNode = NULL;                     //  挂空指针

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
    while(1){                                   //  无限循环
        Next = Node->Next;                      //  保存下一个节点的地址
        free(Node->Package);                    //  删除信息
        free(Node);                             //  删除节点
        if(Next == NULL)    break;              //  如果下一个节点为空，则跳出循环
        Node = Next;                            //  将当前节点设置为下一个节点
    }
    return 0;
}
