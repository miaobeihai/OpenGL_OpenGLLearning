
// lesson006-MFC-MDIView.cpp : Clesson006MFCMDIView ���ʵ��
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
	// ��׼��ӡ����
	ON_COMMAND(ID_FILE_PRINT, &CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_DIRECT, &CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_PREVIEW, &Clesson006MFCMDIView::OnFilePrintPreview)
    ON_WM_DESTROY()
    ON_WM_ERASEBKGND()
END_MESSAGE_MAP()

// Clesson006MFCMDIView ����/����

Clesson006MFCMDIView::Clesson006MFCMDIView()
{
	// TODO: �ڴ˴���ӹ������

}

Clesson006MFCMDIView::~Clesson006MFCMDIView()
{
}

BOOL Clesson006MFCMDIView::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: �ڴ˴�ͨ���޸�
	//  CREATESTRUCT cs ���޸Ĵ��������ʽ

	return CView::PreCreateWindow(cs);
}

// Clesson006MFCMDIView ����

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

	// TODO: �ڴ˴�Ϊ����������ӻ��ƴ���
}


// Clesson006MFCMDIView ��ӡ


void Clesson006MFCMDIView::OnFilePrintPreview()
{
	AFXPrintPreview(this);
}

BOOL Clesson006MFCMDIView::OnPreparePrinting(CPrintInfo* pInfo)
{
	// Ĭ��׼��
	return DoPreparePrinting(pInfo);
}

void Clesson006MFCMDIView::OnBeginPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: ��Ӷ���Ĵ�ӡǰ���еĳ�ʼ������
}

void Clesson006MFCMDIView::OnEndPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: ��Ӵ�ӡ����е��������
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


// Clesson006MFCMDIView ���

#ifdef _DEBUG
void Clesson006MFCMDIView::AssertValid() const
{
	CView::AssertValid();
}

void Clesson006MFCMDIView::Dump(CDumpContext& dc) const
{
	CView::Dump(dc);
}

Clesson006MFCMDIDoc* Clesson006MFCMDIView::GetDocument() const // �ǵ��԰汾��������
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(Clesson006MFCMDIDoc)));
	return (Clesson006MFCMDIDoc*)m_pDocument;
}
#endif //_DEBUG


// Clesson006MFCMDIView ��Ϣ�������

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
