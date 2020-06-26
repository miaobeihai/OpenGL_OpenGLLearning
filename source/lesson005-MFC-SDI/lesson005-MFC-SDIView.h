

#pragma once

#include "GLContext.h"


class Clesson005MFCSDIView : public CView
{
protected: // �������л�����
	Clesson005MFCSDIView();
	DECLARE_DYNCREATE(Clesson005MFCSDIView)

protected:
    GLContext   _context;
// ����
public:
	Clesson005MFCSDIDoc* GetDocument() const;

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
	virtual ~Clesson005MFCSDIView();
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

#ifndef _DEBUG  // lesson005-MFC-SDIView.cpp �еĵ��԰汾
inline Clesson005MFCSDIDoc* Clesson005MFCSDIView::GetDocument() const
   { return reinterpret_cast<Clesson005MFCSDIDoc*>(m_pDocument); }
#endif

