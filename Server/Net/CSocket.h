#ifndef _CSOCKET_H_
#define _CSOCKET_H_

#include "SocketAPI.h"

class CSocket {
public:
	CSocket();
	CSocket(const CHAR *psczIP, UINT port);
	CSocket(SOCKET s, const struct sockaddr_in *addr);

	virtual ~CSocket();
public:
	BOOL Create();
	VOID Close();
	BOOL Connect(const CHAR *psczIP = NULL, UINT port = -1);
	BOOL Reconnect(const CHAR *psczIP, UINT port);
	BOOL Bind(UINT port);
	BOOL Listen(INT backlog);
	SOCKET Accept(struct sockaddr *addr, UINT *addrLen);
	CSocket *Accept();

	INT  Send(const VOID *buf, INT len, UINT flags = 0);
	INT  Recv(VOID* buf, UINT len, UINT flags = 0);

	BOOL IsValidSocket() const;
	BOOL IsSocketError() const;
	SOCKET GetSocket() const;

	UINT GetSendBuffSize() const;
	BOOL SetSendBuffSize(UINT size);

	UINT GetRecvBuffSize() const;
	BOOL SetRecvBuffSize(UINT size);

	BOOL IsNonBlocking() const;
	BOOL SetNonBlocking(BOOL on = TRUE);
	UINT GetSockErrorID() const;
	string GetSockErrorMsg() const;

	UINT GetAvaliable() const;
	BOOL GetSockOpt(INT optname, VOID* optval, UINT* optlen, INT level = SOL_SOCKET);
	BOOL SetSockOpt(INT optname, const VOID* optval, UINT optlen, INT level = SOL_SOCKET);

private:
	SOCKET m_nSocketID;
	SOCKADDR_IN m_oSockAddr;
	CHAR m_szIP[IP_SIZE];
	UINT m_nPort;
	BOOL m_bNoBlocking;
};

#endif