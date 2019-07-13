#pragma once

#include "AsyncTaskManager.hpp"
#include "EventBroadcaster.h"
#include "ServerDBManager.h"
#include "ServerNetwork.h"

class System
{
private:
	friend class CIntelliCADServerApp;
	friend class CIntelliCADServerDlg;

	CIntelliCADServerDlg *__pLogDlg = nullptr;

	static System __instance;

	System() = default;

	void __setLogDlgReference(CIntelliCADServerDlg &dlg);

	void __init();
	void __release();

public:
	class SystemContents
	{
	private:
		friend System;

		AsyncTaskManager *__pTaskMgr = nullptr;
		EventBroadcaster *__pEventBroadcaster = nullptr;
		ServerDBManager *__pServerDBManager = nullptr;
		ServerNetwork *__pServerNetwork = nullptr;

		void __init();
		void __release();

	public:
		AsyncTaskManager &getTaskManager();
		EventBroadcaster &getEventBroadcaster();
		ServerDBManager &getServerDBManager();
		ServerNetwork &getServerNetwork();
	}
	systemContents;

	bool printLog(const std::tstring &message);

	static System& getInstance();
	static System::SystemContents& getSystemContents();
};
