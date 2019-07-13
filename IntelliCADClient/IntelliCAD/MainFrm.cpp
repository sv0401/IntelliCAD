// 이 MFC 샘플 소스 코드는 MFC Microsoft Office Fluent 사용자 인터페이스("Fluent UI")를
// 사용하는 방법을 보여 주며, MFC C++ 라이브러리 소프트웨어에 포함된
// Microsoft Foundation Classes Reference 및 관련 전자 문서에 대해
// 추가적으로 제공되는 내용입니다.
// Fluent UI를 복사, 사용 또는 배포하는 데 대한 사용 약관은 별도로 제공됩니다.
// Fluent UI 라이선싱 프로그램에 대한 자세한 내용은
// https://go.microsoft.com/fwlink/?LinkId=238214.
//
// Copyright (C) Microsoft Corporation
// All rights reserved.

// MainFrm.cpp: CMainFrame 클래스의 구현
//

#include "stdafx.h"
#include "IntelliCAD.h"
#include "MainFrm.h"
#include "IntelliCADView.h"
#include "CVolumeRenderingView.h"
#include "CInspecterView.h"
#include "tstring.h"
#include "Parser.hpp"
#include "System.h"
#include "VolumeReader.h"
#include "CSliceView.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

// CMainFrame

using namespace std;

IMPLEMENT_DYNCREATE(CMainFrame, CFrameWndEx)

BEGIN_MESSAGE_MAP(CMainFrame, CFrameWndEx)
	ON_WM_CREATE()
	ON_COMMAND(ID_FILE_OPEN, &CMainFrame::OnFileOpen)
	ON_COMMAND(ID_BUTTON_CLOUD, &CMainFrame::OnButtonCloudService)
	ON_COMMAND(ID_BUTTON_LIGHT1, &CMainFrame::OnButtonLight1)
	ON_COMMAND(ID_BUTTON_LIGHT2, &CMainFrame::OnButtonLight2)
	ON_COMMAND(ID_BUTTON_LIGHT3, &CMainFrame::OnButtonLight3)
	ON_COMMAND(ID_MAIN_RIBBON_SLICE_FILTER, &CMainFrame::OnMainRibbonSliceFilter)
	ON_COMMAND(ID_MAIN_RIBBON_VOLUME_FILTER, &CMainFrame::OnMainRibbonVolumeFilter)
	ON_COMMAND_RANGE(ID_FILE_MRU_FILE1, ID_FILE_MRU_FILE4, OnOpenMRUFile)
END_MESSAGE_MAP()

// CMainFrame 생성/소멸

CMainFrame::CMainFrame() noexcept :
	__parentSplitterWnd(.7f, 1.f)
{}

CMainFrame::~CMainFrame()
{}

int CMainFrame::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CFrameWndEx::OnCreate(lpCreateStruct) == -1)
		return -1;

	BOOL bNameValid;

	m_wndRibbonBar.Create(this);
	m_wndRibbonBar.LoadFromResource(IDR_RIBBON);
	m_wndRibbonBar.DeleteDropdown();

	if (!m_wndStatusBar.Create(this))
	{
		TRACE0("상태 표시줄을 만들지 못했습니다.\n");
		return -1;      // 만들지 못했습니다.
	}

	CString strTitlePane1;
	CString strTitlePane2;
	bNameValid = strTitlePane1.LoadString(IDS_STATUS_PANE1);
	ASSERT(bNameValid);
	bNameValid = strTitlePane2.LoadString(IDS_STATUS_PANE2);
	ASSERT(bNameValid);
	m_wndStatusBar.AddElement(new CMFCRibbonStatusBarPane(ID_STATUSBAR_PANE1, strTitlePane1, TRUE), strTitlePane1);
	m_wndStatusBar.AddExtendedElement(new CMFCRibbonStatusBarPane(ID_STATUSBAR_PANE2, strTitlePane2, TRUE), strTitlePane2);

	// Visual Studio 2005 스타일 도킹 창 동작을 활성화합니다.
	CDockingManager::SetDockingMode(DT_SMART);
	// Visual Studio 2005 스타일 도킹 창 자동 숨김 동작을 활성화합니다.
	EnableAutoHidePanes(CBRS_ALIGN_ANY);

	// 모든 사용자 인터페이스 요소를 그리는 데 사용하는 비주얼 관리자를 설정합니다.
	CMFCVisualManager::SetDefaultManager(RUNTIME_CLASS(CMFCVisualManagerOffice2007));

	// 비주얼 관리자에서 사용하는 비주얼 스타일을 설정합니다.
	CMFCVisualManagerOffice2007::SetStyle(CMFCVisualManagerOffice2007::Office2007_Silver);

	// TODO: 여기에 추가적인 초기화 내용을 작성합니다.
	__sliceViewFilterDlg.Create(IDD_DIALOG_SLICE_VIEW_FILTER, this);

	return 0;
}

BOOL CMainFrame::PreCreateWindow(CREATESTRUCT& cs)
{
	if( !CFrameWndEx::PreCreateWindow(cs) )
		return FALSE;
	// TODO: CREATESTRUCT cs를 수정하여 여기에서
	//  Window 클래스 또는 스타일을 수정합니다.

	// 타이틀을 변경한다.
	cs.style &= ~FWS_ADDTOTITLE;
	cs.lpszName = TEXT("IntelliCAD");

	return TRUE;
}

// CMainFrame 진단

#ifdef _DEBUG
void CMainFrame::AssertValid() const
{
	CFrameWndEx::AssertValid();
}

