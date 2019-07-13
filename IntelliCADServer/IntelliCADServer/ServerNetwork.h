/*
*	Copyright (C) 2019 APIless team. All right reserved.
*
*	���ϸ�			: ServerNetwork.h
*	�ۼ���			: ������
*	���� ������		: 19.04.09
*
*	Ŭ���̾�Ʈ�� ����ϱ� ���� ���� ��Ʈ��ũ �ֻ��� ���
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
/// <para>Ŭ���̾�Ʈ�� ����ϱ� ���� ���� ��Ʈ��ũ �ֻ��� ���.</para>
/// <para>�̱��� ������ ����Ǿ� ������, ���� �ν��Ͻ��� <c>ServerSocket::getInstance()</c> �Լ��� ���� ��´�.</para>
/// <para>����� �ܿ��� ������ IP ������ �ʿ����� ������, ���� ������ �׻� ���� PC�� IP�� �������� �Ѵ�.</para>
/// </summary>
class ServerNetwork : public SystemInitListener, public ClientAcceptingListener, public ConnectionCheckListener, public ConnectionClosedListener
{
private:

	WSADATA __wsaData;

	/// <summary>
	/// <para><c>ServerNetwork</c> Ŭ������ ���� �ν��Ͻ��̴�.</para>
	/// <para>����ڴ� ���Ƿ� <c>ServerNetwork</c> Ŭ������ �����ϰų� ������ �� ������,
	/// ��Ʈ��ũ�� �̿��ϱ� ���ؼ� �ݵ�� �� ��ü�� �̿��Ͽ��� �Ѵ�.</para>
	/// <para>�� ��ü�� ���۷����� ��� ���ؼ� <c>ServerSocket::getInstance()</c> �Լ��� ����Ѵ�.</para>
	/// </summary>
	static ServerNetwork __instance;

	/// <summary>
	/// ���� ������ �����ϱ� ���� ��Ʈ ��ȣ
	/// </summary>
	std::tstring __port = _T("9000");

	/// <summary>
	/// <para>���� ���� ��ü.</para>
	/// <para><c>ServerSocket::createServerSocket()</c> �Լ��� ����Ͽ� ��ü�� �����Ѵ�.</para>
	/// <para>���� ������ <c>ServerNetwork</c> �ν��Ͻ��� �ִ� 1�� �ۿ� ������ �� ����.</para>
	/// <para>���� ������ �����Ǿ� �ִ��� ���δ� <c>ServerSocket::isCreated()</c> �Լ��� ���� �� �� �ִ�.</para>
	/// </summary>
	std::unique_ptr<ServerSocket> __pServerSocket = nullptr;

	/// <summary>
	/// ���� ���� ���� Ŭ���̾�Ʈ set
	/// </summary>
	std::set<std::shared_ptr<Socket>> __clients;

	/// <summary>
	/// accept ������ ���� ������ ����
	/// </summary>
	bool __runningFlag = false;

	/// <summary>
	/// <para>���� ��ü�� �����ϱ� ���� ���ο� �⺻ ������.</para>
	/// <para>��Ʈ��ũ ��Ű� ������ �ʱ�ȭ ������ ���Եȴ�.</para>
	/// </summary>
	ServerNetwork();

	/// <summary>
	/// <para>accept ������ �����Ѵ�.</para>
	/// <para>�� �Լ��� ���ο� �Լ��̸�, ���� ������ �����Ǿ� �־�� ������ �����ϴ�.</para>
	/// </summary>
	void __acceptingLoop();


	ServerNetwork(const ServerNetwork &) = delete;
	ServerNetwork(ServerNetwork &&) = delete;

public:

	/// <summary>
	/// <para>�����Ǿ� �ִ� ��Ʈ ��ȣ�� ���ڿ��� ��ȯ�Ѵ�.</para>
	/// <para>���� ������ �ѹ��� �������� �ʾҴٸ� �⺻���� 9000�� ��ȯ�Ѵ�.</para>
	/// </summary>
	/// <returns>��Ʈ ��ȣ</returns>
	const std::tstring& getPort() const;

	/// <summary>
	/// ���� ���� ������ �����Ǿ� �ִ��� ���θ� ��ȯ�Ѵ�.
	/// </summary>
	/// <returns>���� ���� ���� ����</returns>
	bool isCreated() const;

	/// <summary>
	/// <para>���� ���� ������ Ȱ��ȭ �Ǿ��ִ��� ���θ� ��ȯ�Ѵ�.</para>
	/// <para>Ȱ��ȭ�� �Ǿ��ٴ� ���� ���� ������ ������ �� Ŭ���̾�Ʈ accept ������ �����Ͽ��ٴ� �ǹ��̴�.</para>
	/// </summary>
	/// <returns>���� Ȱ��ȭ ����</returns>
	bool isRunning();

	/// <summary>
	/// <para>PC�� �⺻ IP�� ������ ��Ʈ ��ȣ�� ������� ������ �����Ѵ�.</para>
	/// <para>���� ������ �ִ� 1�� �ۿ� ������ �� ����.</para>
	/// <para>���� ������ �����Ǿ� �ִ��� ���δ� <c>ServerSocket::isCreated()</c> �Լ��� ���� �� �� �ִ�.</para>
	/// <para>���� ������ ���������� �����Ͽ��ٸ� true��, ��Ʈ ��ȣ �ߺ��̳� ���ͳ� ���� �� ��Ÿ ��Ȳ���� ���� ������ �Ұ����� ��� false�� ��ȯ�Ѵ�.</para>
	/// </summary>
	/// <returns>���� ������ ���������� �����Ͽ��ٸ� true, ��Ʈ ��ȣ �ߺ��̳� ���ͳ� ���� �� ��Ÿ ��Ȳ���� ���� ������ �Ұ����� ��� false</returns>
	bool createServerSocket(const std::tstring &port);

	virtual void onSystemInit() override;
	virtual void onClientAcceptingResult(std::shared_ptr<Socket> pSocket);

	// ���� �߰�
	virtual void onConnectionCheck() override;

	virtual void onConnectionClosed(std::shared_ptr<Socket> pSocket) override;

	/// <summary>
	/// <para>������ Ȱ��ȭ�� �����Ѵ�.</para>
	/// <para>Ȱ��ȭ�� ���� ������ ������ �� Ŭ���̾�Ʈ accept ������ �����Ѵٴ� �ǹ��̴�.</para>
	/// <para>�̹� Ȱ��ȭ�� �Ǿ� �ִ� ���, �� �Լ��� �ƹ��� ����� �������� ������ false�� ��ȯ�Ѵ�.</para>
	/// <para>Ȱ��ȭ ������ true��, �̹� Ȱ��ȭ �Ǿ� �ְų� ��Ÿ �� �� ���� ������ ���Ͽ� �Լ��� Ȱ��ȭ�� �� �� ���ٸ� false�� ��ȯ�Ѵ�.</para>
	/// </summary>
	/// <returns>Ȱ��ȭ ������ true, �̹� Ȱ��ȭ �Ǿ� �ְų� ��Ÿ �� �� ���� ������ ���Ͽ� �Լ��� Ȱ��ȭ�� �� �� ���ٸ� false</returns>
	bool run();

	/// <summary>
	/// <para>���� ������ �����Ѵ�.</para>
	/// <para>accept ���� �۾��� ����Ǹ�, ���� ���� ���� ��� Ŭ���̾�Ʈ���� ���ᵵ �������� �ȴ�.</para>
	/// <para>���� ������ �ٽ� �����ϱ� ���ؼ� <c>ServerNetwork::createServerSocket()</c> �Լ��� ���� ���� ������ �ٽ� �����Ͽ��� �Ѵ�.</para>
	/// <para>���� ���� ���� �� true��, ���� ���� ��ü�� �������� �ʰų� ��Ÿ �� �� ���� ������ ���� �� �� ���ٸ� false�� ��ȯ�Ѵ�.</para>
	/// </summary>
	/// <returns>���� ���� ���� �� true, ���� ���� ��ü�� �������� �ʰų� ��Ÿ �� �� ���� ������ ���� �� �� ���ٸ� false</returns>
	bool releaseServerSocket();

	/// <summary>
	/// ServerNetwork�� ���� ��ü�� ��ȯ�Ѵ�.
	/// </summary>
	/// <returns>ServerNetwork ��ü</returns>
	static ServerNetwork& getInstance();

	/// <summary>
	/// <para>�Ҹ����̴�.</para>
	/// <para>��Ʈ��ũ ��Ű� ������ ������ ������ ���Եȴ�.</para>
	/// </summary>
	~ServerNetwork();
};