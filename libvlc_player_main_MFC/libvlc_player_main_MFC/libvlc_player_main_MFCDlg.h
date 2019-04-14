
// libvlc_player_main_MFCDlg.h : ͷ�ļ�
//

#pragma once
#include <afxwin.h>
#include <string>//std::string
#include <afxcmn.h>

#include "configs.h"//libvlc
#include "MySlider.h"//
#include "CallbackParam.h" //ȡ֡

static uint32_t IMG_WIDTH = 0;
static uint32_t IMG_HEIGHT = 0;

enum PlayerState
{
	STATE_PREPARE,
	STATE_PLAY,
	STATE_PAUSE
};

// Clibvlc_player_main_MFCDlg �Ի���
class Clibvlc_player_main_MFCDlg : public CDialogEx
{
// ����
public:
	Clibvlc_player_main_MFCDlg(CWnd* pParent = NULL);	// ��׼���캯��
// �Ի�������
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_LIBVLC_PLAYER_MAIN_MFC_DIALOG };
#endif

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV ֧��


// ʵ��
protected:
	HICON m_hIcon;

	// ���ɵ���Ϣӳ�亯��
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP();

public:
	//������
	PlayerState m_emplayerState;
	CStatic m_aWndVideo[4];	
	libvlc_instance_t *m_pVlcInstance;
	libvlc_media_player_t *m_apVlcPlayer[4];
	libvlc_media_player_t *m_pVlcHiddenPlayer[4];
	afx_msg void OnBnClickedBtnPlay();
	afx_msg void OnBnClickedBtnStop();

	//ʱ��
	CEdit m_edtTime;
	int64_t m_anTotalTime[4];
	int m_nMaxTime;	//ȷ����������Ƶ�ʱ���һԱ
	//������
	CMySlider m_slider;
	//ȡ֡
	int m_nCount;
	volatile uint16_t m_ucFlag;
	//��������
	CSpinButtonCtrl m_spVideoRate;
	float m_fVideoRate;
	//ƴ��ͼƬ
	CImage m_aImg[5];
	void *m_apDest[5];
	//ȡ֡
	CallbackParam *m_apCallbackParam[4];

	bool SetVideoPosition(int nSliderCurPos);
	afx_msg void OnDestroy();
	afx_msg void OnTimer(UINT_PTR nIDEvent);
	afx_msg void OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar);
	static void *lock(void *data, void **p_pixels);
	static void unlock(void *data, void *id, void *const *p_pixels);
    void OnDeltaposSpVideorate(NMHDR *pNMHDR, LRESULT *pResult);
	void StitchingPicture();
	//��Ϣ��Ӧ	 VideoNotify
	LRESULT VideoNotify(WPARAM wParam, LPARAM lParam);
	bool ShowVideoTime(void);
	bool Process(void);
	bool GetVideoFrame(void);
	bool StopVideoCallback(int ucID);
};
