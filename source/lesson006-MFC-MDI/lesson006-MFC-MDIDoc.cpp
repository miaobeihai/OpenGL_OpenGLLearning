
// lesson006-MFC-MDIDoc.cpp : Clesson006MFCMDIDoc ���ʵ��
//

#include "stdafx.h"
#include "lesson006-MFC-MDI.h"

#include "lesson006-MFC-MDIDoc.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// Clesson006MFCMDIDoc

IMPLEMENT_DYNCREATE(Clesson006MFCMDIDoc, CDocument)

BEGIN_MESSAGE_MAP(Clesson006MFCMDIDoc, CDocument)
END_MESSAGE_MAP()


// Clesson006MFCMDIDoc ����/����

Clesson006MFCMDIDoc::Clesson006MFCMDIDoc()
{
	// TODO: �ڴ����һ���Թ������

}

Clesson006MFCMDIDoc::~Clesson006MFCMDIDoc()
{
}

BOOL Clesson006MFCMDIDoc::OnNewDocument()
{
	if (!CDocument::OnNewDocument())
		return FALSE;

	// TODO: �ڴ�������³�ʼ������
	// (SDI �ĵ������ø��ĵ�)

	return TRUE;
}




// Clesson006MFCMDIDoc ���л�

void Clesson006MFCMDIDoc::Serialize(CArchive& ar)
{
	if (ar.IsStoring())
	{
		// TODO: �ڴ���Ӵ洢����
	}
	else
	{
		// TODO: �ڴ���Ӽ��ش���
	}
}


// Clesson006MFCMDIDoc ���

#ifdef _DEBUG
void Clesson006MFCMDIDoc::AssertValid() const
{
	CDocument::AssertValid();
}

void Clesson006MFCMDIDoc::Dump(CDumpContext& dc) const
{
	CDocument::Dump(dc);
}
#endif //_DEBUG


// Clesson006MFCMDIDoc ����
