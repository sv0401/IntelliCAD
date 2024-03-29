#pragma once

#include "CRenderingView.h"

class CVolumeRenderingView : public CRenderingView
{
	DECLARE_DYNCREATE(CVolumeRenderingView)
	DECLARE_MESSAGE_MAP()

public:
	/* member function */
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnRButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnMButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	afx_msg BOOL OnMouseWheel(UINT nFlags, short zDelta, CPoint pt);

protected:
	/* member function */
	virtual void _onDeviceRender(Pixel* const pDevScreen, const int screenWidth, const int screenHeight) override;
	virtual RenderingScreenType _getScreenType() const override;

private:
	/* member variable */
	bool __lButtonDown = false;
	bool __mButtonDown = false;
	bool __rButtonDown = false;
	CPoint __prevPos;

	bool __dblClickSemaphore = false;

public:
	afx_msg void OnLButtonDblClk(UINT nFlags, CPoint point);
	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnMButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnRButtonUp(UINT nFlags, CPoint point);
};

