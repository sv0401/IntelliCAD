/*
*	Copyright (C) 2019 APIless team. All right reserved.
*
*	���ϸ�			: EventBroadcaster.h
*	�ۼ���			: �̼���
*	���� ������		: 19.05.24
*
*	�̺�Ʈ ���Ŀ� Ŭ����
*/

#pragma once

#include <set>
#include <memory>
#include "SystemInitListener.h"
#include "GenericListener.h"
#include "VolumeLoadingListener.h"
#include "ServerConnectingListener.h"
#include "ConnectionCheckListener.h"
#include "ConnectionClosedListener.h"
#include "ChangeSlicingPointListener.h"
#include "RequestScreenUpdateListener.h"
#include "TransferFunctionUpdateListener.h"
#include "ImageProcessorInitializeListener.h"
#include "LoginSuccessListener.h"
#include "SystemDestroyListener.h"
#include "StreamTransmittingListener.h"
#include "StreamTransmissionFinishedListener.h"

/// <summary>
/// <para>���� �������� �߻��ϴ� �̺�Ʈ�� ��� ����ϴ� Ŭ�����̴�.</para>
/// <para>�ֿ� ����� �̺�Ʈ ������ ��� �� ����, �׸��� �̺�Ʈ �߻��̴�.</para>
/// </summary>
class EventBroadcaster
{
private:
	friend class System;

	/// <summary>
	/// ��ϵǾ� �ִ� SystemListener ��ü set
	/// </summary>
	std::set<SystemInitListener *> __systemInitListeners;
	std::set<SystemDestroyListener *> __systemDestroyListeners;

	/// <summary>
	/// ��ϵǾ� �ִ� GenericListener ��ü set
	/// </summary>
	std::set<GenericListener *> __genericListeners;

	std::set<VolumeLoadingListener *> __volumeLoadingListeners;

	/// <summary>
	/// ��ϵǾ� �ִ� ServerConnectingListener ��ü set
	/// </summary>
	std::set<ServerConnectingListener *> __serverConnectingListeners;

	/// <summary>
	/// ��ϵǾ� �ִ� ConnectionCheckListener ��ü set
	/// </summary>
	std::set<ConnectionCheckListener *> __connectionCheckListeners;

	std::set<ConnectionClosedListener *> __connectionClosedListeners;
	std::set<ChangeSlicingPointListener *> __changeSlicingPointListeners;
	std::set<RequestScreenUpdateListener *> __requestScreenUpdateListeners;
	std::set<TransferFunctionUpdateListener *> __transferFunctionUpdateListeners;
	std::set<ImageProcessorInitializeListener *> __imageProcessorInitializeListeners;
	std::set<LoginSuccessListener *> __loginSuccessListeners;

	std::set<StreamTransmittingListener *> __streamTransmittingListeners;
	std::set<StreamTransmissionFinishedListener *> __streamTransmissionFinishedListeners;


	bool __addSystemInitListener(SystemInitListener &listener);
	bool __removeSystemInitListener(SystemInitListener &listener);
	void __notifySystemInit() const;
	void __notifySystemDestroy() const;

public:

	bool addSystemDestroyListener(SystemDestroyListener &listener);

	/// <summary>
	/// <para>GenericListener�� ���� ����Ѵ�. ��ü�� ���������� ��ϵ� ��� true�� ��ȯ�Ѵ�.</para>
	/// <para>���� ��ü�� �̹� ��ϵǾ� �־� �ְų�, ���� ������ ���� ���������� ��ϵ��� ���� ��� false�� ��ȯ�Ѵ�.</para>
	/// </summary>
	/// <param name="listener">����� GenericListener ��ü</param>
	/// <returns>��ü ��� ���� ����</returns>
	bool addGenericListener(GenericListener &listener);

	bool addVolumeLoadingListener(VolumeLoadingListener &listener);

	/// <summary>
	/// <para>ServerConnectingListener�� ���� ����Ѵ�. ��ü�� ���������� ��ϵ� ��� true�� ��ȯ�Ѵ�.</para>
	/// <para>���� ��ü�� �̹� ��ϵǾ� �־� �ְų�, ���� ������ ���� ���������� ��ϵ��� ���� ��� false�� ��ȯ�Ѵ�.</para>
	/// </summary>
	/// <param name="listener">����� ServerConnectingListener ��ü</param>
	/// <returns>��ü ��� ���� ����</returns>
	bool addServerConnectingListener(ServerConnectingListener &listener);

