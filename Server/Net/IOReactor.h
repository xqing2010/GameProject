#ifndef _IOREACTOR_H_
#define _IOREACTOR_H_

#include "BaseTypeDef.h"
#include "NetDefine.h"

enum IOEvent
{
	Read = 0x01,
	Write = 0x02,
	OOB = 0x04,
};

struct SocketEventInfo
{
	SOCKET sockId;
	BYTE eventFlag;

};

class IOEventHandler
{
public: 
	virtual void Handle(SocketEventInfo s) {}
//protected:
public:
	virtual void HandleInput(SocketEventInfo s) {}
	virtual void HandleOutput(SocketEventInfo s) {}
	virtual void HandleOutOfBand(SocketEventInfo s){}

	
};

class IOReactor
{
public: 
	IOReactor()
	{
		;
	}
	virtual ~IOReactor()
	{
		;
	}

public:
	void RegisterEventHandler(IOEvent e, )
public: 
};

#endif // IOREACTOR_H_
