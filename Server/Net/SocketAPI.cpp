#include "SocketAPI.h"

#if defined(WIN32)
#include <windows.h>
#include <WS2tcpip.h>
#pragma comment(lib, "WS2_32")
#endif

map<int, pair<int, string> > g_oErrorMsg;

#if defined(WIN32)
class WinSockInit {
public:
	WinSockInit() {
		WORD wVersionRequested;
		WSADATA wsaData;
		wVersionRequested = MAKEWORD(2, 2);
		::WSAStartup(wVersionRequested, &wsaData);
	}
	~WinSockInit() {
		::WSACleanup();
	}
};
static WinSockInit s_oInit;
#endif

void UpdateSocketError() {

#if defined(WIN32)

#ifdef _UNICODE
	wchar_t szBuff[1024] = { 0 };
	typedef LPWSTR STRTYPE;
#else
	char szBuff[1024] = { 0 };
	typedef LPSTR STRTYPE;
#endif
	DWORD eCode = ::WSAGetLastError();
	::FormatMessage(FORMAT_MESSAGE_FROM_SYSTEM,
		NULL, 
		eCode, 
		MAKELANGID(LANG_ENGLISH, SUBLANG_ENGLISH_US),
		(STRTYPE)szBuff,
		0, NULL);

	string errorMsg;
#ifdef _UNICODE
	mbstate_t mbstate;
	memset(&mbstate, 0, sizeof(mbstate));

	int requiredNum = wcrtomb(NULL, *szBuff, &mbstate);
	char *pBuff = new char[requiredNum + 1];
	memset(pBuff, 0, sizeof(char) * (requiredNum + 1));
	wcrtomb(pBuff, *szBuff, &mbstate);
	errorMsg = pBuff;
	SAFE_DELETE(pBuff);
#else
	errorMsg = szBuff;
#endif

	DWORD threadId = ::GetCurrentThreadId();
	g_oErrorMsg[threadId] = make_pair(eCode, errorMsg);

#elif defined(__LINUX__)

#endif

}

namespace SocketAPI {

	SOCKET Socket(INT domain, INT type, INT protocol) {
		SOCKET s = ::socket(domain, type, protocol);
		if (INVALID_SOCKET == s) {
			UpdateSocketError();
		}
		return s;
	}
	BOOL   Bind(SOCKET s, const struct sockaddr * addr, UINT addrlen) {
		INT ret = bind(s, addr, addrlen);
		if (SOCKET_ERROR == ret) {
			UpdateSocketError();
		}
		return SOCKET_ERROR != ret;
	}

	BOOL   Connect(SOCKET s, const struct sockaddr * addr, UINT addrlen) {
		INT ret = connect(s, addr, addrlen);
		if (SOCKET_ERROR == ret) {
			UpdateSocketError();
		}

		return SOCKET_ERROR != ret;
	}
	BOOL   Listen(SOCKET s, UINT backlog) {
		INT ret = listen(s, backlog);
		if (SOCKET_ERROR == ret) {
			UpdateSocketError();
		}
		return SOCKET_ERROR != ret;
	}
	SOCKET Accept(SOCKET s, struct sockaddr* addr, UINT* addrlen) {
#if defined(WIN32)
		SOCKET client = accept(s, addr, (INT *)addrlen);
#elif defined(__LINUX__)
		SOCKET client = accept(s, addr, addrlen);
#endif
		if (INVALID_SOCKET == client) {
			UpdateSocketError();
		}
		return client;
	}
	INT   Send(SOCKET s, const VOID* buf, UINT len, UINT flags) {
		INT nSend;
#if  defined(WIN32)
		nSend = send(s, (const CHAR *)buf, len, flags);
#elif defined(__LINUX__)
		nSend = send(s, buf, len, flags);
#endif

		if (SOCKET_ERROR == nSend) {
			UpdateSocketError();
		}
		return nSend;
	}

	INT   Recv(SOCKET s, VOID* buf, UINT len, UINT flags) {
#if defined(WIN32)
		INT nRecv = recv(s, (CHAR*)buf, len, flags);
#elif defined(__LINUX__)
		INT nRecv = recv(s, buf, len, flags);
#endif
		if (SOCKET_ERROR == nRecv) {
			UpdateSocketError();
		}
		return nRecv;
	}

	INT   SendTo(SOCKET s, const VOID* buf, INT len, UINT flags, const struct sockaddr* to, INT tolen) {
#if defined(WIN32)
		INT nSend = sendto(s, (const CHAR *)buf, len, flags, to, tolen);
#elif defined(__LINUX__)
		INT nSend = sendto(s,buf,len,flags,to,tolen);
#endif
		if (SOCKET_ERROR == nSend) {
			UpdateSocketError();
		}
		return nSend;
	}
	INT   RecvFrom(SOCKET s, VOID* buf, INT len, UINT flags, struct sockaddr* from, UINT* fromlen) {
#if defined(WIN32)
		INT nRecv = recvfrom(s, (CHAR*)buf, len, flags, from, (int*)fromlen);
#elif defined(__LINUX__)
		INT nRecv = recvfrom(s,buf,len,flags,from,fromlen);
#endif
		if (SOCKET_ERROR == nRecv) {
			UpdateSocketError();
		}

		return nRecv;
	}