	bool addConnectionCheckListener(ConnectionCheckListener &listener);
	bool addConnectionClosedListener(ConnectionClosedListener &listener);
	bool addChangeSlicingPointListener(ChangeSlicingPointListener &listener);
	bool addRequestScreenUpdateListener(RequestScreenUpdateListener &listener);
	bool addTransferFunctionUpdateListener(TransferFunctionUpdateListener &listener);
	bool addImageProcessorInitializeListener(ImageProcessorInitializeListener &listener);
	bool addLoginSuccessListener(LoginSuccessListener &listener);

	bool addStreamTransmittingListener(StreamTransmittingListener &listener);
	bool addStreamTransmissionFinishedListener(StreamTransmissionFinishedListener &listener);

	bool removeSystemDestroyListener(SystemDestroyListener &listener);

	/// <summary>
	/// <para>��ϵǾ� �ִ� GenericListener�� ��� �����Ѵ�. ��ü�� ���������� ��� ������ ��� true�� ��ȯ�Ѵ�.</para>
	/// <para>���� ��ü�� ��ϵǾ� ���� �ʰų�, ���� ������ ���� ���������� �������� ���� ��� false�� ��ȯ�Ѵ�.</para>
	/// </summary>
	/// <param name="listener">��� ������ GenericListener ��ü</param>
	/// <returns>��ü ��� ���� ���� ����</returns>
	bool removeGenericListener(GenericListener &listener);

	bool removeVolumeLoadingListener(VolumeLoadingListener &listener);

	/// <summary>
	/// <para>��ϵǾ� �ִ� ServerConnectingListener�� ��� �����Ѵ�. ��ü�� ���������� ��� ������ ��� true�� ��ȯ�Ѵ�.</para>
	/// <para>���� ��ü�� ��ϵǾ� ���� �ʰų�, ���� ������ ���� ���������� �������� ���� ��� false�� ��ȯ�Ѵ�.</para>
	/// </summary>
	/// <param name="listener">��� ������ ServerConnectingListener ��ü</param>
	/// <returns>��ü ��� ���� ���� ����</returns>
	bool removeServerConnectingListener(ServerConnectingListener &listener);

	bool removeConnectionCheckListener(ConnectionCheckListener &listener);
	bool removeConnectionClosedListener(ConnectionClosedListener &listener);
	bool removeChangeSlicingPointListener(ChangeSlicingPointListener &listener);
	bool removeRequestScreenUpdateListener(RequestScreenUpdateListener &listener);
	bool removeTransferFunctionUpdateListener(TransferFunctionUpdateListener &listener);
	bool removeImageProcessorInitializeListener(ImageProcessorInitializeListener &listener);
	bool removeLoginSuccessListener(LoginSuccessListener &listener);

	bool removeStreamTransmittingListener(StreamTransmittingListener &listener);
	bool removeStreamTransmissionFinishedListener(StreamTransmissionFinishedListener &listener);

	/// <summary>
	/// <para>���� ��ϵǾ� �ִ� ��� GenericListener ��ü���� generic �̺�Ʈ�� ��ε�ĳ���� �Ѵ�.</para>
	/// <para>generic �̺�Ʈ�� Ư���� ��ɰ� �����Ǿ� ���� �ʱ� ������ ����� �����ε� ����� �����ϴ�.</para>
	/// </summary>
	void notifyGeneric() const;

	void notifyLoadVolume(const VolumeData &volumeData) const;

	/// <summary>
	/// <para>���� ��ϵǾ� �ִ� ��� ServerConnectingListener ��ü���� ServerConnected �̺�Ʈ�� ��ε�ĳ���� �Ѵ�.</para>
	/// <para>pSocket�� �������� ���� ���� ���� ���� ��ü�̴�.</para>
	/// <para>��ü ȹ�濡 ���������� ����ϱ� ���� nullptr�� ������ ���� �ִ�.</para>
	/// </summary>
	/// <param name="pSocket">������ ����� �� ȹ���� ���� ��ü</param>
	void notifyServerConnected(std::shared_ptr<Socket> pSocket) const;

	void notifyConnectionCheck() const;
	void notifyConnectionClosed(std::shared_ptr<Socket> pSocket) const;
	void notifyChangeSlicingPoint(const Point3D &slicingPoint) const;
	void notifyRequestScreenUpdate(const RenderingScreenType targetType) const;
	void notifyTransferFunctionUpdate(const TransferFunctionType funcType, const double *const pNewTable) const;
	void notifyImageProcessorInitialize() const;
	void notifyLoginSuccess(const Account &account) const;

	void notifyStreamTransmitting(
		std::shared_ptr<const class Socket> who,
		TransmittingDirectionType directionType, const int transmittedSize) const;

	void notifyStreamTransmissionFinished(
		std::shared_ptr<const class Socket> who, TransmittingDirectionType directionType) const;
};