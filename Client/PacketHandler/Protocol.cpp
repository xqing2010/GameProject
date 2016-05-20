#include "Protocol.h"
#include "SocketStream.h"
#include "MsgFactory.h"
#include <cassert>

//消息写入：息ID 长度 消息内容
#define PROTOHEADERSIZE sizeof(INT) + sizeof(UINT)

Protocol::Protocol(Message *pMsg, INT id) : m_pMsg(pMsg), m_nID(id) {}

Protocol::~Protocol() {
	SAFE_DELETE(m_pMsg);
}

Message *Protocol::GetMsg() {
	return m_pMsg;
}

INT Protocol::GetID() {
	return m_nID;
}

UINT Protocol::GetSize() {
	if (NULL == m_pMsg) {
		assert(FALSE);
		return 0;
	}
	return m_pMsg->ByteSize();
}

INT Protocol::SerializeFromStream(SocketStream &stream) {
	bool bRead = true;
	stream.StartTransaction(bRead);

	if (0 == stream.Read(m_nID)) { //no data
		return Err_Data_InComplete;
	}

	INT len = 0;
	if (0 == stream.Read(len)) {   //no data
		return Err_Data_InComplete;
	}

	m_pMsg = MsgFactory::GetInstance()->CreateMsg((MsgID)m_nID);
	if (NULL == m_pMsg) {
		stream.RollBack(bRead);
		return Err_Read_ID;
	}

	CHAR *pszBuff = new CHAR[len + 1];
	memset(pszBuff, 0, sizeof(CHAR) * (len + 1));
	if (0 == stream.Read(pszBuff, len)) {
		SAFE_DELETE_ARRAY(pszBuff);
		stream.RollBack(bRead);
		return Err_Data_InComplete;
	}

	if (false == m_pMsg->ParseFromString(pszBuff)) {
		SAFE_DELETE_ARRAY(pszBuff);
		stream.RollBack(bRead);
		return Err_SeralizeData;
	}
	
	SAFE_DELETE_ARRAY(pszBuff);

	stream.Commit(bRead);
	return 1;
Failed:
	stream.RollBack(bRead);
	return -2;
}

INT Protocol::SerializeToStream(SocketStream &stream) {
	if (NULL == m_pMsg ||
		m_nID < 0) {
		return 0;
	}
	bool bRead = false;
	stream.StartTransaction(bRead);
	if (0 == stream.Write(m_nID)) {	//buff overflow 
		goto Buff_OverFlow;
	}

	INT len = GetSize();
	if (0 == stream.Write(len)) {
		goto Buff_OverFlow;
	}

	if (0 == stream.Write(m_pMsg->SerializeAsString().c_str(), len)) {
		goto Buff_OverFlow;
	}

	stream.Commit(bRead);

	return len + PROTOHEADERSIZE;

Buff_OverFlow:
	stream.RollBack(bRead);
	return Err_Write_Overflow;
}

