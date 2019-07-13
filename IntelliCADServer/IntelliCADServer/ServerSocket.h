/*
*	Copyright (C) 2019 APIless team. All right reserved.
*
*	���ϸ�			: ServerSocket.h
*	�ۼ���			: ������
*	���� ������		: 19.04.09
*
*	���� ���� Ŭ����
*/

#pragma once

#include <WinSock2.h>
#include <memory>
#include "tstring.h"
#include "Socket.h"

/// <summary>
/// <para>Ŭ���̾�Ʈ�� ����ϱ� ���� ���� ���� Ŭ�����̴�.</para>
/// <para>�ֿ� ����� Ŭ���̾�Ʈ�� ���� ��û�� accept �ϴ� ���̴�.</para>
/// <para>�ܺο��� ������ �����ڰ� �������� ������, <c>ServerSocket::create()</c>�Լ��� ���ؼ� �ν��Ͻ��� ��´�.</para>
/// </summary>
class ServerSocket
{
private:

	/// <summary>
	/// client accept �� SOCKET
	/// </summary>
	SOCKET __sock;

	/// <summary>
	/// ���� ���� Ŭ������ ���ο� ����� �������̴�.
	/// </summary>
	/// <param name="hServerSockRaw">raw data</param>
	/// <param name="serverSockAddr">raw data</param>
	/// <param name="maxConnection">���� ������ �ִ� Ŭ���̾�Ʈ ��</param>
	ServerSocket(const SOCKET &hServerSockRaw, const SOCKADDR_IN &serverSockAddr, const int maxConnection);

	ServerSocket(const ServerSocket &) = delete;
	ServerSocket(ServerSocket &&) = delete;

public:
	const int MAX_CONNECTION;

	/// <summary>
	/// Ŭ���̾�Ʈ ������ �޾Ƶ��̴� accept �۾��� �����Ѵ�.
	/// </summary>
	/// <returns>accept�� ������ �����̸� true, �Ұ����ϸ� false</returns>
	std::shared_ptr<Socket> accept();

	/// <summary>
	/// ���� ������ �ݴ´�. accept �۾��� ���� ���̶�� �� ���� �����Ѵ�.
	/// </summary>
	/// <returns>������ ���������� �����ٸ� true, ��Ÿ ������ ���� ���� �� false</returns>
	bool close();

	/// <summary>
	/// �Ҹ����̴�. ��Ʈ��ũ ��Ű� ������ ���ҽ� ������ ������ ���Եȴ�.
	/// </summary>
	~ServerSocket();

	/// <summary>
	/// <para>�־��� ��Ʈ�� ������ ���� ������ �����Ѵ�.</para>
	/// <para>�ִ�� ���� ������ Ŭ���̾�Ʈ�� ���� ������ ����� �� ������, �⺻ ���� INT_MAX�̴�.</para>
	/// <para>���� ������ ���������� ������ �� �ִٸ� ��ü�� �����͸�, ���� ���� �� nullptr�� ��ȯ�Ѵ�.</para>
	/// </summary>
	/// <param name="port">������ ������ ��Ʈ</param>
	/// <param name="maxConnection">�ִ� ���� ������ Ŭ���̾�Ʈ ��</param>
	/// <returns>���� ������ ���������� ������ �� �ִٸ� ��ü�� ������, ���� ���� �� nullptr</returns>
	static std::unique_ptr<ServerSocket> create(const std::tstring &port, int maxConnection = INT_MAX);
};