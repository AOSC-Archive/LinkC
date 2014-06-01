#ifndef LINKC_PACKAGE_CONTROL_
#define LINKC_PACKAGE_CONTROL_

#include <stddef.h>     //  For size_t
#include "../Protocol/LinkC_NetWorkProtocol.h"
#include <openssl/rsa.h>
#include <openssl/pem.h>
#include <openssl/err.h>

int	PackMessage(void* Message, size_t Length, LinkC_Socket* Socket, void* Output);
/*
 * TODO:	Package a message for sending
 *
 * ARGS:
 *	[1] Type void*	A point to the message you want to send
 *	[2] Type size_t	The size of the first argument
 *	[3] Type void*	A malloced point , to return value[packaged message]
 *
 * RETN:
 *	[SUCCESS]	0
 *	[FAILURE]	1
 */

 int _PackMessage(int Type, void* Message, size_t Length, LinkC_Socket *Socket, void* Output);

int	UnPackMessage(void* Message, LinkC_Socket* Socket, void* Output);
/*
 * TODO:	Package a message for sending
 *
 * ARGS:
 *	[1] Type void*	A point to the message you recved
 *	[2] Type void*	A malloced point , to return value[unpackaged message]
 *
 * RETN:
 *	[SUCCESS]	0
 *	[FAILURE]	1
 */

int	EncryptPackage(void* Message, int Length, void* Output, RSA* PublicKey);
/*
 * TODO:	Encrypt a message for sending
 *		and return a length-same point
 *
 * ARGS:
 *	[1] Type void*	A point to the message you want to send
 *	[2] Type int	The size of the first argument
 *	[3] Type void*	A malloced point , to return value[packaged message]
 *
 * RETN:
 *	[SUCCESS]	Message's Length
 *	[FAILURE]	-1
 */

int DecryptPackage(void* Message,int Length, void* Output, RSA* PrivateKey);
/*
 * TODO:	Unencrypt a message you recved
 *
 * ARGS:
 *	[1] Type void*	The message you recved
 *	[2] Type void*	return the unencryed package
 * RETN:
 *	[SUCCESS}	the length of the message
 *	[FAILURE]	-1
 *
 */

#endif
