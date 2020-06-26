
// lesson006-MFC-MDIView.cpp : Clesson006MFCMDIView 类的实现
//

#include "stdafx.h"
#include "lesson006-MFC-MDI.h"

#include "lesson006-MFC-MDIDoc.h"
#include "lesson006-MFC-MDIView.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// Clesson006MFCMDIView

IMPLEMENT_DYNCREATE(Clesson006MFCMDIView, CView)

BEGIN_MESSAGE_MAP(Clesson006MFCMDIView, CView)
	// 标准打印命令
	ON_COMMAND(ID_FILE_PRINT, &CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_DIRECT, &CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_PREVIEW, &Clesson006MFCMDIView::OnFilePrintPreview)
    ON_WM_DESTROY()
    ON_WM_ERASEBKGND()
END_MESSAGE_MAP()

// Clesson006MFCMDIView 构造/析构

Clesson006MFCMDIView::Clesson006MFCMDIView()
{
	// TODO: 在此处添加构造代码

}

Clesson006MFCMDIView::~Clesson006MFCMDIView()
{
}

BOOL Clesson006MFCMDIView::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: 在此处通过修改
	//  CREATESTRUCT cs 来修改窗口类或样式

	return CView::PreCreateWindow(cs);
}

// Clesson006MFCMDIView 绘制

int     g_index =   0;
void Clesson006MFCMDIView::OnDraw(CDC* /*pDC*/)
{
	Clesson006MFCMDIDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	if (!pDoc)
		return;

    _context.makeCurrent();
    if (_index == 0)
    {
        glClearColor(1,0,0,1);
    }
    else
    {
        glClearColor(0,1,0,1);
    }
    
    glClear(GL_COLOR_BUFFER_BIT);
    _context.swapBuffer();

	// TODO: 在此处为本机数据添加绘制代码
}


// Clesson006MFCMDIView 打印


void Clesson006MFCMDIView::OnFilePrintPreview()
{
	AFXPrintPreview(this);
}

BOOL Clesson006MFCMDIView::OnPreparePrinting(CPrintInfo* pInfo)
{
	// 默认准备
	return DoPreparePrinting(pInfo);
}

void Clesson006MFCMDIView::OnBeginPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: 添加额外的打印前进行的初始化过程
}

void Clesson006MFCMDIView::OnEndPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: 添加打印后进行的清理过程
}

void Clesson006MFCMDIView::OnRButtonUp(UINT nFlags, CPoint point)
{
	ClientToScreen(&point);
	OnContextMenu(this, point);
}

void Clesson006MFCMDIView::OnContextMenu(CWnd* pWnd, CPoint point)
{
	theApp.GetContextMenuManager()->ShowPopupMenu(IDR_POPUP_EDIT, point.x, point.y, this, TRUE);
}


// Clesson006MFCMDIView 诊断

#ifdef _DEBUG
void Clesson006MFCMDIView::AssertValid() const
{
	CView::AssertValid();
}

void Clesson006MFCMDIView::Dump(CDumpContext& dc) const
{
	CView::Dump(dc);
}

Clesson006MFCMDIDoc* Clesson006MFCMDIView::GetDocument() const // 非调试版本是内联的
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(Clesson006MFCMDIDoc)));
	return (Clesson006MFCMDIDoc*)m_pDocument;
}
#endif //_DEBUG


// Clesson006MFCMDIView 消息处理程序

void Clesson006MFCMDIView::OnInitialUpdate()
{
    CView::OnInitialUpdate();

    _index  =   g_index;
    ++g_index;
    _context.setup(m_hWnd,::GetDC(m_hWnd));
}

void Clesson006MFCMDIView::OnDestroy()
{
    _context.shutdown();
    CView::OnDestroy();
}

BOOL Clesson006MFCMDIView::OnEraseBkgnd(CDC* pDC)
{
    return  TRUE;

    return CView::OnEraseBkgnd(pDC);
}
