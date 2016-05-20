#ifndef _PROTOCOLHANDLER_H_
#define _PROTOCOLHANDLER_H_

#include "Singleton.h"
#include "Protocol.h"
#include "Session.h"
#include "MessageID.h"

class MessageHandler {
public:
	virtual ~MessageHandler() {};
	virtual bool Handler(Session &session, Message *msg) = 0;
};

class ProtocolHandler : Singleton<ProtocolHandler>{
	friend class Singleton < ProtocolHandler > ;
protected:
	ProtocolHandler();
	~ProtocolHandler();
public:
	bool Handler(Session &session, Protocol &protocol) {
		if (protocol.GetID() <= MsgID::Msg_Invalid ||
			protocol.GetID() >= MsgID::Msg_Num) {
			return false;
		}

		return m_oHandlers[protocol.GetID()]->Handler(session, protocol.GetMsg());
	}
private:
	MessageHandler *m_oHandlers[MsgID::Msg_Num];
};

//auto generate
#include "AutoGen/MsgHandleDef.h"
#endif