

#pragma once

#include "GLContext.h"


class Clesson005MFCSDIView : public CView
{
protected: // 仅从序列化创建
	Clesson005MFCSDIView();
	DECLARE_DYNCREATE(Clesson005MFCSDIView)

protected:
    GLContext   _context;
// 属性
public:
	Clesson005MFCSDIDoc* GetDocument() const;

// 操作
public:

// 重写
public:
	virtual void OnDraw(CDC* pDC);  // 重写以绘制该视图
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
protected:
	virtual BOOL OnPreparePrinting(CPrintInfo* pInfo);
	virtual void OnBeginPrinting(CDC* pDC, CPrintInfo* pInfo);
	virtual void OnEndPrinting(CDC* pDC, CPrintInfo* pInfo);

// 实现
public:
	virtual ~Clesson005MFCSDIView();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:

// 生成的消息映射函数
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

#ifndef _DEBUG  // lesson005-MFC-SDIView.cpp 中的调试版本
inline Clesson005MFCSDIDoc* Clesson005MFCSDIView::GetDocument() const
   { return reinterpret_cast<Clesson005MFCSDIDoc*>(m_pDocument); }
#endif

