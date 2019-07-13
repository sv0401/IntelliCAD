/*
*	Copyright (C) 2019 APIless team. All right reserved.
*
*	파일명			: ServerNetwork.cpp
*	작성자			: 오수백
*	최종 수정일		: 19.04.09
*
*	클라이언트와 통신하기 위한 서버 네트워크 최상위 모듈
*/

#include "stdafx.h"
#include "ServerNetwork.h"
#include "System.h"
#include "MacroTransaction.h"
#include "Debugger.h"


using namespace std;

ServerNetwork ServerNetwork::__instance = ServerNetwork();

ServerNetwork::ServerNetwork()
{
	//socket version 2.2
	WSAStartup(MAKEWORD(2, 2), &__wsaData);
}

void ServerNetwork::__acceptingLoop()
{
	EventBroadcaster &eventBroadcaster = System::getSystemContents().getEventBroadcaster();
	
	__runningFlag = true;
	while (__runningFlag)
	{
		std::shared_ptr<Socket> p = __pServerSocket->accept();
		if (p != nullptr)
			eventBroadcaster.notifyClientAcceptingResult(p);
	}
	__runningFlag = false;
}



const tstring& ServerNetwork::getPort() const
{
	return __port;
}

bool ServerNetwork::isCreated() const
{
	IF_T_RET_F(__pServerSocket == nullptr);

	return true;
}

bool ServerNetwork::isRunning()
{
	return __runningFlag;
}

bool ServerNetwork::createServerSocket(const tstring &port)
{
	IF_T_RET_F(isCreated());

	__pServerSocket = ServerSocket::create(port);

	IF_T_RET_F(!isCreated());

	return true;
}

void ServerNetwork::onSystemInit()
{
	EventBroadcaster &eventBroadcaster = System::getSystemContents().getEventBroadcaster();
	eventBroadcaster.addClientAcceptingListener(*this);
	eventBroadcaster.addConnectionCheckListener(*this);
	eventBroadcaster.addConnectionClosedListener(*this);
}

void ServerNetwork::onClientAcceptingResult(std::shared_ptr<Socket> pSocket)
{
	System::getSystemContents().getTaskManager().
		run(TaskType::CONNECTION_CLOSED, *pSocket, &Socket::__receivingLoop);

	__clients.insert(pSocket);

	System::getInstance().printLog(_T("onClientAcceptingResult"));
}

// 세인 추가
void ServerNetwork::onConnectionCheck()
{
	// 서버 open 이후 활성화되는 connection check 버튼을 클릭하면 ConnectionCheck 이벤트가 발생함
	// 이곳에서 패킷 send / receive를 통해 모든 클라이언트와 연결 체크를 해볼 것
	
	//DummyClass d = DummyClass(1);
	//d.dummy2 = 3.4f;
	//d.dummy3 = 'a';
	//for (int i = 1; i <= 1025; i++) {
	//	d.dummy4.push_back(i * 100);
	//}

	//for (shared_ptr<Socket> client : __clients) {
	//	//client->sendMSG(_T("Hello Client"), ProtocolType::CONNECTION_CHECK);
	//	//client->sendFile(string("c:\\network_test\\dummy.txt"));

	//	client->sendObj(d, ObjectType::DUMMY_CLASS);
	//}
	// 이 함수는 "onConnectionCheck"이라는 메세지 박스를 팝업함
	//Debugger::popMessageBox(_T("onConnectionCheck"));
}

void ServerNetwork::onConnectionClosed(std::shared_ptr<Socket> pSocket)
{
	__clients.erase(pSocket);

	System::getInstance().printLog(_T("onConnectionClosed"));
}

bool ServerNetwork::run()
{
	IF_T_RET_F(!isCreated()); // 서버소켓 미생성 상태 false
	IF_T_RET_F(isRunning()); // 이미 running 중이면 false

	System &system = System::getInstance();
	AsyncTaskManager &taskMgr = system.getSystemContents().getTaskManager();

	taskMgr.run_void(TaskType::GENERIC, __instance, &ServerNetwork::__acceptingLoop);

	return true;
}



bool ServerNetwork::releaseServerSocket()
{
	// running 중이 아니면 false
	IF_T_RET_F(!isRunning()); 

	//accepting loop break
	__runningFlag = false;

	//client array clear
	for (std::shared_ptr<Socket> client : __clients) {
		client->close();
	}

	//serversocket delete
	__pServerSocket->close();
	__pServerSocket = nullptr;

	return true;
}

ServerNetwork& ServerNetwork::getInstance()
{
	return __instance;
}

ServerNetwork::~ServerNetwork()
{
	releaseServerSocket();
	WSACleanup();
}