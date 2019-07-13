/*
*	Copyright (C) 2019 APIless team. All right reserved.
*
*	파일명			: EventBroadcaster.h
*	작성자			: 이세인
*	최종 수정일		: 19.05.24
*
*	이벤트 전파용 클래스
*/

#include "EventBroadcaster.h"
#include "MacroTransaction.h"

using namespace std;

bool EventBroadcaster::__addSystemInitListener(SystemInitListener &listener)
{
	return __systemInitListeners.emplace(&listener).second;
}

bool EventBroadcaster::__removeSystemInitListener(SystemInitListener &listener)
{
	return __systemInitListeners.erase(&listener);
}

void EventBroadcaster::__notifySystemInit() const
{
	for (SystemInitListener *const pListener : __systemInitListeners)
		pListener->onSystemInit();
}

void EventBroadcaster::__notifySystemDestroy() const
{
	for (SystemDestroyListener *const pListener : __systemDestroyListeners)
		pListener->onSystemDestroy();
}

bool EventBroadcaster::addSystemDestroyListener(SystemDestroyListener &listener)
{
	return __systemDestroyListeners.emplace(&listener).second;
}

bool EventBroadcaster::addGenericListener(GenericListener &listener)
{
	return __genericListeners.emplace(&listener).second;
}

bool EventBroadcaster::addVolumeLoadingListener(VolumeLoadingListener &listener)
{
	return __volumeLoadingListeners.emplace(&listener).second;
}

bool EventBroadcaster::addServerConnectingListener(ServerConnectingListener &listener)
{
	return __serverConnectingListeners.emplace(&listener).second;
}

bool EventBroadcaster::addConnectionCheckListener(ConnectionCheckListener &listener)
{
	return __connectionCheckListeners.emplace(&listener).second;
}

bool EventBroadcaster::addConnectionClosedListener(ConnectionClosedListener &listener)
{
	return __connectionClosedListeners.emplace(&listener).second;
}

bool EventBroadcaster::addChangeSlicingPointListener(ChangeSlicingPointListener &listener)
{
	return __changeSlicingPointListeners.emplace(&listener).second;
}

bool EventBroadcaster::addRequestScreenUpdateListener(RequestScreenUpdateListener &listener)
{
	return __requestScreenUpdateListeners.emplace(&listener).second;
}

bool EventBroadcaster::addTransferFunctionUpdateListener(TransferFunctionUpdateListener &listener)
{
	return __transferFunctionUpdateListeners.emplace(&listener).second;
}

bool EventBroadcaster::addImageProcessorInitializeListener(ImageProcessorInitializeListener &listener)
{
	return __imageProcessorInitializeListeners.emplace(&listener).second;
}

bool EventBroadcaster::addLoginSuccessListener(LoginSuccessListener &listener)
{
	return __loginSuccessListeners.emplace(&listener).second;
}

bool EventBroadcaster::addStreamTransmittingListener(StreamTransmittingListener &listener)
{
	return __streamTransmittingListeners.emplace(&listener).second;
}

bool EventBroadcaster::addStreamTransmissionFinishedListener(StreamTransmissionFinishedListener &listener)
{
	return __streamTransmissionFinishedListeners.emplace(&listener).second;
}

bool EventBroadcaster::removeSystemDestroyListener(SystemDestroyListener &listener)
{
	return __systemDestroyListeners.erase(&listener);
}

bool EventBroadcaster::removeGenericListener(GenericListener &listener)
{
	return __genericListeners.erase(&listener);
}

bool EventBroadcaster::removeVolumeLoadingListener(VolumeLoadingListener &listener)
{
	return __volumeLoadingListeners.erase(&listener);
}

bool EventBroadcaster::removeServerConnectingListener(ServerConnectingListener &listener)
{
	return __serverConnectingListeners.erase(&listener);
}

bool EventBroadcaster::removeConnectionCheckListener(ConnectionCheckListener &listener)
{
	return __connectionCheckListeners.erase(&listener);
}

bool EventBroadcaster::removeConnectionClosedListener(ConnectionClosedListener &listener)
{
	return __connectionClosedListeners.erase(&listener);
}

