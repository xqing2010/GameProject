#include "MsgFactory.h"
#include "BaseTypeDef.h"
#include "PBMessage.pb.h"

MsgFactory::MsgFactory() {
#include "AutoGen/MsgRegister.h"
}

MsgFactory::~MsgFactory() {
	//
	for (INT index = 0; index < MsgID::Msg_Num; ++index) {
		SAFE_DELETE(m_oMsgArray[index]);
	}
}

Message *MsgFactory::CreateMsg(MsgID id) {
	if (id <= MsgID::Msg_Invalid ||
		id >= MsgID::Msg_Num) {
		return NULL;
	}
	return m_oMsgArray[id];
}

