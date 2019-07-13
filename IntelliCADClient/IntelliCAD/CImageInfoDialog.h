#pragma once

#include "VolumeLoadingListener.h"

// CImageInfoDialog 대화 상자

class CImageInfoDialog : public CDialogEx, public VolumeLoadingListener
{
	DECLARE_DYNAMIC(CImageInfoDialog)
	DECLARE_MESSAGE_MAP()

public:
	CImageInfoDialog(CWnd* pParent = nullptr);   // 표준 생성자입니다.
	virtual ~CImageInfoDialog();

// 대화 상자 데이터입니다.
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_DIALOG_IMAGE_INFO };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.

public:
	virtual void onLoadVolume(const VolumeData &volumeData) override;
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);

private:
	CEdit __ddx_imageName;
	CEdit __ddx_memSizeW;
	CEdit __ddx_memSizeH;
	CEdit __ddx_memSizeD;
	CEdit __ddx_memSizeT;
	CEdit __ddx_spacingX;
	CEdit __ddx_spacingY;
	CEdit __ddx_spacingZ;
	CEdit __ddx_volSizeW;
	CEdit __ddx_volSizeH;
	CEdit __ddx_volSizeD;
public:
	virtual BOOL PreTranslateMessage(MSG* pMsg);
};
