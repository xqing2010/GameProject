#include "ProtocolHandler.h"


ProtocolHandler::ProtocolHandler() {
#include "AutoGen/MsgHandleReg.h"
}

ProtocolHandler::~ProtocolHandler() {
	for (INT index = 0; index < MsgID::Msg_Num; ++index) {
		SAFE_DELETE(m_oHandlers[index]);
	}
}

bool CSLoginHandler::Handler(Session &session, Message *msg) {
	return true;
}

bool SCLoginRetHandler::Handler(Session &session, Message *msg) {
	return true;
}

#ifdef __SERVER__

#else 

#endif