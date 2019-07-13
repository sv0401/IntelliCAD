/*
*	Copyright (C) 2019 APIless team. All right reserved.
*
*	파일명			: ServerNetwork.h
*	작성자			: 오수백
*	최종 수정일		: 19.04.09
*
*	클라이언트와 통신하기 위한 서버 네트워크 최상위 모듈
*/

#pragma once

#include <WinSock2.h>
#include <set>
#include "ServerSocket.h"
#include "SystemInitListener.h"
#include "GenericListener.h"
#include "ClientAcceptingListener.h"
#include "ConnectionClosedListener.h"
#include "ConnectionCheckListener.h"

/// <summary>
/// <para>클라이언트와 통신하기 위한 서버 네트워크 최상위 모듈.</para>
/// <para>싱글톤 패턴이 적용되어 있으며, 유일 인스턴스는 <c>ServerSocket::getInstance()</c> 함수를 통해 얻는다.</para>
/// <para>사용자 단에서 별도의 IP 설정은 필요하지 않으며, 서버 구동은 항상 현재 PC의 IP를 기준으로 한다.</para>
/// </summary>
class ServerNetwork : public SystemInitListener, public ClientAcceptingListener, public ConnectionCheckListener, public ConnectionClosedListener
{
private:

	WSADATA __wsaData;

	/// <summary>
	/// <para><c>ServerNetwork</c> 클래스의 유일 인스턴스이다.</para>
	/// <para>사용자는 임의로 <c>ServerNetwork</c> 클래스를 생성하거나 삭제할 수 없으며,
	/// 네트워크를 이용하기 위해선 반드시 이 객체를 이용하여야 한다.</para>
	/// <para>이 객체의 레퍼런스를 얻기 위해선 <c>ServerSocket::getInstance()</c> 함수를 사용한다.</para>
	/// </summary>
	static ServerNetwork __instance;

	/// <summary>
	/// 서버 소켓을 생성하기 위한 포트 번호
	/// </summary>
	std::tstring __port = _T("9000");

	/// <summary>
	/// <para>서버 소켓 객체.</para>
	/// <para><c>ServerSocket::createServerSocket()</c> 함수를 사용하여 객체를 생성한다.</para>
	/// <para>서버 소켓은 <c>ServerNetwork</c> 인스턴스당 최대 1개 밖에 소유할 수 없다.</para>
	/// <para>서버 소켓이 생성되어 있는지 여부는 <c>ServerSocket::isCreated()</c> 함수를 통해 알 수 있다.</para>
	/// </summary>
	std::unique_ptr<ServerSocket> __pServerSocket = nullptr;

	/// <summary>
	/// 현재 접속 중인 클라이언트 set
	/// </summary>
	std::set<std::shared_ptr<Socket>> __clients;

	/// <summary>
	/// accept 루프가 수행 중인지 여부
	/// </summary>
	bool __runningFlag = false;

	/// <summary>
	/// <para>유일 객체를 생성하기 위한 내부용 기본 생성자.</para>
	/// <para>네트워크 통신과 관련한 초기화 과정이 포함된다.</para>
	/// </summary>
	ServerNetwork();

	/// <summary>
	/// <para>accept 루프를 시작한다.</para>
	/// <para>이 함수는 내부용 함수이며, 서버 소켓이 생성되어 있어야 실행이 가능하다.</para>
	/// </summary>
	void __acceptingLoop();


	ServerNetwork(const ServerNetwork &) = delete;
	ServerNetwork(ServerNetwork &&) = delete;

public:

	/// <summary>
	/// <para>설정되어 있는 포트 번호를 문자열로 반환한다.</para>
	/// <para>서버 소켓이 한번도 생성되지 않았다면 기본값인 9000을 반환한다.</para>
	/// </summary>
	/// <returns>포트 번호</returns>
	const std::tstring& getPort() const;

	/// <summary>
	/// 현재 서버 소켓이 생성되어 있는지 여부를 반환한다.
	/// </summary>
	/// <returns>서버 소켓 생성 여부</returns>
	bool isCreated() const;

	/// <summary>
	/// <para>현재 서버 소켓이 활성화 되어있는지 여부를 반환한다.</para>
	/// <para>활성화가 되었다는 것은 서버 소켓이 생성된 후 클라이언트 accept 루프를 시작하였다는 의미이다.</para>
	/// </summary>
	/// <returns>서버 활성화 여부</returns>
	bool isRunning();

	/// <summary>
	/// <para>PC의 기본 IP와 설정된 포트 번호를 기반으로 소켓을 생성한다.</para>
	/// <para>서버 소켓은 최대 1개 밖에 소유할 수 없다.</para>
	/// <para>서버 소켓이 생성되어 있는지 여부는 <c>ServerSocket::isCreated()</c> 함수를 통해 알 수 있다.</para>
	/// <para>서버 소켓을 성공적으로 생성하였다면 true를, 포트 번호 중복이나 인터넷 문제 등 기타 상황으로 인해 생성이 불가능한 경우 false를 반환한다.</para>
	/// </summary>
	/// <returns>서버 소켓을 성공적으로 생성하였다면 true, 포트 번호 중복이나 인터넷 문제 등 기타 상황으로 인해 생성이 불가능한 경우 false</returns>
	bool createServerSocket(const std::tstring &port);

	virtual void onSystemInit() override;
	virtual void onClientAcceptingResult(std::shared_ptr<Socket> pSocket);

	// 세인 추가
	virtual void onConnectionCheck() override;

	virtual void onConnectionClosed(std::shared_ptr<Socket> pSocket) override;

	/// <summary>
	/// <para>서버의 활성화를 시작한다.</para>
	/// <para>활성화는 서버 소켓이 생성된 후 클라이언트 accept 루프를 시작한다는 의미이다.</para>
	/// <para>이미 활성화가 되어 있는 경우, 본 함수는 아무런 기능을 수행하지 않으며 false를 반환한다.</para>
	/// <para>활성화 성공시 true를, 이미 활성화 되어 있거나 기타 알 수 없는 이유로 인하여 함수를 활성화를 할 수 없다면 false를 반환한다.</para>
	/// </summary>
	/// <returns>활성화 성공시 true, 이미 활성화 되어 있거나 기타 알 수 없는 이유로 인하여 함수를 활성화를 할 수 없다면 false</returns>
	bool run();

	/// <summary>
	/// <para>서버 소켓을 삭제한다.</para>
	/// <para>accept 루프 작업이 종료되며, 현재 접속 중인 모든 클라이언트와의 연결도 끊어지게 된다.</para>
	/// <para>이후 서버를 다시 구동하기 위해선 <c>ServerNetwork::createServerSocket()</c> 함수를 통해 서버 소켓을 다시 생성하여야 한다.</para>
	/// <para>소켓 삭제 성공 시 true를, 서버 소켓 객체가 존재하지 않거나 기타 알 수 없는 이유로 삭제 할 수 없다면 false를 반환한다.</para>
	/// </summary>
	/// <returns>소켓 삭제 성공 시 true, 서버 소켓 객체가 존재하지 않거나 기타 알 수 없는 이유로 삭제 할 수 없다면 false</returns>
	bool releaseServerSocket();

	/// <summary>
	/// ServerNetwork의 유일 객체를 반환한다.
	/// </summary>
	/// <returns>ServerNetwork 객체</returns>
	static ServerNetwork& getInstance();

	/// <summary>
	/// <para>소멸자이다.</para>
	/// <para>네트워크 통신과 관련한 릴리즈 과정이 포함된다.</para>
	/// </summary>
	~ServerNetwork();
};