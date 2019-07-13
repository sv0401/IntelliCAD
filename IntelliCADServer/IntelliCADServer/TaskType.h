/*
*	Copyright (C) 2019 APIless team. All right reserved.
*
*	���ϸ�			: TaskType.h
*	�ۼ���			: �̼���
*	���� ������		: 19.04.06
*
*	�۾��� ������ ������ ������ Ŭ����
*/

#pragma once

/// <summary>
/// AsyncTaskManager���� �۾��� ������ ǥ���ϱ� ���� ������ Ŭ����
/// </summary>
enum class TaskType
{
	/// <summary>
	/// <para>�������� ����ϴ� �۾� Ÿ���̴�.</para>
	/// <para>Ư���� ��ɰ� �����Ǿ� ���� ������, ����� ������ ��� �����ϴ�.</para>
	/// </summary>
	GENERIC,

	/// <summary>
	/// �񵿱� ó���� ��û�ϵ�, ó�� ����� ����ް� ���� ���� �� ����Ѵ�.
	/// </summary>
	INGNORED,

	CONNECTION_CLOSED
};