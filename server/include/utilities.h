#ifndef UTILITIES_H
#define UTILITIES_H
#include <sys/types.h>
#include <sys/socket.h>
#include "def.h"

#define uint8_t		        Message_Header
#define uint16_t		Message_Size
#define END_OF_LINKC_MESSAGE    "3036ACFB8D33BBC4"
#define START_OF_LINKC_MESSAGE	"7791A13CAA92A8B2"
#define CHECK_CODE_LENTH	16

#define NOT_MESSAGE		-1	// 非消息
#define EXCEED_THE_LIMIT	-2	// 大小超出限制
#define MESSAGE_INCOMPLETE	1	// 数据不完整
#define DATA_SIZE_LIMITED	512	// 单个包数据段最大限制
#define HEADER_SIZE		5	// Header的大小
#define MESSAGE_POOL_SIZE	15	// 发送消息池的大小
#define RECV_POOL_SIZE		10240	// 接受缓冲区大小
struct linkc_message_header_t
{
	uint8_t		Mark;		// Data's Mark[A Random Number]
	uint8_t		Type;		// Message Type[Header]
	uint8_t		Totle;		// Totle Message count;
	uint8_t		Current;	// Current Message
	uint16_t	Lenth;		// Current Data's Lenth
}
struct linkc_message_pool_t
{
	struct linkc_message_header_t*	data_header;
	uint8_t	Current;
};
typedef struct linkc_message_header_t	linkc_message_header;
typedef struct linkc_message_pool_t	linkc_message_pool;

errorcode	check_message		(const void *message);	// Errorcode Define in def.h
errorcode	send_message		(int sockfd,const void *message,uint8_t size,uint8_t Type,struct sockaddr_in Dest,int FLAG);
errorcode	recv_message		(int sockfd,void *buffer,struct sockaddr_in Dest,int FALG);
errorcoed	send_message_pack	(int sockfd,const void *message,uint8_t size,struct sockaddr_in Dest,int FALG);
errorcode	recv_message_pack	(int sockfd,void *buffer,struct sockaddr_in Dest,int FLAG);
errorcode	build_message_pool	(struct linkc_message_pool_t* pool);
errorcode	destroy_message_pool	(struct linkc_message_pool_t* pool);
errorcode	add_message_pool_item	(struct linkc_message_pool_t* pool,void *data);
void*		get_message_pool_item	(struct linkc_message_pool_t* pool,uint8_t Mark);
void*		unpack_message		(void *message);
void*		pack_message		(uint8_t header,const void* message,int size);


#endif
