
// lesson005-MFC-SDIDoc.cpp : Clesson005MFCSDIDoc ���ʵ��
//

#include "stdafx.h"
#include "lesson005-MFC-SDI.h"

#include "lesson005-MFC-SDIDoc.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// Clesson005MFCSDIDoc

IMPLEMENT_DYNCREATE(Clesson005MFCSDIDoc, CDocument)

BEGIN_MESSAGE_MAP(Clesson005MFCSDIDoc, CDocument)
END_MESSAGE_MAP()


// Clesson005MFCSDIDoc ����/����

Clesson005MFCSDIDoc::Clesson005MFCSDIDoc()
{
	// TODO: �ڴ����һ���Թ������

}

Clesson005MFCSDIDoc::~Clesson005MFCSDIDoc()
{
}

BOOL Clesson005MFCSDIDoc::OnNewDocument()
{
	if (!CDocument::OnNewDocument())
		return FALSE;

	// TODO: �ڴ�������³�ʼ������
	// (SDI �ĵ������ø��ĵ�)

	return TRUE;
}




// Clesson005MFCSDIDoc ���л�

void Clesson005MFCSDIDoc::Serialize(CArchive& ar)
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


// Clesson005MFCSDIDoc ���

#ifdef _DEBUG
void Clesson005MFCSDIDoc::AssertValid() const
{
	CDocument::AssertValid();
}

void Clesson005MFCSDIDoc::Dump(CDumpContext& dc) const
{
	CDocument::Dump(dc);
}
#endif //_DEBUG


// Clesson005MFCSDIDoc ����
