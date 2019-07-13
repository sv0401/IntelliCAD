/*
*	Copyright (C) 2019 APIless team. All right reserved.
*
*	파일명			: System.cpp
*	작성자			: 이세인
*	최종 수정일		: 19.03.22
*
*	시스템 모듈
*/

#include "stdafx.h"
#include "System.h"
#include "IntelliCADServerDlg.h"

using namespace std;

System System::__instance;

void System::__setLogDlgReference(CIntelliCADServerDlg &dlg)
{
	__pLogDlg = &dlg;
}

void System::__init()
{
	systemContents.__init();
}

void System::__release()
{
	systemContents.__release();
}

void System::SystemContents::__init()
{
	__pTaskMgr = new AsyncTaskManager();
	__pEventBroadcaster = new EventBroadcaster();
	__pServerDBManager = new ServerDBManager();
	__pServerNetwork = &ServerNetwork::getInstance();

	// SystemInitListeners
	__pEventBroadcaster->__addSystemInitListener(*__pServerNetwork);
	__pEventBroadcaster->__notifySystemInit();
}

AsyncTaskManager &System::SystemContents::getTaskManager()
{
	return *__pTaskMgr;
}

EventBroadcaster &System::SystemContents::getEventBroadcaster()
{
	return *__pEventBroadcaster;
}

ServerDBManager &System::SystemContents::getServerDBManager()
{
	return *__pServerDBManager;
}

ServerNetwork &System::SystemContents::getServerNetwork()
{
	return *__pServerNetwork;
}

void System::SystemContents::__release()
{
	if (__pEventBroadcaster)
	{
		delete __pEventBroadcaster;
		__pEventBroadcaster = nullptr;
	}

	if (__pServerDBManager)
	{
		delete __pServerDBManager;
		__pServerDBManager = nullptr;
	}

	if (__pTaskMgr)
	{
		delete __pTaskMgr;
		__pTaskMgr = nullptr;
	}
}

bool System::printLog(const tstring &message)
{
	if (__pLogDlg)
		__pLogDlg->printLog(message);

	return __pLogDlg;
}

System& System::getInstance()
{
	return __instance;
}

System::SystemContents& System::getSystemContents()
{
	return __instance.systemContents;
}