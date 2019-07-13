/*
*	Copyright (C) 2019 APIless team. All right reserved.
*
*	파일명			: ServerSocket.h
*	작성자			: 오수백
*	최종 수정일		: 19.04.09
*
*	서버 소켓 클래스
*/

#pragma once

#include <WinSock2.h>
#include <memory>
#include "tstring.h"
#include "Socket.h"

/// <summary>
/// <para>클라이언트와 통신하기 위한 서버 소켓 클래스이다.</para>
/// <para>주요 기능은 클라이언트의 접속 요청을 accept 하는 것이다.</para>
/// <para>외부에선 별도의 생성자가 제공되지 않으며, <c>ServerSocket::create()</c>함수를 통해서 인스턴스를 얻는다.</para>
/// </summary>
class ServerSocket
{
private:

	/// <summary>
	/// client accept 용 SOCKET
	/// </summary>
	SOCKET __sock;

	/// <summary>
	/// 서버 소켓 클래스의 내부용 명시적 생성자이다.
	/// </summary>
	/// <param name="hServerSockRaw">raw data</param>
	/// <param name="serverSockAddr">raw data</param>
	/// <param name="maxConnection">수용 가능한 최대 클라이언트 수</param>
	ServerSocket(const SOCKET &hServerSockRaw, const SOCKADDR_IN &serverSockAddr, const int maxConnection);

	ServerSocket(const ServerSocket &) = delete;
	ServerSocket(ServerSocket &&) = delete;

public:
	const int MAX_CONNECTION;

	/// <summary>
	/// 클라이언트 소켓을 받아들이는 accept 작업을 시작한다.
	/// </summary>
	/// <returns>accept가 가능한 상태이면 true, 불가능하면 false</returns>
	std::shared_ptr<Socket> accept();

	/// <summary>
	/// 서버 소켓을 닫는다. accept 작업이 수행 중이라면 이 또한 종료한다.
	/// </summary>
	/// <returns>소켓이 정상적으로 닫혔다면 true, 기타 오류로 인한 실패 시 false</returns>
	bool close();

	/// <summary>
	/// 소멸자이다. 네트워크 통신과 관련한 리소스 릴리즈 과정이 포함된다.
	/// </summary>
	~ServerSocket();

	/// <summary>
	/// <para>주어진 포트를 가지고 서버 소켓을 생성한다.</para>
	/// <para>최대로 접속 가능한 클라이언트의 수를 별도로 명시할 수 있으며, 기본 값은 INT_MAX이다.</para>
	/// <para>서버 소켓을 정상적으로 생성할 수 있다면 객체의 포인터를, 생성 실패 시 nullptr를 반환한다.</para>
	/// </summary>
	/// <param name="port">소켓을 생성할 포트</param>
	/// <param name="maxConnection">최대 접속 가능한 클라이언트 수</param>
	/// <returns>서버 소켓을 정상적으로 생성할 수 있다면 객체의 포인터, 생성 실패 시 nullptr</returns>
	static std::unique_ptr<ServerSocket> create(const std::tstring &port, int maxConnection = INT_MAX);
};