
// lesson006-MFC-MDIDoc.cpp : Clesson006MFCMDIDoc 类的实现
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


// Clesson006MFCMDIDoc 构造/析构

Clesson006MFCMDIDoc::Clesson006MFCMDIDoc()
{
	// TODO: 在此添加一次性构造代码

}

Clesson006MFCMDIDoc::~Clesson006MFCMDIDoc()
{
}

BOOL Clesson006MFCMDIDoc::OnNewDocument()
{
	if (!CDocument::OnNewDocument())
		return FALSE;

	// TODO: 在此添加重新初始化代码
	// (SDI 文档将重用该文档)

	return TRUE;
}




// Clesson006MFCMDIDoc 序列化

void Clesson006MFCMDIDoc::Serialize(CArchive& ar)
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


// Clesson006MFCMDIDoc 诊断

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


// Clesson006MFCMDIDoc 命令
