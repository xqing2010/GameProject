#ifndef _PROTOCOL_H_
#define _PROTOCOL_H_

#include "BaseTypeDef.h"
#include "google/protobuf/message.h"

using google::protobuf::Message;

class SocketStream;

enum ProtoSerialErr{
	Err_Write_Overflow = -1,
	Err_Data_InComplete = -10,	//数据不完整 下次再读
	Err_Read_ID = -11,
	Err_SeralizeData = -12,


};

class Protocol {
public:
	Protocol(Message *pMsg, INT id);
	~Protocol();
public:
	INT SerializeFromStream(SocketStream &stream);
	INT SerializeToStream(SocketStream &stream);
	Message *GetMsg();
	INT	 GetID();

protected:
	UINT	 GetSize();
private:
	Message *m_pMsg;
	INT     m_nID;
};

#endif