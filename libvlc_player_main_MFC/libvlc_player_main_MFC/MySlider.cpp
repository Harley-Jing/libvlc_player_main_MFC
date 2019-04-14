// MySlider.cpp : ʵ���ļ�
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


// CMySlider ��Ϣ�������

void CMySlider::OnLButtonDown(UINT nFlags, CPoint point)
{
	// TODO: �ڴ������Ϣ�����������/�����Ĭ��ֵ
	SendMessage(WM_KILLFOCUS, -1, 0); //ʧȥ����
	this->EnableWindow(FALSE);//���ò�����

	CRect rectChannel, rectThumb;
	GetChannelRect(rectChannel);//��ȡ�����������������������
	GetThumbRect(rectThumb);//��ȡ����������������������

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
	this->EnableWindow(TRUE);//���ÿ���

	this->SetFocus();//��ý���
	CSliderCtrl::OnLButtonDown(nFlags, point);
}


void CMySlider::OnMouseMove(UINT nFlags, CPoint point)
{
	// TODO: �ڴ������Ϣ�����������/�����Ĭ��ֵ
	TRACKMOUSEEVENT tme;
	tme.cbSize = sizeof(tme);	//�ṹ�建������С
	tme.dwFlags = TME_HOVER;	//ע��WM_MOUSEHOVER��Ϣ
	tme.dwHoverTime = 1200;		//WM_MOUSEHOVER��Ϣ�������ʱ��	  1.2s
	tme.hwndTrack = m_hWnd;		//�����ھ��

	::TrackMouseEvent(&tme);	//ע�ᷢ����Ϣ
	CSliderCtrl::OnMouseMove(nFlags, point);
}


void CMySlider::OnMouseHover(UINT nFlags, CPoint point)
{
	// TODO: �ڴ������Ϣ�����������/�����Ĭ��ֵ
	//��ȡ�����ͣλ��
	CRect rectChannel, rectThumb;
	GetChannelRect(rectChannel);//��ȡ�����������������������
	GetThumbRect(rectThumb);//��ȡ����������������������

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
	// TODO: �ڴ������Ϣ�����������/�����Ĭ��ֵ
//	GetParent()->PostMessage(UM_VIDEONOTIFY, UNM_STOPVIDEOCALLBACK, 0);
	CSliderCtrl::OnMouseLeave();
}
