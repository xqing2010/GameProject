#ifndef _SESSION_H_
#define _SESSION_H_

#include "CSocket.h"
#include "SocketStream.h"

class Session {
public:
	Session(CSocket *pSocket);
	virtual ~Session();
	BOOL Send();
	BOOL Read();
	BOOL ProcessInput();
	VOID ProcessOutput();
	VOID ProcessCommands();
	VOID ProcessException();
	const CSocket *GetCSocket() const {
		return m_pSocket;
	}
	SocketStream  *GetInputStream() {
		return m_pOutputStream;
	}
	SocketStream  *GetOutputStream() {
		return m_pOutputStream;
	}
private:
	CSocket *m_pSocket;
	SocketStream *m_pInputStream;
	SocketStream *m_pOutputStream;
};

#endif