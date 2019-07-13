// CSliceViewFilterDlg.cpp: 구현 파일
//

#include "stdafx.h"
#include "IntelliCAD.h"
#include "CSliceViewFilterDlg.h"
#include "afxdialogex.h"
#include "System.h"

using namespace std;

// CSliceViewFilterDlg 대화 상자

IMPLEMENT_DYNAMIC(CSliceViewFilterDlg, CDialogEx)

BEGIN_MESSAGE_MAP(CSliceViewFilterDlg, CDialogEx)
	ON_WM_SIZE()
	ON_WM_PAINT()
	ON_WM_GETMINMAXINFO()
	ON_CONTROL(CVN_MouseMovePlotArea, IDC_SLICE_VIEW_FILTER_chartView, OnMouseMovePlotArea)
END_MESSAGE_MAP()

CSliceViewFilterDlg::CSliceViewFilterDlg(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_DIALOG_SLICE_VIEW_FILTER, pParent)
{
	System::getSystemContents().
		getEventBroadcaster().addImageProcessorInitializeListener(*this);
}

CSliceViewFilterDlg::~CSliceViewFilterDlg()
{
}

void CSliceViewFilterDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_SLICE_VIEW_FILTER_chartView, __ddx_chartView);
}


// CSliceViewFilterDlg 메시지 처리기

bool CSliceViewFilterDlg::isActivated() const
{
	return __activated;
}

void CSliceViewFilterDlg::OnSize(UINT nType, int cx, int cy)
{
	CDialogEx::OnSize(nType, cx, cy);

	// TODO: 여기에 메시지 처리기 코드를 추가합니다.
	Invalidate();
}

void CSliceViewFilterDlg::OnPaint()
{
	// CPaintDC dc(this); // device context for painting
					   // TODO: 여기에 메시지 처리기 코드를 추가합니다.
					   // 그리기 메시지에 대해서는 CDialogEx::OnPaint()을(를) 호출하지 마십시오.
	if (__activated)
	{
		__render();
		OnMouseMovePlotArea();
	}

	ValidateRect(nullptr);
}

void CSliceViewFilterDlg::__init()
{
	__plotAreaLButtonDown = false;

	const MonoTransferFunction *pTransferFunc =
		System::getSystemContents().getRenderingEngine().imageProcessor.getTransferFunction();

	if (!pTransferFunc)
		return;

	__chartBuilder.import(*pTransferFunc);

	__activated = true;
}

void CSliceViewFilterDlg::__render()
{
	// TODO:  여기에 추가 초기화 작업을 추가합니다.
	CRect rect;
	GetClientRect(rect);

	__ddx_chartView.setChart(__chartBuilder.build(rect.Width(), rect.Height()).get());
}

void CSliceViewFilterDlg::OnMouseMovePlotArea()
{
	XYChart *pChart = static_cast<XYChart *>(__ddx_chartView.getChart());
	const int PLOT_X = __ddx_chartView.getPlotAreaMouseX();
	const int PLOT_Y = __ddx_chartView.getPlotAreaMouseY();

	// 마우스 좌측 버튼이 눌려있지 않다면
	if (!(GetKeyState(VK_LBUTTON) & 0x100))
		__plotAreaLButtonDown = false;

	if (__plotAreaLButtonDown)
	{
		__chartBuilder.setData(
			static_cast<int>(pChart->getXValue(PLOT_X)), pChart->getYValue(PLOT_Y, pChart->yAxis()));

		__render();

		pChart = static_cast<XYChart *>(__ddx_chartView.getChart());
	}

	MTFChartBuilder::drawCrosshair(pChart, PLOT_X, PLOT_Y);

	__ddx_chartView.updateDisplay();
	__ddx_chartView.removeDynamicLayer(CVN_MouseLeavePlotArea);
}

void CSliceViewFilterDlg::OnLButtonDownPlotArea()
{
	__plotAreaLButtonDown = true;
}

void CSliceViewFilterDlg::OnLButtonUpPlotArea()
{
	__plotAreaLButtonDown = false;
	__chartBuilder.finishSettingData();
}

BOOL CSliceViewFilterDlg::PreTranslateMessage(MSG* pMsg)
{
	// TODO: 여기에 특수화된 코드를 추가 및/또는 기본 클래스를 호출합니다.
	if (pMsg->hwnd == __ddx_chartView.GetSafeHwnd())
	{
		switch (pMsg->message)
		{
		case WM_LBUTTONDOWN:
			OnLButtonDownPlotArea();
			return true;

		case WM_LBUTTONUP:
			OnLButtonUpPlotArea();
			return true;
		}
	}

	return CDialogEx::PreTranslateMessage(pMsg);
}

void CSliceViewFilterDlg::OnGetMinMaxInfo(MINMAXINFO* lpMMI)
{
	// TODO: 여기에 메시지 처리기 코드를 추가 및/또는 기본값을 호출합니다.
	lpMMI->ptMinTrackSize.x = 500;
	lpMMI->ptMinTrackSize.y = 250;

	CDialogEx::OnGetMinMaxInfo(lpMMI);
}

void CSliceViewFilterDlg::onImageProcessorInitialize()
{
	__init();
	Invalidate();
}