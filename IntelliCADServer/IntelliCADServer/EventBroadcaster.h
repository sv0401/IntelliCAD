/*
*	Copyright (C) 2019 APIless team. All right reserved.
*
*	���ϸ�			: EventBroadcaster.h
*	�ۼ���			: �̼���
*	���� ������		: 19.04.06
*
*	�̺�Ʈ ���Ŀ� Ŭ����
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
	std::set<SystemInitListener *> __systemListeners;

	/// <summary>
	/// ��ϵǾ� �ִ� ClientAcceptingListener ��ü set
	/// </summary>
	std::set<ClientAcceptingListener *> __clientAcceptingListeners;

	/// <summary>
	/// ��ϵǾ� �ִ� GenericListener ��ü set
	/// </summary>
	std::set<GenericListener *> __genericListeners;

	/// <summary>
	/// ��ϵǾ� �ִ� ConnectionCheckListener ��ü set
	/// </summary>
	std::set<ConnectionCheckListener *> __connectionCheckListeners;

	std::set<ConnectionClosedListener *> __connectionClosedListeners;

	/// <summary>
	/// <para>SystemListener�� ���� ����Ѵ�. ��ü�� ���������� ��ϵ� ��� true�� ��ȯ�Ѵ�.</para>
	/// <para>���� ��ü�� �̹� ��ϵǾ� �־� �ְų�, ���� ������ ���� ���������� ��ϵ��� ���� ��� false�� ��ȯ�Ѵ�.</para>
	/// </summary>
	/// <param name="listener">����� ServerConnectingListener ��ü</param>
	/// <returns>��ü ��� ���� ����</returns>
	bool __addSystemInitListener(SystemInitListener &listener);

	/// <summary>
	/// <para>��ϵǾ� �ִ� SystemListener�� ��� �����Ѵ�. ��ü�� ���������� ��� ������ ��� true�� ��ȯ�Ѵ�.</para>
	/// <para>���� ��ü�� ��ϵǾ� ���� �ʰų�, ���� ������ ���� ���������� �������� ���� ��� false�� ��ȯ�Ѵ�.</para>
	/// </summary>
	/// <param name="listener">��� ������ SystemListener ��ü</param>
	/// <returns>��ü ��� ���� ���� ����</returns>
	bool __removeSystemListener(SystemInitListener &listener);

	/// <summary>
	/// ���� ��ϵǾ� �ִ� ��� SystemListener ��ü���� SystemInit �̺�Ʈ�� ��ε�ĳ���� �Ѵ�.
	/// </summary>
	void __notifySystemInit() const;

public:
	
	/// <summary>
	/// <para>ClientAcceptingListener�� ���� ����Ѵ�. ��ü�� ���������� ��ϵ� ��� true�� ��ȯ�Ѵ�.</para>
	/// <para>���� ��ü�� �̹� ��ϵǾ� �־� �ְų�, ���� ������ ���� ���������� ��ϵ��� ���� ��� false�� ��ȯ�Ѵ�.</para>
	/// </summary>
	/// <param name="listener">����� ClientAcceptingListener ��ü</param>
	/// <returns>��ü ��� ���� ����</returns>
	bool addClientAcceptingListener(ClientAcceptingListener &listener);

	/// <summary>
	/// <para>GenericListener�� ���� ����Ѵ�. ��ü�� ���������� ��ϵ� ��� true�� ��ȯ�Ѵ�.</para>
	/// <para>���� ��ü�� �̹� ��ϵǾ� �־� �ְų�, ���� ������ ���� ���������� ��ϵ��� ���� ��� false�� ��ȯ�Ѵ�.</para>
	/// </summary>
	/// <param name="listener">����� GenericListener ��ü</param>
	/// <returns>��ü ��� ���� ����</returns>
	bool addGenericListener(GenericListener &listener);

	bool addConnectionCheckListener(ConnectionCheckListener &listener);

	bool addConnectionClosedListener(ConnectionClosedListener &listener);

	/// <summary>
	/// <para>��ϵǾ� �ִ� ClientAcceptingListener�� ��� �����Ѵ�. ��ü�� ���������� ��� ������ ��� true�� ��ȯ�Ѵ�.</para>
	/// <para>���� ��ü�� ��ϵǾ� ���� �ʰų�, ���� ������ ���� ���������� �������� ���� ��� false�� ��ȯ�Ѵ�.</para>
	/// </summary>
	/// <param name="listener">��� ������ ClientAcceptingListener ��ü</param>
	/// <returns>��ü ��� ���� ���� ����</returns>
	bool removeClientAcceptingListener(ClientAcceptingListener &listener);

	/// <summary>
	/// <para>��ϵǾ� �ִ� GenericListener�� ��� �����Ѵ�. ��ü�� ���������� ��� ������ ��� true�� ��ȯ�Ѵ�.</para>
	/// <para>���� ��ü�� ��ϵǾ� ���� �ʰų�, ���� ������ ���� ���������� �������� ���� ��� false�� ��ȯ�Ѵ�.</para>
	/// </summary>
	/// <param name="listener">��� ������ GenericListener ��ü</param>
	/// <returns>��ü ��� ���� ���� ����</returns>
	bool removeGenericListener(GenericListener &listener);

	bool removeConnectionCheckListener(ConnectionCheckListener &listener);

	bool removeConnectionClosedListener(ConnectionClosedListener &listener);

	/// <summary>
	/// <para>���� ��ϵǾ� �ִ� ��� ClientAcceptingListener ��ü���� clientAcceptingResult �̺�Ʈ�� ��ε�ĳ���� �Ѵ�.</para>
	/// <para>pSocket�� Ŭ���̾�Ʈ���� ���� ���� ���� ���� ��ü�̴�.</para>
	/// <para>��ü ȹ�濡 ���������� ����ϱ� ���� nullptr�� ������ ���� �ִ�.</para>
	/// </summary>
	/// <param name="pSocket">Ŭ���̾�Ʈ�� ���� �õ� �� ȹ���� ���� ��ü</param>
	void notifyClientAcceptingResult(std::shared_ptr<Socket> pSocket) const;

	/// <summary>
	/// <para>���� ��ϵǾ� �ִ� ��� GenericListener ��ü���� generic �̺�Ʈ�� ��ε�ĳ���� �Ѵ�.</para>
	/// <para>generic �̺�Ʈ�� Ư���� ��ɰ� �����Ǿ� ���� �ʱ� ������ ����� �����ε� ����� �����ϴ�.</para>
	/// </summary>
	void notifyGeneric() const;

	void notifyConnectionCheck() const;

	void notifyConnectionClosed(std::shared_ptr<Socket> pSocket) const;
};