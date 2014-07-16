#include "../../../include/linkc_p2p_list.h"
#include "../../../include/linkc_error.h"
#include <pthread.h>
#include <string.h>
#include <stdio.h>

#define FAILED(x) ((x < LINKC_SUCCESS) ? 1 : 0)
#define CHECK_FAILED(x,y) ({ if (FAILED(x)) return y; })
#define CHECK_NOT_NULL(x,z) ({if (x==NULL) return z;})

int conn_list_init(conn_list *list){
    if(list != NULL){
        LinkC_Debug("参数不为空[0]",LINKC_WARNING);
        return -1;
    }
    if(list_init(&list->list) <0){
        LinkC_Debug("初始化链表",LINKC_FAILURE);
    }
    if (pthread_mutex_init(&(list->mutex),NULL)<0)
        return LINKC_FAILURE;

    return LINKC_SUCCESS;
}

int conn_list_destroy(conn_list *list){
    if(list == NULL){
        LinkC_Debug("参数为空[5]",LINKC_WARNING);
        return LINKC_FAILURE;
    }
    if(pthread_mutex_lock(&(list->mutex)) < 0){
        LinkC_Debug("互斥锁上锁",LINKC_FAILURE);
        return LINKC_FAILURE;
    }
    list_destroy(&(list->list));
    pthread_mutex_unlock(&(list->mutex));
    pthread_mutex_destroy(&(list->mutex));

    return LINKC_SUCCESS;
}

int conn_list_add(conn_list *list,conn_list_item item){
    CHECK_NOT_NULL(list,-1);
    CHECK_FAILED(pthread_mutex_lock(&(list->mutex)),-1);
    CHECK_FAILED(list_item_add(&(list->list),item.info),-1);
    CHECK_FAILED(pthread_mutex_unlock(&(list->mutex)),-1);    //解锁

    return LINKC_SUCCESS;
}

int conn_list_find(conn_list *list,conn_info info,struct sockaddr_in *Dest){
    struct sockaddr_in k;
    CHECK_NOT_NULL(list,-1);
    CHECK_FAILED(pthread_mutex_lock(&(list->mutex)),-1);
    int i = list_connection_find(&(list->list),info,&k);
    if (i == -14){      //  if not fount
        CHECK_FAILED(pthread_mutex_unlock(&(list->mutex)),-1);  //解锁
        return -14;    // Return Not Found
    }
    CHECK_FAILED(pthread_mutex_unlock(&(list->mutex)),-1);      //解锁
    *Dest = k;

    return LINKC_SUCCESS;
}

int conn_list_remove(conn_list *list, conn_list_item *item)
{
    CHECK_NOT_NULL(list,-1);
    CHECK_FAILED(pthread_mutex_lock(&(list->mutex)),-1);
    int i = list_node_remove(&(list->list),item);
    if (i == -14){
        return LINKC_FAILURE;
    }
    CHECK_FAILED(pthread_mutex_unlock(&(list->mutex)),-1);      //解锁
    return LINKC_SUCCESS;
}
