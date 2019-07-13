#pragma once

#include "ChangeSlicingPointListener.h"
#include "VolumeLoadingListener.h"

// CManipDialog 대화 상자

class CManipDialog : public CDialogEx, public VolumeLoadingListener, public ChangeSlicingPointListener
{
	DECLARE_DYNAMIC(CManipDialog)
	DECLARE_MESSAGE_MAP()

public:
	CManipDialog(CWnd* pParent = nullptr);   // 표준 생성자입니다.
	virtual ~CManipDialog();

// 대화 상자 데이터입니다.
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_DIALOG_MANIP };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.

public:
	virtual void onLoadVolume(const VolumeData &volumeData) override;
	virtual void onChangeSlicingPoint(const Point3D &slicingPoint) override;

private:
	CEdit __ddx_slicingPointX;
	CEdit __ddx_slicingPointY;
	CEdit __ddx_slicingPointZ;

	bool __initialized = false;

public:
	afx_msg void OnBnClickedManipButtonInitSlicingPoint();
	afx_msg void OnBnClickedManipButtonMoveTo();
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	virtual void OnOK();
	virtual void OnCancel();
};
