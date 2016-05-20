#include "CSocket.h"

CSocket::CSocket() {
	m_nSocketID = INVALID_SOCKET;
	memset(&m_oSockAddr, 0, sizeof(m_oSockAddr));
	memset(m_szIP, 0, sizeof(CHAR) * IP_SIZE);
	m_nPort = -1;
	m_bNoBlocking = FALSE;
}

CSocket::CSocket(const CHAR *psczIP, UINT port) : CSocket() {
	strncpy(m_szIP, psczIP, IP_SIZE);
	m_nPort = port;
}

CSocket::CSocket(SOCKET s, const struct sockaddr_in *addr) {
	m_nSocketID = s;
	memcpy(&m_oSockAddr, addr, sizeof(*addr));
	m_nPort = ntohs(m_oSockAddr.sin_port);
	SocketAPI::IPConvertNToP(&m_oSockAddr, m_szIP, IP_SIZE);
	m_bNoBlocking = FALSE;
}

CSocket::~CSocket() {
	;
}

BOOL CSocket::Create() {
	m_nSocketID = SocketAPI::Socket(AF_INET, SOCK_STREAM, 0);
	memset(&m_oSockAddr, 0, sizeof(m_oSockAddr));
	m_oSockAddr.sin_family = AF_INET;
	return IsValidSocket();
}

VOID CSocket::Close() {
	if (IsValidSocket() && FALSE == IsSocketError()) {
		SocketAPI::CloseSocket(m_nSocketID);
	}
	m_nSocketID = INVALID_SOCKET;
	memset(&m_oSockAddr, 0, sizeof(m_oSockAddr));
	memset(m_szIP, 0, IP_SIZE);
	m_nPort = 0;
}

BOOL CSocket::Connect(const CHAR *psczIP /* = NULL */, UINT port /* = -1 */) {
	if (NULL != psczIP) {
		strncpy(m_szIP, psczIP, IP_SIZE);
	}
	if (-1 != port) {
		m_nPort = port;
	}
	m_oSockAddr.sin_port = htons(m_nPort);
	//m_oSockAddr.sin_addr.s_addr = inet_addr(m_szIP);
	SocketAPI::IPConvertPToN(m_szIP, &m_oSockAddr.sin_addr);
	return SocketAPI::Connect(m_nSocketID, (const struct sockaddr *)&m_oSockAddr, sizeof(m_oSockAddr));
}

BOOL CSocket::Reconnect(const CHAR *psczIP, UINT port) {
	Close();
	strncpy(m_szIP, psczIP, IP_SIZE);
	m_nPort = port;

	Create();

	return Connect(m_szIP, m_nPort);
}

BOOL CSocket::Bind(UINT port) {
	m_nPort = port;
	m_oSockAddr.sin_addr.s_addr = htonl(INADDR_ANY);
	m_oSockAddr.sin_port = htons(m_nPort);
	return SocketAPI::Bind(m_nSocketID, (const struct sockaddr *)&m_oSockAddr, sizeof(m_oSockAddr));
}

BOOL CSocket::Listen(INT backlog) {
	return SocketAPI::Listen(m_nSocketID, backlog);
}

SOCKET CSocket::Accept(struct sockaddr *addr, UINT *addrLen) {
	SOCKET client = SocketAPI::Accept(m_nSocketID, addr, addrLen);
	return client;
}

CSocket *CSocket::Accept() {
	sockaddr_in addr;
	memset(&addr, 0, sizeof(addr));
	UINT size = sizeof(addr);
	SOCKET client = Accept((struct sockaddr *)&addr, &size);
	if (INVALID_SOCKET == client) {
		return NULL;
	}

	CSocket *pSocket = new CSocket(client, &addr);
	return pSocket;
}

INT CSocket::Send(const VOID *buf, INT len, UINT flags /* = 0 */) {
	return SocketAPI::Send(m_nSocketID, buf, len, flags);
}

INT CSocket::Recv(VOID* buf, UINT len, UINT flags /* = 0 */) {
	return SocketAPI::Recv(m_nSocketID, buf, len, flags);
}

BOOL CSocket::IsValidSocket() const {
	return INVALID_SOCKET != m_nSocketID;
}

BOOL CSocket::IsSocketError() const {
	INT error;
	UINT len = sizeof(error);

	INT Result = SocketAPI::GetSockOpt(m_nSocketID, SOL_SOCKET, SO_ERROR, &error, &len);

	if (Result == 0)
		return FALSE;
	else
		return TRUE;
}

SOCKET CSocket::GetSocket() const {
	return m_nSocketID;
}

UINT CSocket::GetSendBuffSize() const {
	UINT sendBuffSize = 0;
	UINT size = sizeof(sendBuffSize);
	SocketAPI::GetSockOpt(m_nSocketID, SOL_SOCKET, SO_SNDBUF, &sendBuffSize, &size);
	return sendBuffSize;
}

BOOL CSocket::SetSendBuffSize(UINT size) {
	return SocketAPI::SetSockOpt(m_nSocketID, SOL_SOCKET, SO_SNDBUF, &size, sizeof(UINT));
}

UINT CSocket::GetRecvBuffSize() const {
	UINT recvBuffSize = 0;
	UINT size = sizeof(recvBuffSize);
	SocketAPI::GetSockOpt(m_nSocketID, SOL_SOCKET, SO_RCVBUF, &recvBuffSize, &size);
	return recvBuffSize;

}

BOOL CSocket::SetRecvBuffSize(UINT size) {
	return SocketAPI::SetSockOpt(m_nSocketID, SOL_SOCKET, SO_RCVBUF, &size, sizeof(UINT));
}

BOOL CSocket::IsNonBlocking() const {
	return m_bNoBlocking;
}

BOOL CSocket::SetNonBlocking(BOOL on /* = TRUE */) {
	BOOL ret = SocketAPI::SetSocketNonBlocking(m_nSocketID, on);
	if (ret) {
		m_bNoBlocking = on;
	}
	return ret;
}

UINT CSocket::GetSockErrorID() const {
	return SocketAPI::GetErrorMsg().first;
}

string CSocket::GetSockErrorMsg() const {
	return SocketAPI::GetErrorMsg().second;
}

UINT CSocket::GetAvaliable() const {
	return SocketAPI::GetAvaliable(m_nSocketID);
}

BOOL CSocket::GetSockOpt(INT optname, VOID* optval, UINT* optlen, INT level /* = SOL_SOCKET */) {
	return SocketAPI::GetSockOpt(m_nSocketID, level, optname, optval, optlen);
}

BOOL CSocket::SetSockOpt(INT optname, const VOID* optval, UINT optlen, INT level /* = SOL_SOCKET */) {
	return SocketAPI::SetSockOpt(m_nSocketID, level, optname, optval, optlen);
}
