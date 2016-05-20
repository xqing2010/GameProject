// Server.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include <iostream>
#include <memory>
#include <iostream>
#include <vector>
using namespace std;

#include "SelectIO.h"

#include "Singleton.h"
#include "FixLenBuffPool.h"

int _tmain(int argc, _TCHAR* argv[])
{

	CSocket *pSocket = new CSocket();
	if (false == pSocket->Create()) {
		cout << "create socket error : " << pSocket->GetSockErrorMsg() << endl;
		return 0;
	}

	if (false == pSocket->Bind(9999)) {
		cout << "Bind error : " << pSocket->GetSockErrorMsg() << endl;
		return 0;
	}

	if (false == pSocket->Listen(5)) {
		cout << "Listen error : " << pSocket->GetSockErrorMsg() << endl;
		return 0;
	}

	pSocket->SetNonBlocking(TRUE);

	NetIO *pNetIO = new SelectIO();
	pNetIO->AddListenSocket(pSocket);

	StreamBuff::GetInstance()->Alloc();
	//for (int index = 0; index < BLOCK_NUM; ++index)
	//{
	//	cout << hex << reinterpret_cast<void *>(test.Alloc()) << endl;
	//}


	while (true) {
		Sleep(10);
		pNetIO->Tick();
	}
	
	getchar();
	return 0;
}

