
// lesson006-MFC-MDIView.h : Clesson006MFCMDIView ��Ľӿ�
//


#pragma once

#include "GLContext.h"


class Clesson006MFCMDIView : public CView
{
protected: // �������л�����
	Clesson006MFCMDIView();
	DECLARE_DYNCREATE(Clesson006MFCMDIView)

protected:

    GLContext   _context;
    int         _index;
// ����
public:
	Clesson006MFCMDIDoc* GetDocument() const;

// ����
public:

// ��д
public:
	virtual void OnDraw(CDC* pDC);  // ��д�Ի��Ƹ���ͼ
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
protected:
	virtual BOOL OnPreparePrinting(CPrintInfo* pInfo);
	virtual void OnBeginPrinting(CDC* pDC, CPrintInfo* pInfo);
	virtual void OnEndPrinting(CDC* pDC, CPrintInfo* pInfo);

// ʵ��
public:
	virtual ~Clesson006MFCMDIView();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:

// ���ɵ���Ϣӳ�亯��
protected:
	afx_msg void OnFilePrintPreview();
	afx_msg void OnRButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnContextMenu(CWnd* pWnd, CPoint point);
	DECLARE_MESSAGE_MAP()
public:
    virtual void OnInitialUpdate();
    afx_msg void OnDestroy();
    afx_msg BOOL OnEraseBkgnd(CDC* pDC);
};

#ifndef _DEBUG  // lesson006-MFC-MDIView.cpp �еĵ��԰汾
inline Clesson006MFCMDIDoc* Clesson006MFCMDIView::GetDocument() const
   { return reinterpret_cast<Clesson006MFCMDIDoc*>(m_pDocument); }
#endif

