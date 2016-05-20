#include "Session.h"

Session::Session(CSocket *pSocket) {
	m_pSocket = pSocket;
	m_pInputStream = new SocketStream(m_pSocket);
	m_pOutputStream = new SocketStream(m_pSocket);
}

Session::~Session() {
	SAFE_DELETE(m_pSocket);
	SAFE_DELETE(m_pInputStream);
	SAFE_DELETE(m_pOutputStream);
}

INT Session::ProcessInput() {
	return m_pInputStream->Fill();
}

VOID Session::ProcessOutput() {
	m_pOutputStream->Flush();
}

VOID Session::ProcessCommands() {
	//TODO
}

VOID Session::ProcessException() {
	// no oob
}

BOOL Session::Read() {
	return TRUE;
}

BOOL Session::Send() {
	return TRUE;
}