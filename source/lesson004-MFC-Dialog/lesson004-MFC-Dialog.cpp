
// lesson004-MFC-Dialog.cpp : ����Ӧ�ó��������Ϊ��
//

#include "stdafx.h"
#include "lesson004-MFC-Dialog.h"
#include "lesson004-MFC-DialogDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// Clesson004MFCDialogApp

BEGIN_MESSAGE_MAP(Clesson004MFCDialogApp, CWinAppEx)
	ON_COMMAND(ID_HELP, &CWinApp::OnHelp)
END_MESSAGE_MAP()


// Clesson004MFCDialogApp ����

Clesson004MFCDialogApp::Clesson004MFCDialogApp()
{
	// TODO: �ڴ˴���ӹ�����룬
	// ��������Ҫ�ĳ�ʼ�������� InitInstance ��
}


// Ψһ��һ�� Clesson004MFCDialogApp ����

Clesson004MFCDialogApp theApp;


// Clesson004MFCDialogApp ��ʼ��

BOOL Clesson004MFCDialogApp::InitInstance()
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

	Clesson004MFCDialogDlg dlg;
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
