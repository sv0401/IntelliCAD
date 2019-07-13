/*
*	Copyright (C) 2019 APIless team. All right reserved.
*
*	파일명			: ClientAcceptingListener.h
*	작성자			: 이세인
*	최종 수정일		: 19.03.06
*
*	클라이언트 접속 콜백 인터페이스
*/

#pragma once

#include <memory>
#include "Socket.h"

/// <summary>
/// 서버에 새로운 클라이언트가 접속 되었을 때, 관련 작업을 처리하는 콜백 인터페이스이다.
/// </summary>
class ClientAcceptingListener
{
public:
	/// <summary>
	/// <para>새로운 클라이언트가 접속 되었을 때 콜백되는 함수이다.</para>
	/// <para>클라이언트와 연결된 소켓이 인자로 들어온다.</para>
	/// <para>연결이 제대로 이루어지지 않았다면 nullptr가 넘어온다.</para>
	/// </summary>
	/// <param name="pSocket">클라이언트 소켓. 연결이 제대로 이루어지지 않았을 시 nullptr</param>
	virtual void onClientAcceptingResult(std::shared_ptr<Socket> pSocket) = 0;
};