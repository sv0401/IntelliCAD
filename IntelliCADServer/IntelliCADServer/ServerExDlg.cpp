
// ServerExDlg.cpp : implementation file
//

#include "stdafx.h"
#include <ctime>
#include <chrono>
#include "ServerEx.h"
#include "ServerExDlg.h"
#include "afxdialogex.h"
#include "System.h"
#include "ServerNetwork.h"
#include "Parser.hpp"
#include "EditingPortNumberDlg.h"
#include "MacroTransaction.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

// CServerExDlg dialog

using namespace std;


CServerExDlg::CServerExDlg(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_SERVEREX_DIALOG, pParent) , __ddx_radioIdx(0)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CServerExDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_EDIT_PORT, __ddx_edit_port);
	DDX_Radio(pDX, IDC_RADIO_SERVER1, __ddx_radioIdx);
	DDX_Control(pDX, IDC_BUTTON_EDIT_PORT, __ddx_button_editPort);
	DDX_Control(pDX, IDC_LIST_LOG, __ddx_list_log);
	DDX_Control(pDX, IDC_BUTTON_CONN_CHK, __ddx_button_connChk);
}

BEGIN_MESSAGE_MAP(CServerExDlg, CDialogEx)
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_MESSAGE(WM_KICKIDLE, OnKickIdle)
	ON_BN_CLICKED(IDC_BUTTON_EDIT_PORT, &CServerExDlg::OnBnClickedButtonEditPortNum)
	ON_CONTROL_RANGE(BN_CLICKED, IDC_RADIO_SERVER1, IDC_RADIO_SERVER2, OnRadioClicked)
	ON_WM_DESTROY()
	ON_BN_CLICKED(IDC_BUTTON_CONN_CHK, &CServerExDlg::OnBnClickedButtonConnChk)
END_MESSAGE_MAP()


// CServerExDlg message handlers

BOOL CServerExDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// Set the icon for this dialog.  The framework does this automatically
	//  when the application's main window is not a dialog
	SetIcon(m_hIcon, TRUE);			// Set big icon
	SetIcon(m_hIcon, FALSE);		// Set small icon

	// TODO: Add extra initialization here
	EventBroadcaster &eventBroadcaster = System::getInstance().eventBroadcaster;
	ServerNetwork &server = ServerNetwork::getInstance();

	eventBroadcaster.__addSystemListener(server);
	eventBroadcaster.addClientAcceptingListener(*this);
	eventBroadcaster.__notifySystemInit();

	__ddx_edit_port.SetWindowText(server.getPort().c_str());
	__ddx_radioIdx = 1;

	UpdateData(false);

	return TRUE;  // return TRUE  unless you set the focus to a control
}

// If you add a minimize button to your dialog, you will need the code below
//  to draw the icon.  For MFC applications using the document/view model,
//  this is automatically done for you by the framework.

void CServerExDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // device context for painting

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// Center icon in client rectangle
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// Draw the icon
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialogEx::OnPaint();
	}
}

// The system calls this function to obtain the cursor to display while the user drags
//  the minimized window.
HCURSOR CServerExDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}

LRESULT CServerExDlg::OnKickIdle(WPARAM, LPARAM)
{
	UpdateDialogControls(this, FALSE);

	System &system = System::getInstance();
	AsyncTaskManager &taskMgr = system.taskMgr;
	EventBroadcaster &eventBroadcaster = system.eventBroadcaster;

	for (const FinishedTask &task : taskMgr.getFinishedTasks())
	{
		switch (task.first)
		{
		case TaskType::GENERIC:
			eventBroadcaster.notifyGeneric();
			break;

		case TaskType::CONNECTION_CLOSED:
			eventBroadcaster.notifyConnectionClosed(
				any_cast<shared_ptr<Socket>>(task.second));
		}
	}

	return static_cast<LRESULT>(true);
}

