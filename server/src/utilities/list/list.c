/*
 * Author		： Junfeng Zhang <564691478@qq.com>
 * Last-Change		： March 15, 2014
 */

#include "def.h"
#include "list.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>

errorcode list_init(list_t *list)
{
	CHECK_NOT_NULL(list,ERROR_NULL_ARG);

	list->head = NULL;
	list->size = 0;

	return LINKC_SUCCESS;
}

errorcode list_destroy(list_t *list)
{
	CHECK_NOT_NULL(list,ERROR_NULL_ARG);

	list_node *node;

	while (list->head != NULL)
	{
		node = list->head;
		list->head = node->next;
		free(node);
	}
	return LINKC_SUCCESS;
}

errorcode list_connection_find(list_t *list, conn_info info,struct sockaddr_in *Dest,flag_t FLAG)
{
	CHECK_NOT_NULL(list,ERROR_NULL_ARG);
	list_node *node;
	node = list->head;

	while(1)
	{
		if (node == NULL)		// If The Next points NULL,Break the loop
			break;

		if(node->info.Src.sin_addr.s_addr==info.Dest.sin_addr.s_addr && node->info.Dest.sin_addr.s_addr==info.Src.sin_addr.s_addr)
		{
			*Dest=node->info.Src;
			return LINKC_SUCCESS;
		}
		node=node->next;
	}
	return -14;	// Error Not Found
}

errorcode list_item_add(list_t *list,conn_info item)	// Error???
{
	CHECK_NOT_NULL(list,ERROR_NULL_ARG);

	list_node *node = (list_node *)malloc(sizeof(list_node));

	memcpy((void *)&(node->info),(void*)&item,sizeof(conn_info));

	node->next=list->head;
	node->perv=NULL;
	if(list->size == 0)
		list->head=node;
	else
	{
		list->head->perv=node;
		list->head=node;
	}
	list->size++;
	return LINKC_SUCCESS;
}

errorcode list_node_remove(list_t *list,conn_list_item *item)
{
	CHECK_NOT_NULL(list,ERROR_NULL_ARG);
	list_node *node;
	node = list->head;

	while(1)
	{
		if(node->info.Src.sin_addr.s_addr==item->info.Dest.sin_addr.s_addr && node->info.Dest.sin_addr.s_addr==item->info.Src.sin_addr.s_addr)
		{
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
		if(node->next == NULL)	break;
		node = node->next;
	}
	return ERROR_NOT_FOUND;
}
