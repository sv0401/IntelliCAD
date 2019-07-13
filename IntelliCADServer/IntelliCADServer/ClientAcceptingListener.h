/*
*	Copyright (C) 2019 APIless team. All right reserved.
*
*	���ϸ�			: ClientAcceptingListener.h
*	�ۼ���			: �̼���
*	���� ������		: 19.03.06
*
*	Ŭ���̾�Ʈ ���� �ݹ� �������̽�
*/

#pragma once

#include <memory>
#include "Socket.h"

/// <summary>
/// ������ ���ο� Ŭ���̾�Ʈ�� ���� �Ǿ��� ��, ���� �۾��� ó���ϴ� �ݹ� �������̽��̴�.
/// </summary>
class ClientAcceptingListener
{
public:
	/// <summary>
	/// <para>���ο� Ŭ���̾�Ʈ�� ���� �Ǿ��� �� �ݹ�Ǵ� �Լ��̴�.</para>
	/// <para>Ŭ���̾�Ʈ�� ����� ������ ���ڷ� ���´�.</para>
	/// <para>������ ����� �̷������ �ʾҴٸ� nullptr�� �Ѿ�´�.</para>
	/// </summary>
	/// <param name="pSocket">Ŭ���̾�Ʈ ����. ������ ����� �̷������ �ʾ��� �� nullptr</param>
	virtual void onClientAcceptingResult(std::shared_ptr<Socket> pSocket) = 0;
};