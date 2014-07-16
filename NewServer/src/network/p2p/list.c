#include "../../../include/linkc_p2p_list.h"
#include "../../../include/linkc_error.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>

int list_init(list_t *list){
    if(list == NULL){
        LinkC_Debug("参数为空[0]",LINKC_WARNING);
        return -1;
    }
    list->head = NULL;
    list->size = 0;

    return LINKC_SUCCESS;
}

int list_destroy(list_t *list){
    if(list == NULL){
        LinkC_Debug("参数为空[1]",LINKC_WARNING);
        return -1;
    }

    list_node *node;

    while (list->head != NULL)
    {
        node = list->head;
        list->head = node->next;
        free(node);
    }
    return LINKC_SUCCESS;
}

int list_connection_find(list_t *list, conn_info info,struct sockaddr_in *Dest){
    if(list == NULL){
        LinkC_Debug("参数为空[2]",LINKC_WARNING);
        return -1;
    }
    list_node *node;
    node = list->head;

    while(1){
        if (node == NULL)        // If The Next points NULL,Break the loop
            break;
        if(node->info.Src.sin_addr.s_addr==info.Dest.sin_addr.s_addr && node->info.Dest.sin_addr.s_addr==info.Src.sin_addr.s_addr){
            *Dest=node->info.Src;
            return LINKC_SUCCESS;
        }
        node=node->next;
    }
    return -14;    // Error Not Found
}

int list_item_add(list_t *list,conn_info item){     // maybe there is unknown bug;
    if(list == NULL){
        LinkC_Debug("参数为空[3]",LINKC_WARNING);
        return -1;
    }

    list_node *node = (list_node *)malloc(sizeof(list_node));

    memcpy((void *)&(node->info),(void*)&item,sizeof(conn_info));

    node->next=list->head;
    node->perv=NULL;
    if(list->size == 0)
        list->head=node;
    else{
        list->head->perv=node;
        list->head=node;
    }
    list->size++;
    return LINKC_SUCCESS;
}

int list_node_remove(list_t *list,conn_list_item *item)
{
    if(list == NULL){
        LinkC_Debug("参数为空[4]",LINKC_WARNING);
        return -1;
    }
    list_node *node;
    node = list->head;

    while(1){
        if(node->info.Src.sin_addr.s_addr==item->info.Dest.sin_addr.s_addr && node->info.Dest.sin_addr.s_addr==item->info.Src.sin_addr.s_addr){
            if(node->next != NULL)
                node->next->perv = node->perv;
            if(node->perv != NULL)
                node->perv->next = node->next;
            if(list->size == 1)
                list->head = NULL;
            free(node);
            list->size--;
            return LINKC_SUCCESS;
        }
        if(node->next == NULL)    break;
        node = node->next;
    }
    return -14;
}
