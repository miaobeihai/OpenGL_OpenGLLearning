
// lesson004-MFC-DialogDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "lesson004-MFC-Dialog.h"
#include "lesson004-MFC-DialogDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// Clesson004MFCDialogDlg 对话框




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


// Clesson004MFCDialogDlg 消息处理程序

BOOL Clesson004MFCDialogDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	// 设置此对话框的图标。当应用程序主窗口不是对话框时，框架将自动
	//  执行此操作
	SetIcon(m_hIcon, TRUE);			// 设置大图标
	SetIcon(m_hIcon, FALSE);		// 设置小图标

    HDC     hDC =   ::GetDC(m_hWnd);
	_context.setup(m_hWnd,hDC);

	return TRUE;  // 除非将焦点设置到控件，否则返回 TRUE
}

// 如果向对话框添加最小化按钮，则需要下面的代码
//  来绘制该图标。对于使用文档/视图模型的 MFC 应用程序，
//  这将由框架自动完成。

void Clesson004MFCDialogDlg::OnPaint()
{
    glClearColor(1,0,0,1);
	glClear(GL_COLOR_BUFFER_BIT);
    _context.swapBuffer();
}

//当用户拖动最小化窗口时系统调用此函数取得光标
//显示。
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
