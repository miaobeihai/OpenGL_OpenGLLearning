
// lesson005-MFC-SDIDoc.h : Clesson005MFCSDIDoc ��Ľӿ�
//


#pragma once


class Clesson005MFCSDIDoc : public CDocument
{
protected: // �������л�����
	Clesson005MFCSDIDoc();
	DECLARE_DYNCREATE(Clesson005MFCSDIDoc)

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
	virtual ~Clesson005MFCSDIDoc();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:

// ���ɵ���Ϣӳ�亯��
protected:
	DECLARE_MESSAGE_MAP()
};


