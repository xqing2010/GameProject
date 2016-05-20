#include "SocketStream.h"
#include "NetDefine.h"

SocketStream::SocketStream( CSocket* sock, UINT BufferLen, UINT MaxBufferLen ) 
{
	m_pSocket = sock ;
	m_BufferLen = BufferLen ;
	m_MaxBufferLen = MaxBufferLen ;
		
	m_Head = 0 ;
	m_Tail = 0 ;
	
	m_Buffer = new CHAR[ m_BufferLen ];
	memset( m_Buffer, 0, m_BufferLen ) ;
	m_ReadRollPos = 0;
	m_WriteRollPos = 0;
}

SocketStream::~SocketStream( ) 
{
	SAFE_DELETE_ARRAY(m_Buffer);
}

UINT SocketStream::Length( )const
{
	if( m_Head <= m_Tail )
		return m_Tail - m_Head;
	
	else //if( m_Head > m_Tail ) 
		return m_BufferLen - m_Head + m_Tail ;
}

//返回0表示没有读到数据
UINT SocketStream::Read( CHAR* buf, UINT len ) 
{		
	if ( len > Length() )//only part data need wait to recieve all
		return 0 ;
	
	if ( m_Head < m_Tail ) 
	{
		memcpy( buf, &m_Buffer[m_Head], len ) ;
	} 
	else 
	{
		UINT rightLen = m_BufferLen - m_Head ;
		if( len <= rightLen ) 
		{
			memcpy( buf, &m_Buffer[m_Head], len ) ;
		} 
		else 
		{
			memcpy( buf, &m_Buffer[m_Head], rightLen ) ;
			memcpy( &buf[rightLen], m_Buffer, len - rightLen ) ;
		}
	}

	m_Head = (m_Head + len)%m_BufferLen ;
	
	return len ;
}


BOOL SocketStream::Peek( CHAR* buf, UINT len ) 
{			
	if( len == 0 )
		return FALSE ;
	
	if( len > Length() )
		return FALSE ;

	if( m_Head < m_Tail ) 
	{
		memcpy( buf , &m_Buffer[m_Head] , len );

	} 
	else 
	{
		UINT rightLen = m_BufferLen - m_Head ;
		if( len <= rightLen ) 
		{
			memcpy( &buf[0], &m_Buffer[m_Head], len ) ;
		} 
		else 
		{
			memcpy( &buf[0], &m_Buffer[m_Head], rightLen ) ;
			memcpy( &buf[rightLen], &m_Buffer[0], len-rightLen ) ;
		}
	}
		
	return TRUE ;
}

BOOL SocketStream::Skip( UINT len ) 
{
	if( len == 0 )
		return FALSE ;

	if( len>Length( ) )
		return FALSE ;
	
	m_Head = (m_Head+len)%m_BufferLen ;

	return TRUE ;
}

VOID SocketStream::Initsize( )
{
	m_Head = 0 ;
	m_Tail = 0 ;

	SAFE_DELETE_ARRAY( m_Buffer ) ;
		
	m_Buffer = new CHAR[DEFAULTSOCKETBUFFERSIZE] ;
//	Assert(m_Buffer) ;
		
	m_BufferLen = DEFAULTSOCKETBUFFERSIZE ;
		
	memset( m_Buffer, 0, m_BufferLen ) ;
}

INT CheckReceiveError(INT nReceived)
{
	if (nReceived == SOCKET_ERROR_WOULDBLOCK) return 0;
	if (nReceived == SOCKET_ERROR) return SOCKET_ERROR;
	if (nReceived == 0) return SOCKET_RECV_ZERO;
	return nReceived;

}
	
