#ifndef _LIST_H_
#define _LIST_H_
#include "def.h"
#include <sys/types.h>
#include <sys/socket.h>

struct list_node_t
{
	struct list_node_t *perv;
	struct list_node_t *next;
	conn_info *info;
};
typedef struct list_node_t list_node;

struct list
{
	list_node *head;
	int size;
};
typedef struct list list_t;

errorcode list_init(list_t *list);
errorcode list_destroy(list_t *list);
errorcode list_connection_find(list_t *list,conn_info info,struct sockaddr_in* Dest,flag_t FLAG);
errorcode list_item_add	(list_t *list,conn_info item);
errorcode list_node_remove(list_t *list,conn_info info);




#endif
