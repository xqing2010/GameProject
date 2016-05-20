#include "SelectIO.h"

SelectIO::SelectIO() {
	m_nSockets = 0;
}

SelectIO::~SelectIO() {
	;
}

BOOL SelectIO::Init(CSocket *pListenSock) {
	FD_ZERO(&m_oAllSet);
	FD_ZERO(&m_oReadSet);
	FD_ZERO(&m_oExceptions);
	FD_ZERO(&m_oWriteSet);
	m_nSockets = 0;
	m_oTimeOut.tv_sec  = 0;
	m_oTimeOut.tv_usec = 0;
	AddListenSocket(pListenSock);
	return TRUE;
}

VOID SelectIO::Release() {

}

VOID SelectIO::ProcessInput() {
	for (UINT index = 0; index < m_oReadSet.fd_count; ++index) {
		SOCKET s = m_oReadSet.fd_array[index];
		if (s == m_nListenSock) {
			AddNewConnection();
		}
		else {
			//process input socket fill TODO
			if (m_oSessions.end() != m_oSessions.find(s)) {
				Session *pSession = m_oSessions[s];
				BOOL ret = pSession->ProcessInput();
				if (false == ret) {
					DelConnection(pSession);
				}
			}
		}
	}
}

VOID SelectIO::ProcessOutput() {
	for (UINT index = 0; index < m_oWriteSet.fd_count; ++index) {
		//process out  socket  flush
		SOCKET s = m_oWriteSet.fd_array[index];
		if (m_oSessions.end() != m_oSessions.find(s)) {
			Session *pSession = m_oSessions[s];
			pSession->ProcessOutput();
		}
	}
}

VOID SelectIO::ProcessCommands() {
	for (UINT index = 0; index < m_oReadSet.fd_count; ++index) {
		//process commands;
	}
}

VOID SelectIO::ProcessException() {
	for (UINT index = 0; index < m_oExceptions.fd_count; ++index) {
		//process exception
	}
}

UINT SelectIO::Select() {
	m_oReadSet = m_oAllSet;
	m_oExceptions = m_oAllSet;
	m_oWriteSet = m_oAllSet;	//TODO 改成只监听要发送的

	return SocketAPI::Select(m_nSockets, &m_oReadSet, &m_oWriteSet, &m_oExceptions, &m_oTimeOut);
}


VOID SelectIO::Tick() {
	Select();
	ProcessInput();
	ProcessException();
	ProcessCommands();
	ProcessOutput();
}

VOID SelectIO::AddListenSocket(CSocket *pListenSock) {
	m_pListenSocket = pListenSock;
	m_nListenSock   = m_pListenSocket->GetSocket();
	++m_nSockets;
}

VOID SelectIO::AddSession(Session *pSession) {
	SOCKET s = pSession->GetCSocket()->GetSocket();
	FD_SET(s, &m_oAllSet);
	++m_nSockets;
	m_oSessions[s] = pSession;
}

VOID SelectIO::AddNewConnection() {
	CSocket *pClient = m_pListenSocket->Accept();
	if (NULL == pClient) {
		return;
	}

	pClient->SetNonBlocking(TRUE);
	FD_SET(pClient->GetSocket(), &m_oAllSet);
	Session *pSession = new Session(pClient);
	m_oSessions[pClient->GetSocket()] = pSession;
}

VOID SelectIO::DelConnection(Session *pSession) {
	SOCKET s = pSession->GetCSocket()->GetSocket();
	FD_CLR(s, &m_oAllSet);

	m_oSessions.erase(s);

	SAFE_DELETE(pSession);

	--m_nSockets;
}