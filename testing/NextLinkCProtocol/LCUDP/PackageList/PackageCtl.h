#ifndef LINKC_PACKAGE_CONTROL_
#define LINKC_PACKAGE_CONTROL_

int	PackMessage(void* Message, size_t Length, void* Output);
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

int	UnPackMessage(void* Message, void* Output);
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

int	EncryptPackage(void Message, size_t Length, void* Output);
/*
 * TODO:	Encrypt a message for sending
 *		and return a length-same point
 *
 * ARGS:
 *	[1] Type void*	A point to the message you want to send
 *	[2] Type size_t	The size of the first argument
 *	[3] Type void*	A malloced point , to return value[packaged message]
 *
 * RETN:
 *	[SUCCESS]	Message's Length
 *	[FAILURE]	1
 */

int 	UnEncryPackage(void* Message, void* Output);
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
