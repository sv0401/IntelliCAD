// CManipDialog.cpp: 구현 파일
//

#include "stdafx.h"
#include "IntelliCAD.h"
#include "CManipDialog.h"
#include "afxdialogex.h"
#include "System.h"


// CManipDialog 대화 상자

IMPLEMENT_DYNAMIC(CManipDialog, CDialogEx)

BEGIN_MESSAGE_MAP(CManipDialog, CDialogEx)
	ON_BN_CLICKED(IDC_MANIP_BUTTON_initSlicingPoint, &CManipDialog::OnBnClickedManipButtonInitSlicingPoint)
	ON_BN_CLICKED(IDC_MANIP_BUTTON_moveTo, &CManipDialog::OnBnClickedManipButtonMoveTo)
	ON_WM_CREATE()
END_MESSAGE_MAP()

CManipDialog::CManipDialog(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_DIALOG_MANIP, pParent)
{
	System::getSystemContents().
		getEventBroadcaster().addChangeSlicingPointListener(*this);
}

CManipDialog::~CManipDialog()
{
}

void CManipDialog::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_MANIP_EDIT_slicingPointX, __ddx_slicingPointX);
	DDX_Control(pDX, IDC_MANIP_EDIT_slicingPointY, __ddx_slicingPointY);
	DDX_Control(pDX, IDC_MANIP_EDIT_slicingPointZ, __ddx_slicingPointZ);
}

// CManipDialog 메시지 처리기

void CManipDialog::onLoadVolume(const VolumeData &volumeData)
{
	__initialized = true;
}

void CManipDialog::onChangeSlicingPoint(const Point3D &slicingPoint)
{
	CString tmp;

	// Slicing point - x
	tmp.Format(_T("%f"), slicingPoint.x);
	__ddx_slicingPointX.SetWindowText(tmp);

	// Slicing point - y
	tmp.Format(_T("%f"), slicingPoint.y);
	__ddx_slicingPointY.SetWindowText(tmp);

	// Slicing point - z
	tmp.Format(_T("%f"), slicingPoint.z);
	__ddx_slicingPointZ.SetWindowText(tmp);
}

void CManipDialog::OnBnClickedManipButtonInitSlicingPoint()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	if (!__initialized)
		return;

	System::getSystemContents().
		getRenderingEngine().imageProcessor.setSlicingPointAdj({ 0.f, 0.f, 0.f });

	System::getSystemContents().
		getEventBroadcaster().notifyRequestScreenUpdate(RenderingScreenType::SLICE);
}


void CManipDialog::OnBnClickedManipButtonMoveTo()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	if (!__initialized)
		return;
	
	CString tmp;

	// Slicing point - x
	__ddx_slicingPointX.GetWindowText(tmp);
	const float pointX = static_cast<float>(_ttof(tmp));

	__ddx_slicingPointY.GetWindowText(tmp);
	const float pointY = static_cast<float>(_ttof(tmp));

	__ddx_slicingPointZ.GetWindowText(tmp);
	const float pointZ = static_cast<float>(_ttof(tmp));

	System::getSystemContents().
		getRenderingEngine().imageProcessor.setSlicingPoint({ pointX, pointY, pointZ });

	System::getSystemContents().
		getEventBroadcaster().notifyRequestScreenUpdate(RenderingScreenType::SLICE);
}


int CManipDialog::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (__super::OnCreate(lpCreateStruct) == -1)
		return -1;

	// TODO:  여기에 특수화된 작성 코드를 추가합니다.
	System::getSystemContents().
		getEventBroadcaster().addVolumeLoadingListener(*this);

	return 0;
}

void CManipDialog::OnOK()
{
	// TODO: 여기에 특수화된 코드를 추가 및/또는 기본 클래스를 호출합니다.
	if (GetFocus()->IsKindOf(RUNTIME_CLASS(CEdit)))
		OnBnClickedManipButtonMoveTo();
}


void CManipDialog::OnCancel()
{
	// TODO: 여기에 특수화된 코드를 추가 및/또는 기본 클래스를 호출합니다.

	// __super::OnCancel();
}