void CServerExDlg::OnBnClickedButtonEditPortNum()
{
	// TODO: Add your control notification handler code here
	EditingPortNumberDlg dlg(__ddx_edit_port, this);

	if (dlg.DoModal() == IDOK)
	{
		CString portString;
		__ddx_edit_port.GetWindowText(portString);

		CString str;
		str.Format(_T("포트 번호가 %s로 변경되었습니다."), portString.GetString());
		__printLog(str.GetString());
	}
}

void CServerExDlg::OnRadioClicked(const UINT)
{
	const int PREV_IDX = __ddx_radioIdx;
	UpdateData(true);

	if (PREV_IDX == __ddx_radioIdx)
		return;

	switch (__ddx_radioIdx)
	{
	case 0:
		OnBnClickedRadioServerOn();
		break;

	case 1:
		OnBnClickedRadioServerOff();
		break;
	}
}

void CServerExDlg::onClientAcceptingResult(const shared_ptr<Socket> pSocket)
{
	if (pSocket)
		__printLog(_T("새로운 클라이언트가 접속하였습니다."));
}

void CServerExDlg::__printLog(const tstring &content)
{
	using namespace chrono;

	auto now = system_clock::now();
	time_t now_t = system_clock::to_time_t(now);
	
	tm now_tm;
	localtime_s(&now_tm, &now_t);

	char buffer[32];
	strftime(buffer, sizeof(buffer), "[%H:%M:%S] ", &now_tm);

	tstring outString =
		(Parser::LPCSTR$tstring(buffer) + content);

	__ddx_list_log.AddString(outString.c_str());
	__ddx_list_log.SetTopIndex(__ddx_list_log.GetCount() - 1);
}

void CServerExDlg::OnBnClickedRadioServerOn()
{
	// TODO: Add your control notification handler code here
	CString portString;
	__ddx_edit_port.GetWindowText(portString);

	ServerNetwork &network = ServerNetwork::getInstance();

	const bool SUCCESS =
		network.createServerSocket(Parser::CString$tstring(portString));

	if (!SUCCESS)
	{
		__printLog(_T("서버 구동에 실패하였습니다."));

		__ddx_radioIdx = 1;
		UpdateData(false);

		return;
	}

	__printLog(_T("서버가 구동되었습니다."));

	__ddx_button_editPort.EnableWindow(false);
	__ddx_button_connChk.EnableWindow();
	network.run();
}

void CServerExDlg::OnBnClickedRadioServerOff()
{
	// TODO: Add your control notification handler code here
	ServerNetwork &network = ServerNetwork::getInstance();
	
	if (network.releaseServerSocket())
		__printLog(_T("서버가 종료되었습니다."));
	else
		__printLog(_T("서버 종료 중 오류가 발생하였습니다. 비정상 종료됩니다."));

	__ddx_button_editPort.EnableWindow();
	__ddx_button_connChk.EnableWindow(false);
}

void CServerExDlg::OnDestroy()
{
	__super::OnDestroy();

	// TODO: Add your message handler code here
	EventBroadcaster &eventBroadcaster = System::getInstance().eventBroadcaster;

	eventBroadcaster.__notifySystemDestroy();
	eventBroadcaster.removeClientAcceptingListener(*this);
	eventBroadcaster.__removeSystemListener(ServerNetwork::getInstance());
}

BOOL CServerExDlg::PreTranslateMessage(MSG* pMsg)
{
	// TODO: Add your specialized code here and/or call the base class
	IF_T_RET_T(
		(pMsg->message == WM_KEYDOWN) &&
		((pMsg->wParam == VK_RETURN) || (pMsg->wParam == VK_ESCAPE)));

	return __super::PreTranslateMessage(pMsg);
}

void CServerExDlg::OnBnClickedButtonConnChk()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	EventBroadcaster &eventBroadcaster = System::getInstance().eventBroadcaster;

	eventBroadcaster.notifyConnectionCheck();
}
