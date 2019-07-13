/*
*	Copyright (C) 2019 APIless team. All right reserved.
*
*	파일명			: EventBroadcaster.cpp
*	작성자			: 이세인
*	최종 수정일		: 19.04.06
*
*	이벤트 전파용 클래스
*/

#include "stdafx.h"
#include "EventBroadcaster.h"

using namespace std;

bool EventBroadcaster::__addSystemInitListener(SystemInitListener &listener)
{
	return __systemListeners.emplace(&listener).second;
}

bool EventBroadcaster::__removeSystemListener(SystemInitListener &listener)
{
	return __systemListeners.erase(&listener);
}

void EventBroadcaster::__notifySystemInit() const
{
	for (SystemInitListener *const pListener : __systemListeners)
		pListener->onSystemInit();
}

bool EventBroadcaster::addClientAcceptingListener(ClientAcceptingListener &listener)
{
	return __clientAcceptingListeners.emplace(&listener).second;
}

bool EventBroadcaster::addGenericListener(GenericListener &listener)
{
	return __genericListeners.emplace(&listener).second;
}

bool EventBroadcaster::addConnectionCheckListener(ConnectionCheckListener &listener)
{
	return __connectionCheckListeners.emplace(&listener).second;
}

bool EventBroadcaster::addConnectionClosedListener(ConnectionClosedListener & listener)
{
	return __connectionClosedListeners.emplace(&listener).second;
}

bool EventBroadcaster::removeClientAcceptingListener(ClientAcceptingListener &listener)
{
	return __clientAcceptingListeners.erase(&listener);
}

bool EventBroadcaster::removeGenericListener(GenericListener &listener)
{
	return __genericListeners.erase(&listener);
}

bool EventBroadcaster::removeConnectionCheckListener(ConnectionCheckListener &listener)
{
	return __connectionCheckListeners.erase(&listener);
}

bool EventBroadcaster::removeConnectionClosedListener(ConnectionClosedListener & listener)
{
	return __connectionClosedListeners.erase(&listener);
}

void EventBroadcaster::notifyClientAcceptingResult(const shared_ptr<Socket> pSocket) const
{
	for (ClientAcceptingListener *const pListener : __clientAcceptingListeners)
		pListener->onClientAcceptingResult(pSocket);
}

void EventBroadcaster::notifyGeneric() const
{
	for (GenericListener *const pListener : __genericListeners)
		pListener->onGeneric();
}

void EventBroadcaster::notifyConnectionCheck() const
{
	for (ConnectionCheckListener *const pListener : __connectionCheckListeners)
		pListener->onConnectionCheck();
}

void EventBroadcaster::notifyConnectionClosed(std::shared_ptr<Socket> pSocket) const
{
	for (ConnectionClosedListener *const pListener : __connectionClosedListeners)
		pListener->onConnectionClosed(pSocket);
}
