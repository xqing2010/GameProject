#ifndef _NETDEFINE_H_
#define _NETDEFINE_H_

#include "BaseTypeDef.h"

#if defined( WIN32 )
#undef FD_SETSIZE
#define FD_SETSIZE 1024

#include <WinSock2.h>
#define SOCKET_ERROR_WOULDBLOCK WSAEWOULDBLOCK
#elif defined( __LINUX__ )
#include <arpa/inet.h>		//for inet_xxx
#include <netinet/in.h>		//for sockaddr
#include <sys/stat.h>		//for isfdtype
#include <sys/types.h>
#include <sys/socket.h>		//for socket
#include <sys/ioctl.h>		//for ioctl
#include <unistd.h>			//for close	
#include <fcntl.h>			//for fcntl


typedef INT SOCKET;
#define INVALID_SOCKET	-1
#define SOCKET_ERROR	-1
#endif

#define IP_SIZE 16

#define SOCKET_SENDBUFF_OVERFLOW    (-2)
#define SOCKET_SEND_ERROR_HLT (-3)	//send error when buff stream head less than tail 
#define SOCKET_SEND_ERROR_HGT (-4)  //send error when buff stream head great than tail
#define SOCKET_RECVBUFF_OVERFLOW    (-5)
#define SOCKET_BUFF_RESIZE_FAILED    (-6)
#define SOCKET_RECV_ZERO    (-7)


#endif