INT SocketStream::Fill( ) 
{
	INT nFilled = 0 ;
	INT nReceived = 0 ;
	UINT nFree = 0 ;
	UINT available = m_pSocket->GetAvaliable();

	UINT curLen = Length();

	if(curLen + available > m_MaxBufferLen)
	{
		return SOCKET_RECVBUFF_OVERFLOW;
	}

	if (curLen + available > Capacity())
	{
		BOOL ret = Resize((curLen + available) - Capacity());
		if(!ret)
		{
			return SOCKET_BUFF_RESIZE_FAILED;
		}
	}

	if (m_Head <= m_Tail)
	{
		nFree = Capacity() - curLen;
		if (nFree >= available)
		{
			nReceived = m_pSocket->Recv(&m_Buffer[m_Tail], available);
			UINT errorId = CheckReceiveError(nReceived);
			if (errorId <= 0) return errorId;
			nFilled += nReceived;
		}
		else
		{
			nReceived = m_pSocket->Recv(&m_Buffer[m_Tail], nFree);
			UINT errorId = CheckReceiveError(nReceived);
			if (errorId <= 0) return errorId;
			nFilled += nReceived;

			nReceived = m_pSocket->Recv(&m_Buffer[0], available - nFree);
			errorId = CheckReceiveError(nReceived);
			if (errorId <= 0) return errorId;
			nFilled += nReceived;
		}
	}
	else
	{
		nReceived = m_pSocket->Recv(&m_Buffer[m_Tail], available);
		UINT errorId = CheckReceiveError(nReceived);
		if (errorId <= 0) return errorId;
		nFilled += nReceived;
	}
#pragma region DEL
#if 0
	if (m_Head <= m_Tail)
	{
		if (m_Head == 0)
		{
			//
			// H   T		LEN=10
			// 0123456789
			// abcd......
			//

			nReceived = 0;
			nFree = m_BufferLen - m_Tail - 1;
			if (nFree != 0)
			{
				nReceived = m_pSocket->Recv(&m_Buffer[m_Tail], nFree);
				if (nReceived == SOCKET_ERROR_WOULDBLOCK) return 0;
				if (nReceived == SOCKET_ERROR) return SOCKET_ERROR - 1;
				if (nReceived == 0) return SOCKET_ERROR - 2;

				m_Tail += nReceived;
				nFilled += nReceived;
			}

			if (nReceived == nFree)
			{
				UINT available = m_pSocket->GetAvaliable();
				if (available > 0)
				{
					if ((m_BufferLen + available + 1) > m_MaxBufferLen)
					{
						Initsize();
						return SOCKET_ERROR - 3;
					}
					if (!Resize(available + 1))
						return 0;

					nReceived = m_pSocket->Recv(&m_Buffer[m_Tail], available);
					if (nReceived == SOCKET_ERROR_WOULDBLOCK) return 0;
					if (nReceived == SOCKET_ERROR) return SOCKET_ERROR - 4;
					if (nReceived == 0) return SOCKET_ERROR - 5;

					m_Tail += nReceived;
					nFilled += nReceived;
				}
			}
		}
		else
		{
			//
			//    H   T		LEN=10
			// 0123456789
			// ...abcd...
			//

			nFree = m_BufferLen - m_Tail;
			nReceived = m_pSocket->Recv(&m_Buffer[m_Tail], nFree);
			if (nReceived == SOCKET_ERROR_WOULDBLOCK) return 0;
			if (nReceived == SOCKET_ERROR) return SOCKET_ERROR - 6;
			if (nReceived == 0) return SOCKET_ERROR - 7;

			m_Tail = (m_Tail + nReceived) % m_BufferLen;
			nFilled += nReceived;

			if (nReceived == nFree)
			{
				//				Assert( m_Tail == 0 );

				nReceived = 0;
				nFree = m_Head - 1;
				if (nFree != 0)
				{
					nReceived = m_pSocket->Recv(&m_Buffer[0], nFree);
					if (nReceived == SOCKET_ERROR_WOULDBLOCK) return 0;
					if (nReceived == SOCKET_ERROR) return SOCKET_ERROR - 8;
					if (nReceived == 0) return SOCKET_ERROR - 9;

					m_Tail += nReceived;
					nFilled += nReceived;
				}

				if (nReceived == nFree)
				{
					UINT available = m_pSocket->GetAvaliable();
					if (available > 0)
					{
						if ((m_BufferLen + available + 1) > m_MaxBufferLen)
						{
							Initsize();
							return SOCKET_ERROR - 10;
						}
						if (!Resize(available + 1))
							return 0;

						nReceived = m_pSocket->Recv(&m_Buffer[m_Tail], available);
						if (nReceived == SOCKET_ERROR_WOULDBLOCK) return 0;
						if (nReceived == SOCKET_ERROR) return SOCKET_ERROR - 11;
						if (nReceived == 0) return SOCKET_ERROR - 12;

						m_Tail += nReceived;
						nFilled += nReceived;
					}
				}
			}
		}

	}
	else
	{
		//
		//     T  H		LEN=10
		// 0123456789
		// abcd...efg
		//

		nReceived = 0;
		nFree = m_Head - m_Tail - 1;
		if (nFree != 0)
		{
			nReceived = m_pSocket->Recv(&m_Buffer[m_Tail], nFree);
			if (nReceived == SOCKET_ERROR_WOULDBLOCK) return 0;
			if (nReceived == SOCKET_ERROR) return SOCKET_ERROR - 13;
			if (nReceived == 0) return SOCKET_ERROR - 14;

			m_Tail += nReceived;
			nFilled += nReceived;
		}
		if (nReceived == nFree)
		{
			UINT available = m_pSocket->GetAvaliable();
			if (available > 0)
			{
				if ((m_BufferLen + available + 1) > m_MaxBufferLen)
				{
					Initsize();
					return SOCKET_ERROR - 15;
				}
				if (!Resize(available + 1))
					return 0;

				nReceived = m_pSocket->Recv(&m_Buffer[m_Tail], available);
				if (nReceived == SOCKET_ERROR_WOULDBLOCK) return 0;
				if (nReceived == SOCKET_ERROR) return SOCKET_ERROR - 16;
				if (nReceived == 0) return SOCKET_ERROR - 17;

				m_Tail += nReceived;
				nFilled += nReceived;
			}
		}
	}
#endif
#pragma endregion DEL

	return nFilled ;
}


