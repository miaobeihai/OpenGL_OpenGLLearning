
// lesson008-MFC-Dialog-ES2.h : PROJECT_NAME Ӧ�ó������ͷ�ļ�
//

#pragma once

#ifndef __AFXWIN_H__
	#error "�ڰ������ļ�֮ǰ������stdafx.h�������� PCH �ļ�"
#endif

#include "resource.h"		// ������


// Clesson008MFCDialogES2App:
// �йش����ʵ�֣������ lesson008-MFC-Dialog-ES2.cpp
//

class Clesson008MFCDialogES2App : public CWinAppEx
{
public:
	Clesson008MFCDialogES2App();

// ��д
	public:
	virtual BOOL InitInstance();

// ʵ��

	DECLARE_MESSAGE_MAP()
};

extern Clesson008MFCDialogES2App theApp;