
// lesson005-MFC-SDIView.cpp : Clesson005MFCSDIView 类的实现
//

#include "stdafx.h"
#include "lesson005-MFC-SDI.h"

#include "lesson005-MFC-SDIDoc.h"
#include "lesson005-MFC-SDIView.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// Clesson005MFCSDIView

IMPLEMENT_DYNCREATE(Clesson005MFCSDIView, CView)

BEGIN_MESSAGE_MAP(Clesson005MFCSDIView, CView)
	// 标准打印命令
	ON_COMMAND(ID_FILE_PRINT, &CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_DIRECT, &CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_PREVIEW, &Clesson005MFCSDIView::OnFilePrintPreview)
    ON_WM_DESTROY()
    ON_WM_ERASEBKGND()
END_MESSAGE_MAP()

// Clesson005MFCSDIView 构造/析构

Clesson005MFCSDIView::Clesson005MFCSDIView()
{
	// TODO: 在此处添加构造代码

}

Clesson005MFCSDIView::~Clesson005MFCSDIView()
{
}

BOOL Clesson005MFCSDIView::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: 在此处通过修改
	//  CREATESTRUCT cs 来修改窗口类或样式

	return CView::PreCreateWindow(cs);
}

// Clesson005MFCSDIView 绘制

void Clesson005MFCSDIView::OnDraw(CDC* /*pDC*/)
{
	Clesson005MFCSDIDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	if (!pDoc)
		return;

	glClearColor(0,1,0,1);
    glClear(GL_COLOR_BUFFER_BIT);
    _context.swapBuffer();
}


// Clesson005MFCSDIView 打印


void Clesson005MFCSDIView::OnFilePrintPreview()
{
	AFXPrintPreview(this);
}

BOOL Clesson005MFCSDIView::OnPreparePrinting(CPrintInfo* pInfo)
{
	// 默认准备
	return DoPreparePrinting(pInfo);
}

void Clesson005MFCSDIView::OnBeginPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: 添加额外的打印前进行的初始化过程
}

void Clesson005MFCSDIView::OnEndPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: 添加打印后进行的清理过程
}

void Clesson005MFCSDIView::OnRButtonUp(UINT nFlags, CPoint point)
{
	ClientToScreen(&point);
	OnContextMenu(this, point);
}

void Clesson005MFCSDIView::OnContextMenu(CWnd* pWnd, CPoint point)
{
	theApp.GetContextMenuManager()->ShowPopupMenu(IDR_POPUP_EDIT, point.x, point.y, this, TRUE);
}


// Clesson005MFCSDIView 诊断

#ifdef _DEBUG
void Clesson005MFCSDIView::AssertValid() const
{
	CView::AssertValid();
}

void Clesson005MFCSDIView::Dump(CDumpContext& dc) const
{
	CView::Dump(dc);
}

Clesson005MFCSDIDoc* Clesson005MFCSDIView::GetDocument() const // 非调试版本是内联的
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(Clesson005MFCSDIDoc)));
	return (Clesson005MFCSDIDoc*)m_pDocument;
}
#endif //_DEBUG


// Clesson005MFCSDIView 消息处理程序

void Clesson005MFCSDIView::OnInitialUpdate()
{
    CView::OnInitialUpdate();

    HDC     hDC =   ::GetDC(m_hWnd);
    _context.setup(m_hWnd,hDC);
}

void Clesson005MFCSDIView::OnDestroy()
{
    _context.shutdown();
    CView::OnDestroy();
}

BOOL Clesson005MFCSDIView::OnEraseBkgnd(CDC* pDC)
{
    return  TRUE;
}
