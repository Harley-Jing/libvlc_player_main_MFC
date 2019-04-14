
// libvlc_player_main_MFCDlg.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "libvlc_player_main_MFC.h"
#include "libvlc_player_main_MFCDlg.h"
#include "afxdialogex.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// ����Ӧ�ó��򡰹��ڡ��˵���� CAboutDlg �Ի���

class CAboutDlg : public CDialogEx
{
public:
	CAboutDlg();

// �Ի�������
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_ABOUTBOX };
#endif

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

// ʵ��
protected:
	DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() : CDialogEx(IDD_ABOUTBOX)
{
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialogEx)
END_MESSAGE_MAP()


// Clibvlc_player_main_MFCDlg �Ի���



Clibvlc_player_main_MFCDlg::Clibvlc_player_main_MFCDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(IDD_LIBVLC_PLAYER_MAIN_MFC_DIALOG, pParent)
	, m_pVlcInstance(NULL)	 
	, m_nMaxTime(0)
	, m_fVideoRate(1.0f)
	, m_nCount(0)
	, m_ucFlag(0)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void Clibvlc_player_main_MFCDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	for (int i = 0; i < 4; ++i)
	{
		DDX_Control(pDX, IDC_SCRN_UPPERLEFT + i, m_aWndVideo[i]);
	}
	DDX_Control(pDX, IDC_EDIT_TIME, m_edtTime);
	DDX_Control(pDX, IDC_SLIDER, m_slider);
	DDX_Control(pDX, IDC_SP_VIDEORATE, m_spVideoRate);
}

BEGIN_MESSAGE_MAP(Clibvlc_player_main_MFCDlg, CDialogEx)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_WM_DESTROY()
	ON_BN_CLICKED(IDC_BTN_PLAY, &Clibvlc_player_main_MFCDlg::OnBnClickedBtnPlay)
	ON_BN_CLICKED(IDC_BTN_STOP, &Clibvlc_player_main_MFCDlg::OnBnClickedBtnStop)
	ON_WM_TIMER()
	ON_MESSAGE(UM_VIDEONOTIFY, &Clibvlc_player_main_MFCDlg::VideoNotify)	 //my
	ON_WM_HSCROLL()
	ON_NOTIFY(UDN_DELTAPOS, IDC_SP_VIDEORATE, &Clibvlc_player_main_MFCDlg::OnDeltaposSpVideorate)
END_MESSAGE_MAP()


// Clibvlc_player_main_MFCDlg ��Ϣ�������
BOOL Clibvlc_player_main_MFCDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// ��������...���˵�����ӵ�ϵͳ�˵��С�

	// IDM_ABOUTBOX ������ϵͳ���Χ�ڡ�
	ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
	ASSERT(IDM_ABOUTBOX < 0xF000);

	CMenu* pSysMenu = GetSystemMenu(FALSE);
	if (pSysMenu != NULL)
	{
		BOOL bNameValid;
		CString strAboutMenu;
		bNameValid = strAboutMenu.LoadString(IDS_ABOUTBOX);
		ASSERT(bNameValid);
		if (!strAboutMenu.IsEmpty())
		{
			pSysMenu->AppendMenu(MF_SEPARATOR);
			pSysMenu->AppendMenu(MF_STRING, IDM_ABOUTBOX, strAboutMenu);
		}
	}

	// ���ô˶Ի����ͼ�ꡣ  ��Ӧ�ó��������ڲ��ǶԻ���ʱ����ܽ��Զ�
	//  ִ�д˲���
	SetIcon(m_hIcon, TRUE);			// ���ô�ͼ��
	SetIcon(m_hIcon, FALSE);		// ����Сͼ��

