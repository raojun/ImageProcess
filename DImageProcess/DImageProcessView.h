
// DImageProcessView.h : CDImageProcessView ��Ľӿ�
//

#pragma once


class CDImageProcessView : public CView
{
protected: // �������л�����
	CDImageProcessView();
	DECLARE_DYNCREATE(CDImageProcessView)

// ����
public:
	CDImageProcessDoc* GetDocument() const;

// ����
public:

// ��д
public:
	virtual void OnDraw(CDC* pDC);  // ��д�Ի��Ƹ���ͼ
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
protected:
	virtual BOOL OnPreparePrinting(CPrintInfo* pInfo);
	virtual void OnBeginPrinting(CDC* pDC, CPrintInfo* pInfo);
	virtual void OnEndPrinting(CDC* pDC, CPrintInfo* pInfo);

// ʵ��
public:
	virtual ~CDImageProcessView();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:

	double* m_pDbImage;
	int m_nDWTCurDepth;

// ���ɵ���Ϣӳ�亯��
protected:
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnZoom();
	afx_msg void OnTranslation();
	afx_msg void OnRotation();
};

#ifndef _DEBUG  // DImageProcessView.cpp �еĵ��԰汾
inline CDImageProcessDoc* CDImageProcessView::GetDocument() const
   { return reinterpret_cast<CDImageProcessDoc*>(m_pDocument); }
#endif

