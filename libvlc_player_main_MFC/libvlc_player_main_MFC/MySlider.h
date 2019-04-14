#pragma once
// CMySlider

class CMySlider : public CSliderCtrl
{
	DECLARE_DYNAMIC(CMySlider)

public:
	CMySlider();
	virtual ~CMySlider();

protected:
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);

	int m_mouseHoverPos;
	bool m_bisOkGetFrame;
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	afx_msg void OnMouseHover(UINT nFlags, CPoint point);
	afx_msg void OnMouseLeave();
};


