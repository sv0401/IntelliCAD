#pragma once

#include "MTFChartBuilder.h"
#include "ImageProcessorInitializeListener.h"

// CSliceViewFilterDlg 대화 상자

class CSliceViewFilterDlg : public CDialogEx, public ImageProcessorInitializeListener
{
	DECLARE_DYNAMIC(CSliceViewFilterDlg)
	DECLARE_MESSAGE_MAP()

public:
	CSliceViewFilterDlg(CWnd* pParent = nullptr);   // 표준 생성자입니다.
	virtual ~CSliceViewFilterDlg();

// 대화 상자 데이터입니다.
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_DIALOG_SLICE_VIEW_FILTER };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.

private:
	CChartViewer __ddx_chartView;
	MTFChartBuilder __chartBuilder;

	bool __activated = false;
	bool __plotAreaLButtonDown = false;

	void __init();
	void __render();

public:
	bool isActivated() const;

	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnPaint();

	void OnLButtonDownPlotArea();
	void OnLButtonUpPlotArea();
	void OnMouseMovePlotArea();

	virtual BOOL PreTranslateMessage(MSG* pMsg);
	afx_msg void OnGetMinMaxInfo(MINMAXINFO* lpMMI);

	virtual void onImageProcessorInitialize() override;
};
