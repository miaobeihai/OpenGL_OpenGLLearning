
// lesson006-MFC-MDIDoc.h : Clesson006MFCMDIDoc 类的接口
//


#pragma once


class Clesson006MFCMDIDoc : public CDocument
{
protected: // 仅从序列化创建
	Clesson006MFCMDIDoc();
	DECLARE_DYNCREATE(Clesson006MFCMDIDoc)

// 属性
public:

// 操作
public:

// 重写
public:
	virtual BOOL OnNewDocument();
	virtual void Serialize(CArchive& ar);

// 实现
public:
	virtual ~Clesson006MFCMDIDoc();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:

// 生成的消息映射函数
protected:
	DECLARE_MESSAGE_MAP()
};


