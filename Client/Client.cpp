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

int _tmain(int argc, _TCHAR* argv[])
{

	CSocket *pSocket = new CSocket();
	if (false == pSocket->Create()) {
		cout << "create socket error : " << pSocket->GetSockErrorMsg() << endl;
		return 0;
	}

	if (false == pSocket->Connect("127.0.0.1", 9999)) {
		cout << "Bind error : " << pSocket->GetSockErrorMsg() << endl;
		return 0;
	}

	pSocket->SetNonBlocking(TRUE);
	Session *pSession = new Session(pSocket);

	NetIO *pNetIO = new SelectIO();
	pNetIO->AddSession(pSession);

	while (true) {
		Sleep(10);
		pNetIO->Tick();
	}
	
	getchar();
	return 0;
}

