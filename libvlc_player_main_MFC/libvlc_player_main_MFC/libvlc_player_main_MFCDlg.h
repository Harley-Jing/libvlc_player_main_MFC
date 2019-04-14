
// libvlc_player_main_MFCDlg.h : 头文件
//

#pragma once
#include <afxwin.h>
#include <string>//std::string
#include <afxcmn.h>

#include "configs.h"//libvlc
#include "MySlider.h"//
#include "CallbackParam.h" //取帧

static uint32_t IMG_WIDTH = 0;
static uint32_t IMG_HEIGHT = 0;

enum PlayerState
{
	STATE_PREPARE,
	STATE_PLAY,
	STATE_PAUSE
};

// Clibvlc_player_main_MFCDlg 对话框
class Clibvlc_player_main_MFCDlg : public CDialogEx
{
// 构造
public:
	Clibvlc_player_main_MFCDlg(CWnd* pParent = NULL);	// 标准构造函数
// 对话框数据
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_LIBVLC_PLAYER_MAIN_MFC_DIALOG };
#endif

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV 支持


// 实现
protected:
	HICON m_hIcon;

	// 生成的消息映射函数
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP();

public:
	//播放器
	PlayerState m_emplayerState;
	CStatic m_aWndVideo[4];	
	libvlc_instance_t *m_pVlcInstance;
	libvlc_media_player_t *m_apVlcPlayer[4];
	libvlc_media_player_t *m_pVlcHiddenPlayer[4];
	afx_msg void OnBnClickedBtnPlay();
	afx_msg void OnBnClickedBtnStop();

	//时间
	CEdit m_edtTime;
	int64_t m_anTotalTime[4];
	int m_nMaxTime;	//确定数组中视频最长时间的一员
	//进度条
	CMySlider m_slider;
	//取帧
	int m_nCount;
	volatile uint16_t m_ucFlag;
	//播放速率
	CSpinButtonCtrl m_spVideoRate;
	float m_fVideoRate;
	//拼接图片
	CImage m_aImg[5];
	void *m_apDest[5];
	//取帧
	CallbackParam *m_apCallbackParam[4];

	bool SetVideoPosition(int nSliderCurPos);
	afx_msg void OnDestroy();
	afx_msg void OnTimer(UINT_PTR nIDEvent);
	afx_msg void OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar);
	static void *lock(void *data, void **p_pixels);
	static void unlock(void *data, void *id, void *const *p_pixels);
    void OnDeltaposSpVideorate(NMHDR *pNMHDR, LRESULT *pResult);
	void StitchingPicture();
	//消息响应	 VideoNotify
	LRESULT VideoNotify(WPARAM wParam, LPARAM lParam);
	bool ShowVideoTime(void);
	bool Process(void);
	bool GetVideoFrame(void);
	bool StopVideoCallback(int ucID);
};
