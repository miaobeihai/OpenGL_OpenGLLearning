
// lesson008-MFC-Dialog-ES2.cpp : ����Ӧ�ó��������Ϊ��
//

#include "stdafx.h"
#include "lesson008-MFC-Dialog-ES2.h"
#include "lesson008-MFC-Dialog-ES2Dlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// Clesson008MFCDialogES2App

BEGIN_MESSAGE_MAP(Clesson008MFCDialogES2App, CWinAppEx)
	ON_COMMAND(ID_HELP, &CWinApp::OnHelp)
END_MESSAGE_MAP()


// Clesson008MFCDialogES2App ����

Clesson008MFCDialogES2App::Clesson008MFCDialogES2App()
{
	// TODO: �ڴ˴���ӹ�����룬
	// ��������Ҫ�ĳ�ʼ�������� InitInstance ��
}


// Ψһ��һ�� Clesson008MFCDialogES2App ����

Clesson008MFCDialogES2App theApp;


// Clesson008MFCDialogES2App ��ʼ��

BOOL Clesson008MFCDialogES2App::InitInstance()
{
	CWinAppEx::InitInstance();

	// ��׼��ʼ��
	// ���δʹ����Щ���ܲ�ϣ����С
	// ���տ�ִ���ļ��Ĵ�С����Ӧ�Ƴ�����
	// ����Ҫ���ض���ʼ������
	// �������ڴ洢���õ�ע�����
	// TODO: Ӧ�ʵ��޸ĸ��ַ�����
	// �����޸�Ϊ��˾����֯��
	SetRegistryKey(_T("Ӧ�ó��������ɵı���Ӧ�ó���"));

	Clesson008MFCDialogES2Dlg dlg;
	m_pMainWnd = &dlg;
	INT_PTR nResponse = dlg.DoModal();
	if (nResponse == IDOK)
	{
		// TODO: �ڴ˷��ô����ʱ��
		//  ��ȷ�������رնԻ���Ĵ���
	}
	else if (nResponse == IDCANCEL)
	{
		// TODO: �ڴ˷��ô����ʱ��
		//  ��ȡ�������رնԻ���Ĵ���
	}

	// ���ڶԻ����ѹرգ����Խ����� FALSE �Ա��˳�Ӧ�ó���
	//  ����������Ӧ�ó������Ϣ�á�
	return FALSE;
}
