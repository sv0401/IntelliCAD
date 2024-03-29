/*
*	Copyright (C) 2019 APIless team. All right reserved.
*
*	파일명			: EventBroadcaster.h
*	작성자			: 이세인
*	최종 수정일		: 19.04.06
*
*	이벤트 전파용 클래스
*/

#pragma once

#include <set>
#include <memory>
#include "ClientAcceptingListener.h"
#include "SystemInitListener.h"
#include "GenericListener.h"
#include "ConnectionCheckListener.h"
#include "ConnectionClosedListener.h"

/// <summary>
/// <para>응용 전역에서 발생하는 이벤트의 제어를 담당하는 클래스이다.</para>
/// <para>주요 기능은 이벤트 리스너 등록 및 해제, 그리고 이벤트 발생이다.</para>
/// </summary>
class EventBroadcaster
{
private:
	friend class System;

	/// <summary>
	/// 등록되어 있는 SystemListener 객체 set
	/// </summary>
	std::set<SystemInitListener *> __systemListeners;

	/// <summary>
	/// 등록되어 있는 ClientAcceptingListener 객체 set
	/// </summary>
	std::set<ClientAcceptingListener *> __clientAcceptingListeners;

	/// <summary>
	/// 등록되어 있는 GenericListener 객체 set
	/// </summary>
	std::set<GenericListener *> __genericListeners;

	/// <summary>
	/// 등록되어 있는 ConnectionCheckListener 객체 set
	/// </summary>
	std::set<ConnectionCheckListener *> __connectionCheckListeners;

	std::set<ConnectionClosedListener *> __connectionClosedListeners;

	/// <summary>
	/// <para>SystemListener를 새로 등록한다. 객체가 정상적으로 등록된 경우 true를 반환한다.</para>
	/// <para>현재 객체가 이미 등록되어 있어 있거나, 여러 이유로 인해 정상적으로 등록되지 않은 경우 false를 반환한다.</para>
	/// </summary>
	/// <param name="listener">등록할 ServerConnectingListener 객체</param>
	/// <returns>객체 등록 성공 여부</returns>
	bool __addSystemInitListener(SystemInitListener &listener);

	/// <summary>
	/// <para>등록되어 있는 SystemListener를 등록 해제한다. 객체가 정상적으로 등록 해제된 경우 true를 반환한다.</para>
	/// <para>현재 객체가 등록되어 있지 않거나, 여러 이유로 인해 정상적으로 해제되지 않은 경우 false를 반환한다.</para>
	/// </summary>
	/// <param name="listener">등록 해제할 SystemListener 객체</param>
	/// <returns>객체 등록 해제 성공 여부</returns>
	bool __removeSystemListener(SystemInitListener &listener);

	/// <summary>
	/// 현재 등록되어 있는 모든 SystemListener 객체에게 SystemInit 이벤트를 브로드캐스팅 한다.
	/// </summary>
	void __notifySystemInit() const;

public:
	
	/// <summary>
	/// <para>ClientAcceptingListener를 새로 등록한다. 객체가 정상적으로 등록된 경우 true를 반환한다.</para>
	/// <para>현재 객체가 이미 등록되어 있어 있거나, 여러 이유로 인해 정상적으로 등록되지 않은 경우 false를 반환한다.</para>
	/// </summary>
	/// <param name="listener">등록할 ClientAcceptingListener 객체</param>
	/// <returns>객체 등록 성공 여부</returns>
	bool addClientAcceptingListener(ClientAcceptingListener &listener);

	/// <summary>
	/// <para>GenericListener를 새로 등록한다. 객체가 정상적으로 등록된 경우 true를 반환한다.</para>
	/// <para>현재 객체가 이미 등록되어 있어 있거나, 여러 이유로 인해 정상적으로 등록되지 않은 경우 false를 반환한다.</para>
	/// </summary>
	/// <param name="listener">등록할 GenericListener 객체</param>
	/// <returns>객체 등록 성공 여부</returns>
	bool addGenericListener(GenericListener &listener);

	bool addConnectionCheckListener(ConnectionCheckListener &listener);

	bool addConnectionClosedListener(ConnectionClosedListener &listener);

	/// <summary>
	/// <para>등록되어 있는 ClientAcceptingListener를 등록 해제한다. 객체가 정상적으로 등록 해제된 경우 true를 반환한다.</para>
	/// <para>현재 객체가 등록되어 있지 않거나, 여러 이유로 인해 정상적으로 해제되지 않은 경우 false를 반환한다.</para>
	/// </summary>
	/// <param name="listener">등록 해제할 ClientAcceptingListener 객체</param>
	/// <returns>객체 등록 해제 성공 여부</returns>
	bool removeClientAcceptingListener(ClientAcceptingListener &listener);

	/// <summary>
	/// <para>등록되어 있는 GenericListener를 등록 해제한다. 객체가 정상적으로 등록 해제된 경우 true를 반환한다.</para>
	/// <para>현재 객체가 등록되어 있지 않거나, 여러 이유로 인해 정상적으로 해제되지 않은 경우 false를 반환한다.</para>
	/// </summary>
	/// <param name="listener">등록 해제할 GenericListener 객체</param>
	/// <returns>객체 등록 해제 성공 여부</returns>
	bool removeGenericListener(GenericListener &listener);

	bool removeConnectionCheckListener(ConnectionCheckListener &listener);

	bool removeConnectionClosedListener(ConnectionClosedListener &listener);

	/// <summary>
	/// <para>현재 등록되어 있는 모든 ClientAcceptingListener 객체에게 clientAcceptingResult 이벤트를 브로드캐스팅 한다.</para>
	/// <para>pSocket은 클라이언트와의 연결 이후 얻은 소켓 객체이다.</para>
	/// <para>객체 획득에 실패했음을 명시하기 위해 nullptr를 전달할 수도 있다.</para>
	/// </summary>
	/// <param name="pSocket">클라이언트와 연결 시도 후 획득한 소켓 객체</param>
	void notifyClientAcceptingResult(std::shared_ptr<Socket> pSocket) const;

	/// <summary>
	/// <para>현재 등록되어 있는 모든 GenericListener 객체에게 generic 이벤트를 브로드캐스팅 한다.</para>
	/// <para>generic 이벤트는 특별한 기능과 연관되어 있지 않기 때문에 디버깅 용으로도 사용이 가능하다.</para>
	/// </summary>
	void notifyGeneric() const;

	void notifyConnectionCheck() const;

	void notifyConnectionClosed(std::shared_ptr<Socket> pSocket) const;
};