
// lesson008-MFC-Dialog-ES2Dlg.h : 头文件
//

#pragma once

#include "GLESContext.h"

// Clesson008MFCDialogES2Dlg 对话框
class Clesson008MFCDialogES2Dlg : public CDialog
{
// 构造
public:
	Clesson008MFCDialogES2Dlg(CWnd* pParent = NULL);	// 标准构造函数

// 对话框数据
	enum { IDD = IDD_LESSON008MFCDIALOGES2_DIALOG };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV 支持

protected:
    GLESContext _context;
// 实现
protected:
	HICON m_hIcon;

	// 生成的消息映射函数
	virtual BOOL OnInitDialog();
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()
public:
    afx_msg void OnDestroy();
    afx_msg BOOL OnEraseBkgnd(CDC* pDC);
};
