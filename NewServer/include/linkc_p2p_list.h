#ifndef _LIST_H_
#define _LIST_H_
#include <sys/types.h>
#include <sys/socket.h>
#include <arpa/inet.h>

struct conn_info_t{
    struct sockaddr_in Src;
    struct sockaddr_in Dest;
};

struct list_node_t{
    struct list_node_t *perv;
    struct list_node_t *next;
    struct conn_info_t  info;
};


struct list{
    struct list_node_t *head;
    int size;
};

struct conn_list_item_t{
    struct conn_info_t info;    
};
typedef struct conn_info_t conn_info;
typedef struct list_node_t list_node;
typedef struct list list_t;
typedef struct conn_list_item_t conn_list_item;
typedef struct conn_list_item_t conn_list_item;

int list_init             (list_t *list);
int list_destroy          (list_t *list);
int list_connection_find  (list_t *list,conn_info info,struct sockaddr_in* Dest);
int list_item_add         (list_t *list,conn_info item);
int list_node_remove      (list_t *list,conn_list_item *item);

struct conn_list_t{
    list_t list;
    pthread_mutex_t mutex;
};
typedef struct conn_list_t conn_list;

int conn_list_init(conn_list *list);
int conn_list_destroy(conn_list *list);
int conn_list_add(conn_list *list,conn_list_item item);
int conn_list_find(conn_list *list,conn_info info,struct sockaddr_in *addr);
int conn_list_remove(conn_list *list,conn_list_item *info);
#endif
