// MySlider.cpp : 实现文件
//

#include "stdafx.h"
#include "libvlc_player_main_MFC.h"
#include "MySlider.h"


// CMySlider

IMPLEMENT_DYNAMIC(CMySlider, CSliderCtrl)

CMySlider::CMySlider()
	:m_mouseHoverPos(0), m_bisOkGetFrame(false)
{

}

CMySlider::~CMySlider()
{
}


BEGIN_MESSAGE_MAP(CMySlider, CSliderCtrl)
	ON_WM_LBUTTONDOWN()
	ON_WM_MOUSEMOVE()
	ON_WM_MOUSEHOVER()

	ON_WM_MOUSELEAVE()
END_MESSAGE_MAP()


// CMySlider 消息处理程序

void CMySlider::OnLButtonDown(UINT nFlags, CPoint point)
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值
	SendMessage(WM_KILLFOCUS, -1, 0); //失去焦点
	this->EnableWindow(FALSE);//设置不可用

	CRect rectChannel, rectThumb;
	GetChannelRect(rectChannel);//获取滑块槽在拉动条的区域坐标
	GetThumbRect(rectThumb);//获取滑块在拉动条的区域坐标

	int nMax = 0;
	int nMin = 0;
	GetRange(nMin, nMax);
	int nDelta = nMax - nMin;
	int nPos = ((point.x - rectChannel.left)*nDelta) / (rectChannel.right - rectChannel.left);

	if ( nPos > nMax / 2 )
	{
		nPos = ((point.x + (rectThumb.right - rectThumb.left) / 2
			- rectChannel.left)*nDelta) / (rectChannel.right - rectChannel.left);
	}

	CSliderCtrl::SetPos(nPos);
	this->EnableWindow(TRUE);//设置可用

	this->SetFocus();//获得焦点
	CSliderCtrl::OnLButtonDown(nFlags, point);
}


void CMySlider::OnMouseMove(UINT nFlags, CPoint point)
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值
	TRACKMOUSEEVENT tme;
	tme.cbSize = sizeof(tme);	//结构体缓冲区大小
	tme.dwFlags = TME_HOVER;	//注册WM_MOUSEHOVER消息
	tme.dwHoverTime = 1200;		//WM_MOUSEHOVER消息触发间隔时间	  1.2s
	tme.hwndTrack = m_hWnd;		//当窗口句柄

	::TrackMouseEvent(&tme);	//注册发送消息
	CSliderCtrl::OnMouseMove(nFlags, point);
}


void CMySlider::OnMouseHover(UINT nFlags, CPoint point)
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值
	//获取鼠标悬停位置
	CRect rectChannel, rectThumb;
	GetChannelRect(rectChannel);//获取滑块槽在拉动条的区域坐标
	GetThumbRect(rectThumb);//获取滑块在拉动条的区域坐标

	int nMax = 0;
	int nMin = 0;
	GetRange(nMin, nMax);
	int nDelta = nMax - nMin;
	int nPos = ((point.x - rectChannel.left)*nDelta) / (rectChannel.right - rectChannel.left);

	if ( nPos > nMax / 2 )
	{
		nPos = ((point.x + (rectThumb.right - rectThumb.left) / 2
			- rectChannel.left)*nDelta) / (rectChannel.right - rectChannel.left);
	}

	m_mouseHoverPos = nPos;
	m_bisOkGetFrame = true;
	GetParent()->PostMessage(UM_VIDEONOTIFY, UNM_GETVIDEOFRAME, 0);
	CSliderCtrl::OnMouseHover(nFlags, point);
}


void CMySlider::OnMouseLeave()
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值
//	GetParent()->PostMessage(UM_VIDEONOTIFY, UNM_STOPVIDEOCALLBACK, 0);
	CSliderCtrl::OnMouseLeave();
}
