
// lesson006-MFC-MDI.h : lesson006-MFC-MDI Ӧ�ó������ͷ�ļ�
//
#pragma once

#ifndef __AFXWIN_H__
	#error "�ڰ������ļ�֮ǰ������stdafx.h�������� PCH �ļ�"
#endif

#include "resource.h"       // ������


// Clesson006MFCMDIApp:
// �йش����ʵ�֣������ lesson006-MFC-MDI.cpp
//

class Clesson006MFCMDIApp : public CWinAppEx
{
public:
	Clesson006MFCMDIApp();


// ��д
public:
	virtual BOOL InitInstance();

// ʵ��
	UINT  m_nAppLook;
	BOOL  m_bHiColorIcons;

	virtual void PreLoadState();
	virtual void LoadCustomState();
	virtual void SaveCustomState();

	afx_msg void OnAppAbout();
	DECLARE_MESSAGE_MAP()
};

extern Clesson006MFCMDIApp theApp;
