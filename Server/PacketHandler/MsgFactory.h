#ifndef _MSGFACTORY_H_
#define _MSGFACTORY_H_

#include "Singleton.h"
#include "MessageID.h"
#include "google/protobuf/message.h"

using google::protobuf::Message;

class MsgFactory : public Singleton < MsgFactory > {
	friend class Singleton < MsgFactory > ;
	//construct
protected:
	MsgFactory();
	~MsgFactory();
	//operator
public:
	Message *CreateMsg(MsgID id);
	//data
private:
	Message *m_oMsgArray[MsgID::Msg_Num];
};

#endif