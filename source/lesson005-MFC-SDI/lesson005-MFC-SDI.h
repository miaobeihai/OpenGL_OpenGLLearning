
// lesson005-MFC-SDI.h : lesson005-MFC-SDI Ӧ�ó������ͷ�ļ�
//
#pragma once

#ifndef __AFXWIN_H__
	#error "�ڰ������ļ�֮ǰ������stdafx.h�������� PCH �ļ�"
#endif

#include "resource.h"       // ������


// Clesson005MFCSDIApp:
// �йش����ʵ�֣������ lesson005-MFC-SDI.cpp
//

class Clesson005MFCSDIApp : public CWinAppEx
{
public:
	Clesson005MFCSDIApp();


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

extern Clesson005MFCSDIApp theApp;