/************************************************************************************/
/*																					*/
/*                     TODO: �ڴ���Ӷ���ĳ�ʼ������									*/	
/*																				    */
/************************************************************************************/
	//�����豸�ĳ�ʼ��
	m_pVlcInstance = libvlc_new(0, NULL);
 	memset(m_apVlcPlayer, NULL, sizeof(m_apVlcPlayer));
	memset(m_pVlcHiddenPlayer, NULL, sizeof(m_pVlcHiddenPlayer));
	m_emplayerState = STATE_PREPARE;
	//ʱ��
	memset(m_anTotalTime, 0, sizeof(m_anTotalTime));
	m_edtTime.SetWindowText(_T("00:00:00 / 00:00:00"));	
	//��������ʼ��
	m_slider.SetRange(0, 100);
	m_slider.SetTicFreq(10);//������ʾ�̶ȵļ��
	m_slider.SetPos(0);
	m_slider.SetPageSize(10);  	 //һҳ�Ĵ�С����Ӧ����PGUP PGDN
	m_slider.SetLineSize(3);	//һ�еĴ�С����Ӧ���̵ķ����
	m_slider.EnableWindow(FALSE);
	//video rate
	CString strText;
	strText.Format(_T("%.1fx"), m_fVideoRate);
	GetDlgItem(IDC_EDT_VIDEORATE)->SetWindowText(strText);
	//����ͼ
	memset(m_apCallbackParam, NULL, sizeof(m_apCallbackParam) );
	memset(m_apDest, NULL, sizeof(m_apDest));

	return TRUE;  // ���ǽ��������õ��ؼ������򷵻� TRUE
}

void Clibvlc_player_main_MFCDlg::OnSysCommand(UINT nID, LPARAM lParam)
{
	if ((nID & 0xFFF0) == IDM_ABOUTBOX)
	{
		CAboutDlg dlgAbout;
		dlgAbout.DoModal();
	}
	else
	{
		CDialogEx::OnSysCommand(nID, lParam);
	}
}

// �����Ի��������С����ť������Ҫ����Ĵ���
//  �����Ƹ�ͼ�ꡣ  ����ʹ���ĵ�/��ͼģ�͵� MFC Ӧ�ó���
//  �⽫�ɿ���Զ���ɡ�

void Clibvlc_player_main_MFCDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // ���ڻ��Ƶ��豸������

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// ʹͼ���ڹ����������о���
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// ����ͼ��
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialogEx::OnPaint();
	}
}

//���û��϶���С������ʱϵͳ���ô˺���ȡ�ù��
//��ʾ��
HCURSOR Clibvlc_player_main_MFCDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}

/*********************************************************************/
void Clibvlc_player_main_MFCDlg::OnBnClickedBtnPlay()			 
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	if (m_emplayerState == STATE_PREPARE)
	{
		m_slider.EnableWindow(TRUE);
		m_emplayerState = STATE_PLAY;
		GetDlgItem(IDC_BTN_PLAY)->SetWindowText(_T("��ͣ"));

		libvlc_media_t *apVlcPath = NULL;
		std::string astrUrlPath;

		for (int i = 0; i < 4; ++i)
		{
			astrUrlPath = "D:\\cpp\\video\\video" + std::to_string(i + 1) + ".mp4";
			//1.Create a media player item	
			//1.1���������ַ
			//m_pVlcPath = libvlc_media_new_location(m_apVlcInstance[i], astrUrlPath[i]);  
			//1.2���ű�����Ƶ�ļ�
			apVlcPath = libvlc_media_new_path(m_pVlcInstance, (const char*)astrUrlPath.c_str());	
			//2.Create a media player playing environment
			m_apVlcPlayer[i] = libvlc_media_player_new_from_media(apVlcPath);
			m_pVlcHiddenPlayer[i] = libvlc_media_player_new_from_media(apVlcPath);
			//3.No need to keep the media player item now
			libvlc_media_release(apVlcPath);
			//4.on windows 
			libvlc_media_player_set_hwnd(m_apVlcPlayer[i], m_aWndVideo[i].m_hWnd);
			//mute
			libvlc_audio_set_mute(m_apVlcPlayer[i], true);
			libvlc_audio_set_mute(m_pVlcHiddenPlayer[i], true);
			//5.play the media_player	
			libvlc_media_player_play(m_apVlcPlayer[i]);	
		}

		//������ʱ�� 1s
		SetTimer(1000, 1000, NULL);
		PostMessage(UM_VIDEONOTIFY, UNM_PROCESS, 0);
	}
	else if (m_emplayerState == STATE_PLAY)
	{
		for (int i = 0; i < 4; ++i)
		{
			libvlc_media_player_set_pause(m_apVlcPlayer[i], 1);//��ͣ
		}
		m_emplayerState = STATE_PAUSE;
		GetDlgItem(IDC_BTN_PLAY)->SetWindowText(_T("����"));
	}
	else if (m_emplayerState == STATE_PAUSE)
	{
		for (int i = 0; i < 4; ++i)
		{
			libvlc_media_player_set_pause(m_apVlcPlayer[i], 0);//����
		}
		m_emplayerState = STATE_PLAY;
		GetDlgItem(IDC_BTN_PLAY)->SetWindowText(_T("��ͣ"));
	}

} 

