
// libvlc_player_main_MFC.h : PROJECT_NAME Ӧ�ó������ͷ�ļ�
//

#pragma once

#ifndef __AFXWIN_H__
	#error "�ڰ������ļ�֮ǰ������stdafx.h�������� PCH �ļ�"
#endif

#include "resource.h"		// ������


// Clibvlc_player_main_MFCApp: 
// �йش����ʵ�֣������ libvlc_player_main_MFC.cpp
//

class Clibvlc_player_main_MFCApp : public CWinApp
{
public:
	Clibvlc_player_main_MFCApp();

// ��д
public:
	virtual BOOL InitInstance();

// ʵ��

	DECLARE_MESSAGE_MAP()
};

extern Clibvlc_player_main_MFCApp theApp;