
// lesson008-MFC-Dialog-ES2Dlg.h : ͷ�ļ�
//

#pragma once

#include "GLESContext.h"

// Clesson008MFCDialogES2Dlg �Ի���
class Clesson008MFCDialogES2Dlg : public CDialog
{
// ����
public:
	Clesson008MFCDialogES2Dlg(CWnd* pParent = NULL);	// ��׼���캯��

// �Ի�������
	enum { IDD = IDD_LESSON008MFCDIALOGES2_DIALOG };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV ֧��

protected:
    GLESContext _context;
// ʵ��
protected:
	HICON m_hIcon;

	// ���ɵ���Ϣӳ�亯��
	virtual BOOL OnInitDialog();
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()
public:
    afx_msg void OnDestroy();
    afx_msg BOOL OnEraseBkgnd(CDC* pDC);
};
