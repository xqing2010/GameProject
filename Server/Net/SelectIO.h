#ifndef _SELECTIO_H_
#define _SELECTIO_H_

#include "NetIO.h"

#include <map>
using namespace std;

class Session;

class SelectIO : public NetIO {
public:
	SelectIO();
	~SelectIO();
public:
	virtual BOOL Init(CSocket *pListenSock);
	virtual VOID Release();

	virtual VOID ProcessInput();
	virtual VOID ProcessOutput();
	virtual VOID ProcessCommands();
	virtual VOID ProcessException();
	virtual VOID Tick();
	virtual VOID AddListenSocket(CSocket *pListenSock);
	virtual VOID AddSession(Session *pSession);
protected:
	VOID AddNewConnection();
	VOID DelConnection(Session *pSession);
	UINT Select();

private:
	CSocket *m_pListenSocket;
	SOCKET   m_nListenSock;
	fd_set   m_oReadSet;
	fd_set   m_oWriteSet;
	fd_set   m_oExceptions;
	fd_set   m_oAllSet;
	int      m_nSockets;
	timeval  m_oTimeOut;

	//-----------------------------------------
	// ¡Ÿ ±¥˙¬Î TODO TO DEL
	// 
	//-----------------------------------------
	map<SOCKET, Session *> m_oSessions;
};

#endif