//NO BUG
void Clibvlc_player_main_MFCDlg::OnBnClickedBtnStop()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	if (m_emplayerState != STATE_PREPARE)
	{
		GetDlgItem(IDC_BTN_PLAY)->SetWindowText(_T("����"));
		for (int i = 0; i < 4; ++i)
		{
			//Stop playing
			libvlc_media_player_stop(m_apVlcPlayer[i]);
			libvlc_media_player_stop(m_pVlcHiddenPlayer[i]);
		}
	}
	//�رն�ʱ��
	KillTimer(1000);

	m_edtTime.SetWindowText(_T("00:00:00 / 00:00:00"));
	m_slider.SetPos(0);
	m_slider.EnableWindow(FALSE);
	m_emplayerState = STATE_PREPARE;
}

void Clibvlc_player_main_MFCDlg::OnTimer(UINT_PTR nIDEvent)
{
	// TODO: �ڴ������Ϣ�����������/�����Ĭ��ֵ
	//����ϵͳ������ ����	
	//MessageBeep(0xFFFFFFFF);	
	if (nIDEvent == 1000)
	{
		PostMessage(UM_VIDEONOTIFY, UNM_SHOWVIDEOTIME, 0);
	}

	CDialogEx::OnTimer(nIDEvent);
}

//NO BUG
bool Clibvlc_player_main_MFCDlg::Process(void)
{	 
	int64_t nMaxTime = 0;
	for (int i = 0; i < 4; ++i)
	{
		//ms
		m_anTotalTime[i] = (int)libvlc_media_player_get_length(m_apVlcPlayer[i]);
		if ( nMaxTime < m_anTotalTime[i])
		{
			nMaxTime = m_anTotalTime[i];
			m_nMaxTime = i;
		}

		libvlc_video_get_size(m_apVlcPlayer[i], 0, &IMG_WIDTH, &IMG_HEIGHT);
		m_apCallbackParam[i] = new CallbackParam(i, IMG_WIDTH, IMG_HEIGHT, this);
	}

	return true;
}

//NO BUG
bool Clibvlc_player_main_MFCDlg::ShowVideoTime()
{
	if (libvlc_media_player_get_state(m_apVlcPlayer[m_nMaxTime]) == libvlc_Ended)
	{
		OnBnClickedBtnStop();
		return true;
	}

	for (int i = 0; i < 4; ++i)
	{
		if (libvlc_media_player_get_state(m_apVlcPlayer[i]) == libvlc_Ended)
		{
			libvlc_media_player_stop(m_apVlcPlayer[i]);
		}
	}

	CString strTotalTime;
	int64_t nCurTime = 0, nDuration = 0;
	int64_t nCurTotalSec = 0, nCurHour = 0, nCurMin = 0, nCurSec = 0;
	int64_t nDurTotalSec = 0, nDurHour = 0, nDurMin = 0, nDurSec = 0;

	//ms
	nCurTime = libvlc_media_player_get_time(m_apVlcPlayer[m_nMaxTime]);
	if (nCurTime != 0)
	{
		//change to second
		nCurTotalSec = nCurTime / 1000;
		nCurHour = nCurTotalSec / 3600;
		nCurMin = (nCurTotalSec % 3600) / 60;
		nCurSec = (nCurTotalSec % 60);
	}

	if (nDurSec == 0)
	{
		//ms
		nDuration = m_anTotalTime[m_nMaxTime];
		if (nDuration != 0)
		{
			//change to second
			nDurTotalSec = nDuration / 1000;
			nDurHour = nDurTotalSec / 3600;
			nDurMin = (nDurTotalSec % 3600) / 60;
			nDurSec = (nDurTotalSec % 60);
		}
 	}

	int sliderCurPos = 0;	   
	sliderCurPos = (nCurTime * 100) / nDuration;
	m_slider.SetPos(sliderCurPos);

	strTotalTime.Format(_T("%02d:%02d:%02d / %02d:%02d:%02d"),
		nCurHour, nCurMin, nCurSec, nDurHour, nDurMin, nDurSec);
	m_edtTime.SetWindowText(strTotalTime);

	return true;
}