BOOL SocketStream::Resize( UINT size )
{
	size = max(size, (int)(m_BufferLen >> 1));
	UINT newBufferLen = m_BufferLen + size;
	UINT len = Length();
	
	CHAR * newBuffer = new CHAR[ newBufferLen ];
	if (nullptr == newBuffer)
	{
		//OPS TODO memeory overflow
		return FALSE;
	}
//	Assert( newBuffer ) ;
		
	if ( m_Head < m_Tail ) 
	{
		memcpy( newBuffer , &m_Buffer[m_Head] , m_Tail - m_Head );
	} 
	else if ( m_Head > m_Tail ) 
	{
		memcpy( newBuffer , &m_Buffer[m_Head] , m_BufferLen - m_Head );
		memcpy( &newBuffer[ m_BufferLen - m_Head ] , m_Buffer , m_Tail );
	}
		
	delete [] m_Buffer ;
		
	m_Buffer = newBuffer ;
	m_BufferLen = newBufferLen ;
	m_Head = 0 ;
	m_Tail = len ;

	return TRUE ;
}

VOID SocketStream::CleanUp( )
{
	m_Head = 0 ;
	m_Tail = 0 ;
}


UINT SocketStream::Write(const CHAR* buf, UINT len)
{
	UINT nFree = ((m_Head <= m_Tail) ? (m_BufferLen - m_Tail + m_Head - 1) : (m_Head - m_Tail - 1));
	if (len >= nFree)
	{
		if (!Resize(len - nFree + 1))
			return 0;
	}

	if (m_Head <= m_Tail)
	{
		nFree = m_BufferLen - m_Tail;
		if (len <= nFree)
		{
			memcpy(&m_Buffer[m_Tail], buf, len);
		}
		else
		{
			memcpy(&m_Buffer[m_Tail], buf, nFree);
			memcpy(m_Buffer, &buf[nFree], len - nFree);
		}
	}
	else
	{
		memcpy(&m_Buffer[m_Tail], buf, len);
	}
	m_Tail = (m_Tail + len) % m_BufferLen;
	return len;
}

UINT SocketStream::Flush()
{
	UINT nFlushed = 0;
	UINT nSent = 0;
	UINT nLeft;

	if (m_BufferLen > m_MaxBufferLen)
	{//如果单个客户端的缓存太大，则重新设置缓存，并将此客户端断开连接
		Initsize();
		return SOCKET_SENDBUFF_OVERFLOW;
	}

#if defined(WIN32)
	UINT flag = MSG_DONTROUTE; //windows ignore
#elif defined(__LINUX__)
	UINT flag = MSG_NOSIGNAL;
#endif
	if (m_Head < m_Tail)
	{
		nLeft = m_Tail - m_Head;

		while (nLeft > 0)
		{
			nSent = m_pSocket->Send(&m_Buffer[m_Head], nLeft, flag);
			if (nSent == SOCKET_ERROR_WOULDBLOCK) return 0;
			if (nSent == SOCKET_ERROR) return SOCKET_SEND_ERROR_HLT;
			if (nSent == 0) return 0;

			nFlushed += nSent;
			nLeft -= nSent;
			m_Head += nSent;
		}
	}
	else if (m_Head > m_Tail)
	{
		nLeft = m_BufferLen - m_Head;

		while (nLeft > 0)
		{
			nSent = m_pSocket->Send(&m_Buffer[m_Head], nLeft, flag);
			if (nSent == SOCKET_ERROR_WOULDBLOCK) return 0;
			if (nSent == SOCKET_ERROR) return SOCKET_SEND_ERROR_HGT;
			if (nSent == 0) return 0;
			nFlushed += nSent;
			nLeft -= nSent;
			m_Head += nSent;
		}
		m_Head = 0;

		nLeft = m_Tail;

		while (nLeft > 0)
		{
			nSent = m_pSocket->Send(&m_Buffer[m_Head], nLeft, flag);
			if (nSent == SOCKET_ERROR_WOULDBLOCK) return 0;
			if (nSent == SOCKET_ERROR) return SOCKET_SEND_ERROR_HGT;
			if (nSent == 0) return 0;

			nFlushed += nSent;
			nLeft -= nSent;
			m_Head += nSent;
		}
	}

	m_Head = m_Tail = 0;
	return nFlushed;
}


UINT SocketStream::Write(INT data) {
	INT value = htonl(data);
	return Write((CHAR *)&value, sizeof(INT));
}

UINT SocketStream::Read(INT &date) {
	char szBuff[MAX_PATH] = { 0 };
	if (0 == Read(szBuff, sizeof(INT))) {
		return 0;
	}
	INT value = *(INT *)szBuff;
	date = ntohl(value);
	return sizeof(INT);
}