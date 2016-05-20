#ifndef _MESSAGEID_H_
#define _MESSAGEID_H_

enum MsgID {
	Msg_Invalid = -1,
	#include "AutoGen/MessageIDEnum.h"
	Msg_Num,
};

#endif