//����������
void Clibvlc_player_main_MFCDlg::OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar)
{
	// TODO: �ڴ������Ϣ�����������/�����Ĭ��ֵ
	if ( m_emplayerState != STATE_PREPARE )
	{
		int nCurPos = m_slider.GetPos();
		// Determine the new position of scroll box.
		switch ( nSBCode )
		{
			case SB_LEFT:      // Scroll to far left.
				nCurPos = 0;
				break;

			case SB_RIGHT:      // Scroll to far right.
				nCurPos = 100;
				break;

			case SB_ENDSCROLL:   // End scroll.
				break;

			case SB_LINELEFT:      // Scroll left.
				if ( nCurPos > 0 )
					nCurPos--;
				break;

			case SB_LINERIGHT:   // Scroll right.
			if ( nCurPos < 100 )
				nCurPos++;
				break;

			//	Scrolls left by the width of the window
			case SB_PAGELEFT:    // Scroll one page left.
				nCurPos -= 2;
				break;
		
			//Scrolls right by the width of the window.
			case SB_PAGERIGHT:      // Scroll one page right. 
				nCurPos += 2;
				break;

			//The user has dragged the scroll box (thumb) and released the mouse button.
			// Scroll to absolute position. nPos is the position of the scroll box at the end of the drag operation.
			case SB_THUMBPOSITION: 
				nCurPos = nPos;    
				break;

			//The user is dragging the scroll box. This message is sent repeatedly until the user releases the mouse button
			// Drag scroll box to specified position. nPos is the position that the scroll box has been dragged to.
			case SB_THUMBTRACK:   
				nCurPos = nPos;  
				break;
		}

		if ( nCurPos > 100 )
		{
			nCurPos = 100;
		}
		else if ( nCurPos < 0 )
		{
			nCurPos = 0;
		}

		m_slider.SetPos(nCurPos);

		SetVideoPosition(nCurPos);
	}
	 
	CDialogEx::OnHScroll(nSBCode, nPos, pScrollBar);
}

bool Clibvlc_player_main_MFCDlg::SetVideoPosition(int nSliderCurPos)
{
	int nCurPos = nSliderCurPos;
	float fPos = 0.0;//�ļ�����λ�ã���Χ0.0-1.0

	if (m_emplayerState != STATE_PREPARE)
	{	
		for ( int i = 0; i < 4; ++i )
		{
			//��ȡ��Ӧ��������Χ
			float fMaxTime = (float)m_anTotalTime[m_nMaxTime];
			float fRange = (float)((m_anTotalTime[i] * 100) / fMaxTime);

			if (fRange < 0.0001f)
			{
				AfxMessageBox(_T("fRange error"));
				return false;
			}

			if (nCurPos >= (int)fRange)
			{
				libvlc_media_player_set_position(m_apVlcPlayer[i], 0.9985f);//�����ļ�����λ��
			}
			else
			{
				fPos = (float)(nCurPos / fRange);
				if (fPos >= 0.9985f)
				{
					fPos = 0.9985f;
				}

				if (libvlc_media_player_get_state(m_apVlcPlayer[i]) == libvlc_Stopped)
				{
					libvlc_media_player_play(m_apVlcPlayer[i]);
				}
				libvlc_media_player_set_position(m_apVlcPlayer[i], fPos);//�����ļ�����λ��
			}
		}
	}

	return true;
}

//��Ƶ�������ʿ���
void Clibvlc_player_main_MFCDlg::OnDeltaposSpVideorate(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMUPDOWN pNMUpDown = reinterpret_cast<LPNMUPDOWN>(pNMHDR);

	if (m_emplayerState == STATE_PLAY)
	{
		if (pNMUpDown->iDelta < 0)
		{
			if (m_fVideoRate < 1.0f)
				m_fVideoRate += 0.1f;
			else
				m_fVideoRate += 1.0f;
			if (m_fVideoRate >= 10.0f)
				m_fVideoRate = 10.0f;
		}
		else
		{
			if (m_fVideoRate <= 1.0f)
				m_fVideoRate -= 0.1f;
			else
				m_fVideoRate -= 1.0f;
			if (m_fVideoRate <= 0.1f)
				m_fVideoRate = 0.1f;
		}
		CString strText;
		strText.Format(_T("%.1fx"), m_fVideoRate);
		GetDlgItem(IDC_EDT_VIDEORATE)->SetWindowText(strText);
		for (int i = 0; i < 4; ++i)
		{
			libvlc_media_player_set_rate(m_apVlcPlayer[i], m_fVideoRate);
		}
	}

	*pResult = 0;
}