bool EventBroadcaster::removeChangeSlicingPointListener(ChangeSlicingPointListener &listener)
{
	return __changeSlicingPointListeners.erase(&listener);
}

bool EventBroadcaster::removeRequestScreenUpdateListener(RequestScreenUpdateListener &listener)
{
	return __requestScreenUpdateListeners.erase(&listener);
}

bool EventBroadcaster::removeTransferFunctionUpdateListener(TransferFunctionUpdateListener &listener)
{
	return __transferFunctionUpdateListeners.erase(&listener);
}

bool EventBroadcaster::removeImageProcessorInitializeListener(ImageProcessorInitializeListener &listener)
{
	return __imageProcessorInitializeListeners.erase(&listener);
}

bool EventBroadcaster::removeLoginSuccessListener(LoginSuccessListener &listener)
{
	return __loginSuccessListeners.erase(&listener);
}

bool EventBroadcaster::removeStreamTransmittingListener(StreamTransmittingListener &listener)
{
	return __streamTransmittingListeners.erase(&listener);
}

bool EventBroadcaster::removeStreamTransmissionFinishedListener(StreamTransmissionFinishedListener &listener)
{
	return __streamTransmissionFinishedListeners.erase(&listener);
}

void EventBroadcaster::notifyGeneric() const
{
	for (GenericListener *const pListener : __genericListeners)
		pListener->onGeneric();
}

void EventBroadcaster::notifyLoadVolume(const VolumeData &volumeData) const
{
	for (VolumeLoadingListener *const pListener : __volumeLoadingListeners)
		pListener->onLoadVolume(volumeData);
}

void EventBroadcaster::notifyServerConnected(shared_ptr<Socket> pSocket) const
{
	for (ServerConnectingListener *const pListener : __serverConnectingListeners)
		pListener->onServerConnectionResult(pSocket);
}

void EventBroadcaster::notifyConnectionCheck() const
{
	for (ConnectionCheckListener *const pListener : __connectionCheckListeners)
		pListener->onConnectionCheck();
}

void EventBroadcaster::notifyConnectionClosed(shared_ptr<Socket> pSocket) const
{
	for (ConnectionClosedListener *const pListener : __connectionClosedListeners)
		pListener->onConnectionClosed(pSocket);
}

void EventBroadcaster::notifyChangeSlicingPoint(const Point3D &slicingPoint) const
{
	for (ChangeSlicingPointListener *const pListener : __changeSlicingPointListeners)
		pListener->onChangeSlicingPoint(slicingPoint);
}

void EventBroadcaster::notifyRequestScreenUpdate(const RenderingScreenType targetType) const
{
	for (RequestScreenUpdateListener *const pListener : __requestScreenUpdateListeners)
		pListener->onRequestScreenUpdate(targetType);
}

void EventBroadcaster::notifyTransferFunctionUpdate(
	const TransferFunctionType funcType, const double *const pNewTable) const
{
	for (TransferFunctionUpdateListener *const pListener : __transferFunctionUpdateListeners)
		pListener->onTransferFunctionUpdate(funcType, pNewTable);
}

void EventBroadcaster::notifyImageProcessorInitialize() const
{
	for (ImageProcessorInitializeListener *const pListener : __imageProcessorInitializeListeners)
		pListener->onImageProcessorInitialize();
}

void EventBroadcaster::notifyLoginSuccess(const Account &account) const
{
	for (LoginSuccessListener *const pListener : __loginSuccessListeners)
		pListener->onLoginSuccess(account);
}

void EventBroadcaster::notifyStreamTransmitting(
	shared_ptr<const Socket> who, TransmittingDirectionType directionType, const int transmittedSize) const
{
	for (StreamTransmittingListener *const pListener : __streamTransmittingListeners)
		pListener->onStreamTransmitting(who, directionType, transmittedSize);
}

void EventBroadcaster::notifyStreamTransmissionFinished(
	shared_ptr<const Socket> who, TransmittingDirectionType directionType) const
{
	for (StreamTransmissionFinishedListener *const pListener : __streamTransmissionFinishedListeners)
		pListener->onStreamTransmissionFinished(who, directionType);
}