
// lesson004-MFC-Dialog.h : PROJECT_NAME Ӧ�ó������ͷ�ļ�
//

#pragma once

#ifndef __AFXWIN_H__
	#error "�ڰ������ļ�֮ǰ������stdafx.h�������� PCH �ļ�"
#endif

#include "resource.h"		// ������


// Clesson004MFCDialogApp:
// �йش����ʵ�֣������ lesson004-MFC-Dialog.cpp
//

class Clesson004MFCDialogApp : public CWinAppEx
{
public:
	Clesson004MFCDialogApp();

// ��д
	public:
	virtual BOOL InitInstance();

// ʵ��

	DECLARE_MESSAGE_MAP()
};

extern Clesson004MFCDialogApp theApp;