void *Clibvlc_player_main_MFCDlg::lock(void *data, void **p_pixels)
{
	CallbackParam *pCallback = (CallbackParam *)data;
	if ( NULL == pCallback->m_buf)
	{
		return NULL;
	}
	pCallback->m_mutex.Lock();
	*p_pixels = pCallback->m_buf;  /*tell VLC to put decoded data to this buffer*/
 	return NULL;
}

void Clibvlc_player_main_MFCDlg::unlock(void *data, void *id, void *const *p_pixels)
{
	CallbackParam *pCallback = (CallbackParam *)data;
	Clibvlc_player_main_MFCDlg *pMain = (Clibvlc_player_main_MFCDlg*)pCallback->m_pParam;
	
	int ucID = pCallback->m_ucID;
	if ((pMain->m_ucFlag & 0x8000)
		&& (pMain->m_ucFlag & (1 << ucID)) == 0)
	{
// 		int nWidth = IMG_WIDTH;
// 		int nHeight = IMG_HEIGHT;
// 		CImage img;
// 		if (img.IsNull())
// 		{
// 			img.Create(nWidth, -nHeight, 32, 0);
// 		}
// 		//����cimage��  
// 		void *pDst = img.GetBits();
// 		memcpy(pDst, *p_pixels, IMG_WIDTH * IMG_HEIGHT * 4);
// 
// 
// 		CString strfilter;
// 		strfilter.Format(_T("D:\\cpp\\image\\bmp%d.bmp"), ucID);
// 		img.Save((LPCTSTR)strfilter);
// 		img.Destroy();

		pMain->m_ucFlag |= 1 << ucID;

		printf("unlock %d \n", ucID);

		if ((pMain->m_ucFlag & 0x0F) == 0x0F)
		{
			pMain->m_ucFlag = 0;
			pMain->PostMessage(UM_VIDEONOTIFY, UNM_STOPVIDEOCALLBACK, ucID);
			
		}
	}

	pCallback->m_mutex.Unlock();
}

bool Clibvlc_player_main_MFCDlg::GetVideoFrame(void)
{
	if (m_slider.m_bisOkGetFrame)
	{
		m_ucFlag = 0;
		for (int i = 0; i < 4; ++i)
		{
			//frame
			libvlc_video_set_callbacks(m_pVlcHiddenPlayer[i], lock, unlock, NULL, m_apCallbackParam[i]);
			libvlc_video_set_format(m_pVlcHiddenPlayer[i], "BGRA",
				IMG_WIDTH, IMG_HEIGHT, IMG_WIDTH * 4);
			libvlc_media_player_play(m_pVlcHiddenPlayer[i]);
			libvlc_media_player_set_pause(m_pVlcHiddenPlayer[i], 1);//��ͣ

			printf("start callback [%d] \n", i);
		}

		int nCurPos = m_slider.m_mouseHoverPos;
		float fPos = 0.0;//�ļ�����λ�ã���Χ0.0-1.0

		if (m_anTotalTime[m_nMaxTime] != 0)
		{
			
			for (int i = 0; i < 4; ++i)
			{
				//��ȡ��Ӧ��������Χ
				float fMaxTime = (float)m_anTotalTime[m_nMaxTime];
				float fRange = (float)((m_anTotalTime[i] * 100) / fMaxTime);

				if (fRange < 0.0001f)
				{
					AfxMessageBox(_T("HiddenPlayer slider fRange error"));
					return false;
				}

				if (nCurPos >= (int)fRange)
				{
					libvlc_media_player_set_position(m_pVlcHiddenPlayer[i], 0.9985f);//�����ļ�����λ��
				}
				else
				{
					fPos = (float)(nCurPos / fRange);
					if (fPos >= 0.9985f)
					{
						fPos = 0.9985f;
					}

					if (libvlc_media_player_get_state(m_pVlcHiddenPlayer[i]) == libvlc_Stopped)
					{
						libvlc_media_player_play(m_pVlcHiddenPlayer[i]);
					}
					libvlc_media_player_set_position(m_pVlcHiddenPlayer[i], fPos);//�����ļ�����λ��
				}
			}
		}

		m_ucFlag |= 0x8000;
	}

	return true;
}


