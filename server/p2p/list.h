#ifndef _LIST_H_
#define _LIST_H_

#include "def.h"

struct list_node_t
{
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
errorcode list_find(list_t *list,ip_t SrcIp,void **found,flag_t FLAG);
errorcode list_item_add	(list_t *list,conn_info item);
errorcode list_node_remove(list_t *list,list_node *node);




#endif
