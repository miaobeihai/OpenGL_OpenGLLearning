
// lesson008-MFC-Dialog-ES2Dlg.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "lesson008-MFC-Dialog-ES2.h"
#include "lesson008-MFC-Dialog-ES2Dlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// Clesson008MFCDialogES2Dlg �Ի���




Clesson008MFCDialogES2Dlg::Clesson008MFCDialogES2Dlg(CWnd* pParent /*=NULL*/)
	: CDialog(Clesson008MFCDialogES2Dlg::IDD, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void Clesson008MFCDialogES2Dlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(Clesson008MFCDialogES2Dlg, CDialog)
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	//}}AFX_MSG_MAP
    ON_WM_DESTROY()
    ON_WM_ERASEBKGND()
END_MESSAGE_MAP()


// Clesson008MFCDialogES2Dlg ��Ϣ�������

BOOL Clesson008MFCDialogES2Dlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	// ���ô˶Ի����ͼ�ꡣ��Ӧ�ó��������ڲ��ǶԻ���ʱ����ܽ��Զ�
	//  ִ�д˲���
	SetIcon(m_hIcon, TRUE);			// ���ô�ͼ��
	SetIcon(m_hIcon, FALSE);		// ����Сͼ��

    _context.setup(m_hWnd,::GetDC(m_hWnd));

	return TRUE;  // ���ǽ��������õ��ؼ������򷵻� TRUE
}

// �����Ի��������С����ť������Ҫ����Ĵ���
//  �����Ƹ�ͼ�ꡣ����ʹ���ĵ�/��ͼģ�͵� MFC Ӧ�ó���
//  �⽫�ɿ���Զ���ɡ�

void Clesson008MFCDialogES2Dlg::OnPaint()
{
    glClearColor(1,0,0,1);
    glClear(GL_COLOR_BUFFER_BIT);
    _context.swapBuffer();
}

//���û��϶���С������ʱϵͳ���ô˺���ȡ�ù��
//��ʾ��
HCURSOR Clesson008MFCDialogES2Dlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}


void Clesson008MFCDialogES2Dlg::OnDestroy()
{
    _context.shutdown();
    CDialog::OnDestroy();
}

BOOL Clesson008MFCDialogES2Dlg::OnEraseBkgnd(CDC* pDC)
{
   return   TRUE;
}