void Clibvlc_player_main_MFCDlg::OnDestroy()
{
	
	// TODO: �ڴ˴������Ϣ����������
	CDialogEx::OnDestroy();
	if (m_apVlcPlayer[0] != NULL)
	{
		for (int i = 0; i < 4; ++i)
		{
			if (m_apVlcPlayer[i] != NULL)
			{
				libvlc_media_player_release(m_apVlcPlayer[i]);
				m_apVlcPlayer[i] = NULL;
				libvlc_media_player_release(m_pVlcHiddenPlayer[i]);
				m_pVlcHiddenPlayer[i] = NULL;
			}
		}
	}

	if (m_pVlcInstance != NULL)
	{
		libvlc_release(m_pVlcInstance);
		m_pVlcInstance = NULL;
	}

	if (m_apCallbackParam[0] != NULL)
	{
		for (int i = 0; i < 4; ++i)
		{	
			if (m_apCallbackParam[i] != NULL)
			{
				delete m_apCallbackParam[i];
				m_apCallbackParam[i] = NULL;
			}
		
		}
	}

}

bool Clibvlc_player_main_MFCDlg::StopVideoCallback(int ucID)
{
	Sleep(50);
	for (int i = 0; i < 4; ++i)
	{
		
		libvlc_media_player_stop(m_pVlcHiddenPlayer[i]);

		printf("stop HiddenPlayer %d\n", i);
	}	
	
		StitchingPicture();

	return true;
}

void Clibvlc_player_main_MFCDlg::StitchingPicture()
{

	for (int i = 0; i < 4; ++i)
	{
		if (m_aImg[i].IsNull())
		{
			m_aImg[i].Create(IMG_WIDTH, -(int)IMG_HEIGHT, 32, 0);
		}
		m_apDest[i] = m_aImg[i].GetBits();
		memcpy(m_apDest[i], m_apCallbackParam[i]->m_buf, IMG_WIDTH * IMG_HEIGHT * 4);
	}

	if (m_aImg[4].IsNull())
	{
		m_aImg[4].Create(IMG_WIDTH, -(int)IMG_HEIGHT, 32, 0);
	}

	::SetStretchBltMode(m_aImg[4].GetDC(), STRETCH_HALFTONE);

	m_aImg[0].StretchBlt(m_aImg[4].GetDC(),
		CRect(0, 0, m_aImg[4].GetWidth() / 2, m_aImg[4].GetHeight() / 2),
		CRect(0, 0, m_aImg[0].GetWidth(), m_aImg[0].GetHeight()));

	m_aImg[1].StretchBlt(m_aImg[4].GetDC(),
		CRect(m_aImg[4].GetWidth() / 2, 0, m_aImg[4].GetWidth(), m_aImg[4].GetHeight() / 2),
		CRect(0, 0, m_aImg[1].GetWidth(), m_aImg[1].GetHeight()));
	m_aImg[2].StretchBlt(m_aImg[4].GetDC(),
		CRect(0, m_aImg[4].GetHeight() / 2, m_aImg[4].GetWidth() / 2, m_aImg[4].GetHeight()),
		CRect(0, 0, m_aImg[2].GetWidth(), m_aImg[2].GetHeight()));
	m_aImg[3].StretchBlt(m_aImg[4].GetDC(),
		CRect(m_aImg[4].GetWidth() / 2, m_aImg[4].GetHeight() / 2, m_aImg[4].GetWidth(), m_aImg[4].GetHeight()),
		CRect(0, 0, m_aImg[3].GetWidth(), m_aImg[3].GetHeight()));

	m_aImg[4].Save(_T("D:\\cpp\\image\\new1.bmp"));

	for (int i = 0; i < 5; ++i)
	{
		if (m_aImg[i].IsNull())
		{
			m_aImg[i].Destroy();
		}

	}

	printf("Stitching  Picture \n");
}


LRESULT Clibvlc_player_main_MFCDlg::VideoNotify(WPARAM wParam, LPARAM lParam)
{
	switch (wParam)
	{
	case UNM_PROCESS:
		Process();
		break;
	case UNM_SHOWVIDEOTIME:
		ShowVideoTime();
		break;
	case UNM_GETVIDEOFRAME:
		GetVideoFrame();
		break;
	case UNM_STOPVIDEOCALLBACK:
		StopVideoCallback((int)lParam);
		break;
	default:
		break;
	}
	return 0;
}