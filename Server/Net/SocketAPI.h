#ifndef _SOCKETAPI_H_
#define _SOCKETAPI_H_
#include "NetDefine.h"

#include <map>
#include <string>
using std::map;
using std::pair;
using std::string;

namespace SocketAPI {
	//socket 
	SOCKET Socket(INT domain, INT type, INT protocol);
	BOOL   Bind(SOCKET s, const struct sockaddr * addr, UINT addrlen);
	BOOL   Connect(SOCKET s, const struct sockaddr * addr, UINT addrlen);
	BOOL   Listen(SOCKET s, UINT backlog);
	SOCKET Accept(SOCKET s, struct sockaddr* addr, UINT* addrlen);
	INT    Send(SOCKET s, const VOID* buf, UINT len, UINT flags);
	INT    SendTo(SOCKET s, const VOID* buf, INT len, UINT flags, const struct sockaddr* to, INT tolen);
	INT    Recv(SOCKET s, VOID* buf, UINT len, UINT flags);
	INT    RecvFrom(SOCKET s, VOID* buf, INT len, UINT flags, struct sockaddr* from, UINT* fromlen);
	BOOL   CloseSocket(SOCKET s);
	BOOL   ShutDown(SOCKET s, UINT how);

	INT    Select(INT maxfdp1, fd_set* readset, fd_set* writeset, fd_set* exceptset, struct timeval* timeout);

	//socket operator
	BOOL   GetSockOpt(SOCKET s, INT level, INT optname, VOID* optval, UINT* optlen);
	BOOL   SetSockOpt(SOCKET s, INT level, INT optname, const VOID* optval, UINT optlen);
	UINT   GetAvaliable(SOCKET s);
	BOOL   GetSocketNonBlocking(SOCKET s);
	BOOL   SetSocketNonBlocking(SOCKET s, BOOL on);

	pair<int, string>GetErrorMsg();	//thread safe

	BOOL IPConvertPToN(const CHAR *pcszIp, VOID *pAddr, INT family = AF_INET);
	const CHAR *IPConvertNToP(const SOCKADDR_IN *pAddr, CHAR *pszIp, UINT len);
}

#endif