	BOOL   CloseSocket(SOCKET s) {
#if defined(WIN32)
		if (SOCKET_ERROR == closesocket(s)) {
			UpdateSocketError();
			return FALSE;
		}
#elif defined(__LINUX__)
		close(s);
#endif
		return TRUE;
	}
	BOOL   ShutDown(SOCKET s, UINT how) {
		if (shutdown(s, how) < 0) {
			UpdateSocketError();
			return FALSE;
		}
		return TRUE;
	}

	INT Select(INT maxfdp1, fd_set* readset, fd_set* writeset, fd_set* exceptset, struct timeval* timeout) {
		INT result = select(maxfdp1, readset, writeset, exceptset, timeout);
		if (SOCKET_ERROR == result) {
			UpdateSocketError();
		}
		return result;
	}

	BOOL   GetSockOpt(SOCKET s, INT level, INT optname, VOID* optval, UINT* optlen) {
#if defined(WIN32)
		if (SOCKET_ERROR == getsockopt(s, level, optname, (CHAR*)optval, (int*)optlen)) {
			UpdateSocketError();
			return FALSE;
		}
#elif defined(__LINUX__)
		if (SOCKET_ERROR == getsockopt(s , level , optname , optval , optlen)) {
			return FALSE;
		}
#endif
		return TRUE;
	}
	BOOL   SetSockOpt(SOCKET s, INT level, INT optname, const VOID* optval, UINT optlen) {
#if defined(WIN32)
		if (SOCKET_ERROR == setsockopt(s, level, optname, (CHAR*)optval, optlen)) {
			UpdateSocketError();
			return FALSE;
		}
#elif defined(__LINUX__)
		if (SOCKET_ERROR == setsockopt( s , level , optname , optval , optlen )) {
			return FALSE;
		}
#endif
		return TRUE;
	}

	UINT   GetAvaliable(SOCKET s) {
#if defined(WIN32)
		ULONG avaliable = 0;
		::ioctlsocket(s, FIONREAD, &avaliable);
		return avaliable;
#elif defined(__LINUX__)
		UINT arg = 0;
		ioctl(fd, FIONREAD, &arg);
		return arg;
#endif
	}
	BOOL   GetSocketNonBlocking(SOCKET s) {
#if defined(WIN32)
		return FALSE;
#elif defined(__LINUX__)
		INT flags = fcntl ( s , F_GETFL , 0);
		return flags | O_NONBLOCK;
#endif
	}
	BOOL   SetSocketNonBlocking(SOCKET s, BOOL on) {
#if defined(WIN32)
		ULONG argp = (on == TRUE) ? 1 : 0;
		return ioctlsocket(s, FIONBIO, &argp);
#elif defined(__LINUX__)
		INT flags = FileAPI::fcntl_ex(s , F_GETFL , 0);
		if (on)
			// set nonblocking
			flags |= O_NONBLOCK;
		else
			// set blocking
			flags &= ~O_NONBLOCK;
		fcntl(s, F_SETFL, flags);
		return TRUE;
#endif
	}

	pair<int, string>GetErrorMsg() {
#if defined(WIN32)
		DWORD threadId = GetCurrentThreadId();
		return g_oErrorMsg[threadId];
#elif defined(__LINUX__)
		//TODO
		return make_pair(-1, "");
#endif
	}

	BOOL IPConvertPToN(const CHAR *pcszIp, VOID *pAddr, INT family/* = AF_INET*/) {
#if defined(WIN32)
		inet_pton(family, pcszIp, pAddr);
#elif defined(__LINUX__) 

#endif
		return TRUE;
	}
	const CHAR *IPConvertNToP(const SOCKADDR_IN *pAddr, CHAR *pszIp, UINT len) {
#if defined(WIN32)
		inet_ntop(AF_INET, (PVOID )pAddr, pszIp, len);
		//const CHAR *pszTemp = inet_ntoa(pAddr->sin_addr);
		//strncpy(pszIp, pszTemp, len);
		return pszIp;
#elif defined(__LINUX__) 
		if (AF_INET == pAddr->sin_family) {
			assert(len >= INET_ADDRSTRLEN);
			inet_ntop(AF_INET, &pAddr->sin_addr, pszIp, len);
	}
		else if (AF_INET6 == pAddr->sin_family) {
			assert(len >= INET6_ADDRSTRLEN);
			inet_ntop(AF_INET6, &pAddr->sin_addr, pszIp, len);
		}
		return pszIp;
#endif
	}
}