void CMainFrame::Dump(CDumpContext& dc) const
{
	CFrameWndEx::Dump(dc);
}
#endif //_DEBUG


// CMainFrame 메시지 처리기
BOOL CMainFrame::OnCreateClient(LPCREATESTRUCT lpcs, CCreateContext* pContext)
{
	// 1 * 2 정적 분할 윈도우를 만든다.
	__parentSplitterWnd.CreateStatic(this, 1, 2);

	// 좌측 윈도우를 2 * 2로 분할한다.
	__childSplitterWnd.CreateStatic(
		&__parentSplitterWnd, 2, 2,
		(WS_CHILD | WS_VISIBLE | WS_BORDER),
		__parentSplitterWnd.IdFromRowCol(0, 0));

	// 좌측 윈도우에 렌더링 뷰를 할당한다.
	__childSplitterWnd.CreateView(0, 0, RUNTIME_CLASS(CSliceView), {}, pContext);
	__childSplitterWnd.CreateView(0, 1, RUNTIME_CLASS(CSliceView), {}, pContext);
	__childSplitterWnd.CreateView(1, 0, RUNTIME_CLASS(CSliceView), {}, pContext);
	__childSplitterWnd.CreateView(1, 1, RUNTIME_CLASS(CVolumeRenderingView), {}, pContext);

	// 좌측 윈도우 View를 구분하기 위하여 인덱스를 설정한다.

	// Top-Left
	CSliceView &topView = *__childSplitterWnd.getChildView<CSliceView>(0, 0);

	// Top-Right
	CSliceView &frontView = *__childSplitterWnd.getChildView<CSliceView>(0, 1);

	// Bottom-Left
	CSliceView &rightView = *__childSplitterWnd.getChildView<CSliceView>(1, 0);

	// Bottom-Right
	CVolumeRenderingView &volumeRenderingView = *__childSplitterWnd.getChildView<CVolumeRenderingView>(1, 1);

	// 렌더링 뷰 초기화
	topView.init(0, SliceAxis::TOP);
	frontView.init(1, SliceAxis::FRONT);
	rightView.init(2, SliceAxis::RIGHT);
	volumeRenderingView.init(3);

	__parentSplitterWnd.splitted = true;
	__childSplitterWnd.splitted = true;

	// 우측 윈도우에 Inspecter 뷰를 할당한다.
	__parentSplitterWnd.CreateView(0, 1, RUNTIME_CLASS(CInspecterView), {}, pContext);

	return true;
	// return CFrameWndEx::OnCreateClient(lpcs, pContext);
}

void CMainFrame::__openFile(const CString &path)
{
	AfxGetApp()->AddToRecentFileList(path);

	const tstring PATH = Parser::CString$tstring(path);
	const VolumeData RESULT = VolumeReader::readMetaImage(PATH);

	if (!RESULT.pBuffer)
	{
		AfxMessageBox(_T("해당 경로에 위치한 파일을 읽을 수 없습니다."), MB_ICONERROR);
		return;
	}

	System::getSystemContents().loadVolume(RESULT);
}

void CMainFrame::OnFileOpen()
{
	// TODO: 여기에 명령 처리기 코드를 추가합니다.
	const TCHAR *const szFilter = _T("MetaImage (*.mhd) |*.mhd|");
	CFileDialog fileDlg(true, _T("MetaImage(*.mhd)"), nullptr, OFN_FILEMUSTEXIST, szFilter, nullptr);

	if (fileDlg.DoModal() == IDOK)
		__openFile(fileDlg.GetPathName());
}


void CMainFrame::OnButtonCloudService()
{
	RemoteAccessAuthorizer &accessAuthorizer =
		System::getSystemContents().getRemoteAccessAuthorizer();

	if (!accessAuthorizer.isAuthorized())
		if (__loginDlg.DoModal() == LOGIN_DLG_LOGIN_SUCCESS)
			MessageBox(_T("로그인에 성공하였습니다."), _T("로그인 성공"), MB_ICONINFORMATION);
}

void CMainFrame::OnButtonLight1()
{
	// TODO: 여기에 명령 처리기 코드를 추가합니다.
}


void CMainFrame::OnButtonLight2()
{
	// TODO: 여기에 명령 처리기 코드를 추가합니다.
}


void CMainFrame::OnButtonLight3()
{
	// TODO: 여기에 명령 처리기 코드를 추가합니다.
}

void CMainFrame::OnMainRibbonSliceFilter()
{
	// TODO: 여기에 명령 처리기 코드를 추가합니다.
	if (!__sliceViewFilterDlg.isActivated())
		return;

	if (__sliceViewFilterDlg.IsWindowVisible())
		__sliceViewFilterDlg.ShowWindow(SW_RESTORE);
	else
		__sliceViewFilterDlg.ShowWindow(SW_SHOWDEFAULT);
}


void CMainFrame::OnMainRibbonVolumeFilter()
{
	// TODO: 여기에 명령 처리기 코드를 추가합니다.
}

void CMainFrame::OnOpenMRUFile(UINT nID)
{
	int index = 0;
	switch (nID)
	{
	case ID_FILE_MRU_FILE1:
		index = 0;
		break;

	case ID_FILE_MRU_FILE2:
		index = 1;
		break;

	case ID_FILE_MRU_FILE3:
		index = 2;
		break;

	case ID_FILE_MRU_FILE4:
		index = 3;
		break;
	}

	const CString &PATH =
		static_cast<CIntelliCADApp *>(AfxGetApp())->getRecentFileName(index);

	__openFile(PATH);
}