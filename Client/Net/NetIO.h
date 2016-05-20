#ifndef _NETIO_H_
#define _NETIO_H_
#include "BaseTypeDef.h"
#include "Session.h"
class NetIO {
public:
	virtual BOOL Init(CSocket *pListenSock) = 0;
	virtual VOID Release() = 0;

	virtual VOID ProcessInput() = 0;
	virtual VOID ProcessOutput() = 0;
	virtual VOID ProcessCommands() = 0;
	virtual VOID ProcessException() = 0;
	virtual VOID Tick() = 0;
	virtual VOID AddListenSocket(CSocket *pListenSock) = 0;
	virtual VOID AddSession(Session *pSession) = 0;
};

#endif