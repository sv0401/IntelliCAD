
// IntelliCADServerDlg.cpp: 구현 파일
//

#include "stdafx.h"
#include <ctime>
#include <chrono>
#include "IntelliCADServer.h"
#include "IntelliCADServerDlg.h"
#include "afxdialogex.h"
#include "System.h"
#include "Parser.hpp"
#include "Account.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

using namespace std;

// 응용 프로그램 정보에 사용되는 CAboutDlg 대화 상자입니다.

class CAboutDlg : public CDialogEx
{
public:
	CAboutDlg();

// 대화 상자 데이터입니다.
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_ABOUTBOX };
#endif

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.

// 구현입니다.
protected:
	DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() : CDialogEx(IDD_ABOUTBOX)
{
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialogEx)
END_MESSAGE_MAP()


// CIntelliCADServerDlg 대화 상자



CIntelliCADServerDlg::CIntelliCADServerDlg(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_INTELLICADSERVER_DIALOG, pParent), __ddx_radioIdx(0)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);

	System::getInstance().__setLogDlgReference(*this);
}

void CIntelliCADServerDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Radio(pDX, IDC_RADIO_SERVER1, __ddx_radioIdx);
	DDX_Control(pDX, IDC_LIST_LOG, __ddx_list_log);
}

BEGIN_MESSAGE_MAP(CIntelliCADServerDlg, CDialogEx)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_MESSAGE(WM_KICKIDLE, OnKickIdle)
	ON_CONTROL_RANGE(BN_CLICKED, IDC_RADIO_SERVER1, IDC_RADIO_SERVER2, OnRadioClicked)
	ON_WM_QUERYDRAGICON()
END_MESSAGE_MAP()


// CIntelliCADServerDlg 메시지 처리기

BOOL CIntelliCADServerDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// 시스템 메뉴에 "정보..." 메뉴 항목을 추가합니다.

	// IDM_ABOUTBOX는 시스템 명령 범위에 있어야 합니다.
	ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
	ASSERT(IDM_ABOUTBOX < 0xF000);

	CMenu* pSysMenu = GetSystemMenu(FALSE);
	if (pSysMenu != nullptr)
	{
		BOOL bNameValid;
		CString strAboutMenu;
		bNameValid = strAboutMenu.LoadString(IDS_ABOUTBOX);
		ASSERT(bNameValid);
		if (!strAboutMenu.IsEmpty())
		{
			pSysMenu->AppendMenu(MF_SEPARATOR);
			pSysMenu->AppendMenu(MF_STRING, IDM_ABOUTBOX, strAboutMenu);
		}
	}

	// 이 대화 상자의 아이콘을 설정합니다.  응용 프로그램의 주 창이 대화 상자가 아닐 경우에는
	//  프레임워크가 이 작업을 자동으로 수행합니다.
	SetIcon(m_hIcon, TRUE);			// 큰 아이콘을 설정합니다.
	SetIcon(m_hIcon, FALSE);		// 작은 아이콘을 설정합니다.

	// TODO: 여기에 추가 초기화 작업을 추가합니다.
	__ddx_radioIdx = 1;

	UpdateData(false);

	return TRUE;  // 포커스를 컨트롤에 설정하지 않으면 TRUE를 반환합니다.
}

void CIntelliCADServerDlg::OnSysCommand(UINT nID, LPARAM lParam)
{
	if ((nID & 0xFFF0) == IDM_ABOUTBOX)
	{
		CAboutDlg dlgAbout;
		dlgAbout.DoModal();
	}
	else
	{
		CDialogEx::OnSysCommand(nID, lParam);
	}
}

// 대화 상자에 최소화 단추를 추가할 경우 아이콘을 그리려면
//  아래 코드가 필요합니다.  문서/뷰 모델을 사용하는 MFC 응용 프로그램의 경우에는
//  프레임워크에서 이 작업을 자동으로 수행합니다.

void CIntelliCADServerDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // 그리기를 위한 디바이스 컨텍스트입니다.

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// 클라이언트 사각형에서 아이콘을 가운데에 맞춥니다.
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// 아이콘을 그립니다.
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialogEx::OnPaint();
	}
}

// 사용자가 최소화된 창을 끄는 동안에 커서가 표시되도록 시스템에서
//  이 함수를 호출합니다.
HCURSOR CIntelliCADServerDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}

BOOL CIntelliCADServerDlg::PreTranslateMessage(MSG* pMsg)
{
	// TODO: 여기에 특수화된 코드를 추가 및/또는 기본 클래스를 호출합니다.
	if ((pMsg->wParam == VK_RETURN) || (pMsg->wParam == VK_ESCAPE))
		return TRUE;

	return CDialogEx::PreTranslateMessage(pMsg);
}

void CIntelliCADServerDlg::OnRadioClicked(const UINT)
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

void CIntelliCADServerDlg::OnBnClickedRadioServerOn()
{
	ServerNetwork &network = System::getSystemContents().getServerNetwork();
	ServerDBManager &dbManager = System::getSystemContents().getServerDBManager();

	if (!dbManager.isLoaded())
		if (!dbManager.load())
		{
			printLog(_T("DB로부터 서버 구동에 필요한 정보를 불러오는데 실패하였습니다."));
			__ddx_radioIdx = 1;
			UpdateData(false);

			return;
		}

	const ConfigDBManager &configMgr = dbManager.getConfigManager();

	const bool SUCCESS =
		network.createServerSocket(
			configMgr.getAttribute(ConfigDBSectionType::SERVER_NETWORK, _T("port")));

	if (!SUCCESS)
	{
		printLog(_T("서버 구동에 실패하였습니다."));

		__ddx_radioIdx = 1;
		UpdateData(false);

		return;
	}

	printLog(_T("서버가 구동되었습니다."));

	network.run();
}

void CIntelliCADServerDlg::OnBnClickedRadioServerOff()
{
	ServerNetwork &network = ServerNetwork::getInstance();

	if (network.releaseServerSocket())
		printLog(_T("서버가 종료되었습니다."));
	else
		printLog(_T("서버 종료 중 오류가 발생하였습니다. 비정상 종료됩니다."));
}

void CIntelliCADServerDlg::printLog(const std::tstring &message)
{
	using namespace chrono;

	auto now = system_clock::now();
	time_t now_t = system_clock::to_time_t(now);

	tm now_tm;
	localtime_s(&now_tm, &now_t);

	char buffer[32];
	strftime(buffer, sizeof(buffer), "[%H:%M:%S] ", &now_tm);

	tstring outString =
		(Parser::LPCSTR$tstring(buffer) + message);

	__ddx_list_log.AddString(outString.c_str());
	__ddx_list_log.SetTopIndex(__ddx_list_log.GetCount() - 1);
}

LRESULT CIntelliCADServerDlg::OnKickIdle(WPARAM, LPARAM)
{
	UpdateDialogControls(this, false);

	AsyncTaskManager &taskMgr = System::getSystemContents().getTaskManager();
	EventBroadcaster &eventBroadcaster = System::getSystemContents().getEventBroadcaster();

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