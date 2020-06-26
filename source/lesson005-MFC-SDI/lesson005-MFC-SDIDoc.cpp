
// lesson005-MFC-SDIDoc.cpp : Clesson005MFCSDIDoc 类的实现
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


// Clesson005MFCSDIDoc 构造/析构

Clesson005MFCSDIDoc::Clesson005MFCSDIDoc()
{
	// TODO: 在此添加一次性构造代码

}

Clesson005MFCSDIDoc::~Clesson005MFCSDIDoc()
{
}

BOOL Clesson005MFCSDIDoc::OnNewDocument()
{
	if (!CDocument::OnNewDocument())
		return FALSE;

	// TODO: 在此添加重新初始化代码
	// (SDI 文档将重用该文档)

	return TRUE;
}




// Clesson005MFCSDIDoc 序列化

void Clesson005MFCSDIDoc::Serialize(CArchive& ar)
{
	if (ar.IsStoring())
	{
		// TODO: 在此添加存储代码
	}
	else
	{
		// TODO: 在此添加加载代码
	}
}


// Clesson005MFCSDIDoc 诊断

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


// Clesson005MFCSDIDoc 命令
