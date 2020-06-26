
// lesson004-MFC-DialogDlg.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "lesson004-MFC-Dialog.h"
#include "lesson004-MFC-DialogDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// Clesson004MFCDialogDlg �Ի���




Clesson004MFCDialogDlg::Clesson004MFCDialogDlg(CWnd* pParent /*=NULL*/)
	: CDialog(Clesson004MFCDialogDlg::IDD, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void Clesson004MFCDialogDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(Clesson004MFCDialogDlg, CDialog)
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	//}}AFX_MSG_MAP
    ON_WM_DESTROY()
    ON_WM_ERASEBKGND()
END_MESSAGE_MAP()


// Clesson004MFCDialogDlg ��Ϣ�������

BOOL Clesson004MFCDialogDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	// ���ô˶Ի����ͼ�ꡣ��Ӧ�ó��������ڲ��ǶԻ���ʱ����ܽ��Զ�
	//  ִ�д˲���
	SetIcon(m_hIcon, TRUE);			// ���ô�ͼ��
	SetIcon(m_hIcon, FALSE);		// ����Сͼ��

    HDC     hDC =   ::GetDC(m_hWnd);
	_context.setup(m_hWnd,hDC);

	return TRUE;  // ���ǽ��������õ��ؼ������򷵻� TRUE
}

// �����Ի��������С����ť������Ҫ����Ĵ���
//  �����Ƹ�ͼ�ꡣ����ʹ���ĵ�/��ͼģ�͵� MFC Ӧ�ó���
//  �⽫�ɿ���Զ���ɡ�

void Clesson004MFCDialogDlg::OnPaint()
{
    glClearColor(1,0,0,1);
	glClear(GL_COLOR_BUFFER_BIT);
    _context.swapBuffer();
}

//���û��϶���С������ʱϵͳ���ô˺���ȡ�ù��
//��ʾ��
HCURSOR Clesson004MFCDialogDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}


void Clesson004MFCDialogDlg::OnDestroy()
{
    _context.shutdown();
    CDialog::OnDestroy();

}

BOOL Clesson004MFCDialogDlg::OnEraseBkgnd(CDC* pDC)
{
   return   TRUE;
}
