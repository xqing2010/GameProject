//
//�ļ����ƣ�	SocketInputStream.h
//����������	��Ϣ���ݵĽ��ջ��棬�ṩ���ݵĽ��պ͸�ʽ����ȡ����
//				
//
//


#ifndef __SOCKETSTREAM_H__
#define __SOCKETSTREAM_H__

#include "CSocket.h"

//��ʼ���Ļ��泤��
#define DEFAULTSOCKETBUFFERSIZE 64*1024
//����������Ļ��泤�ȣ������������ֵ����Ͽ�����
#define DISCONNECTSOCKETINPUTSIZE 96*1024
#define DISCONNECTSOCKETOUTPUTSIZE 512*1024
class Packet ;
class SocketStream
{
public :
	SocketStream( CSocket* sock, 
		UINT BufferSize = DEFAULTSOCKETBUFFERSIZE,
		UINT MaxBufferSize = DISCONNECTSOCKETINPUTSIZE ) ;
	virtual ~SocketStream( ) ;

public :
	void						StartTransaction(bool read = true) { 
		if (read) {
			m_ReadRollPos = m_Head;
		}
		else {
			m_WriteRollPos = m_Tail;
		}
	}
	void						Commit(bool read = true) { 
		if (read) {
			m_ReadRollPos = -1;
		}
		else {
			m_WriteRollPos = -1;
		}
	}
	void						RollBack(bool read = true) {
		if (read) {
			m_Head = m_ReadRollPos;
		}
		else {
			m_Tail = m_WriteRollPos;
		}
	}
	UINT						Write(const CHAR* buf, UINT len);
	UINT						Read( CHAR* buf, UINT len ) ;
	
	UINT						Write(INT data);
	UINT						Read(INT &date);

	BOOL						Peek( CHAR* buf, UINT len ) ;
	BOOL						Skip( UINT len ) ;

	UINT						Flush();
	UINT						Fill( ) ;

	VOID						Initsize( ) ;
	BOOL						Resize( INT size ) ;
	
	UINT						Capacity( )const { return m_BufferLen; }
	
	UINT						Length( )const ;
	UINT						Size( )const { return Length(); }

	BOOL						IsEmpty( )const { return m_Head==m_Tail; }

	VOID						CleanUp( ) ;

	UINT						GetHead(){ return m_Head; }
	UINT						GetTail(){ return m_Tail; }
	UINT						GetBuffLen(){ return m_BufferLen; }
	CHAR*						GetBuff(){ return m_Buffer; }
	CHAR*						GetBuff()const { return m_Buffer; }
	CHAR*						GetTail()const { return &(m_Buffer[m_Tail]); }
private :
	CSocket*	m_pSocket;	
	CHAR*		m_Buffer;
	
	UINT		m_BufferLen;
	UINT		m_MaxBufferLen;
	
	UINT		m_Head;
	UINT		m_Tail;
	UINT		m_ReadRollPos;
	UINT		m_WriteRollPos;
};
#endif
