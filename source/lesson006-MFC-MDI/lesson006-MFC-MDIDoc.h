
// lesson006-MFC-MDIDoc.h : Clesson006MFCMDIDoc ��Ľӿ�
//


#pragma once


class Clesson006MFCMDIDoc : public CDocument
{
protected: // �������л�����
	Clesson006MFCMDIDoc();
	DECLARE_DYNCREATE(Clesson006MFCMDIDoc)

// ����
public:

// ����
public:

// ��д
public:
	virtual BOOL OnNewDocument();
	virtual void Serialize(CArchive& ar);

// ʵ��
public:
	virtual ~Clesson006MFCMDIDoc();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:

// ���ɵ���Ϣӳ�亯��
protected:
	DECLARE_MESSAGE_MAP()
};


