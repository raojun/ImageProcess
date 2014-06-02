
// DImageProcessView.cpp : CDImageProcessView ���ʵ��
//

#include "stdafx.h"
// SHARED_HANDLERS ������ʵ��Ԥ��������ͼ������ɸѡ�������
// ATL ��Ŀ�н��ж��壬�����������Ŀ�����ĵ����롣
#ifndef SHARED_HANDLERS
#include "DImageProcess.h"
#endif

#include "DImageProcessDoc.h"
#include "DImageProcessView.h"

#include "function.h"
#include "DlgZoom.h"
#include "DlgTran.h"
#include "DlgRot.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CDImageProcessView

IMPLEMENT_DYNCREATE(CDImageProcessView, CView)

BEGIN_MESSAGE_MAP(CDImageProcessView, CView)
	// ��׼��ӡ����
	ON_COMMAND(ID_FILE_PRINT, &CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_DIRECT, &CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_PREVIEW, &CView::OnFilePrintPreview)
	ON_COMMAND(ID_geo_zoom, &CDImageProcessView::OnZoom)
	ON_COMMAND(ID_geo_translation, &CDImageProcessView::OnTranslation)
	ON_COMMAND(ID_geo_rotation, &CDImageProcessView::OnRotation)
END_MESSAGE_MAP()

// CDImageProcessView ����/����

CDImageProcessView::CDImageProcessView()
{
	// TODO:  �ڴ˴���ӹ������
	
	m_pDbImage = NULL;
	m_nDWTCurDepth = 0;
}

CDImageProcessView::~CDImageProcessView()
{
}

BOOL CDImageProcessView::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO:  �ڴ˴�ͨ���޸�
	//  CREATESTRUCT cs ���޸Ĵ��������ʽ

	return CView::PreCreateWindow(cs);
}

// CDImageProcessView ����

void CDImageProcessView::OnDraw(CDC* pDC)
{
	CDImageProcessDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);

	// TODO:  �ڴ˴�Ϊ����������ӻ��ƴ���

	HGLOBAL hDIB = pDoc->GetHObject();
	// �ж�DIB�Ƿ�Ϊ��
	if (hDIB != NULL)
	{
		LPSTR lpDibSection = (LPSTR) ::GlobalLock((HGLOBAL)hDIB);

		// ��ȡDIB���
		int cxDIB = (int)pDoc->m_dib.GetWidth(lpDibSection);

		// ��ȡDIB�߶�
		int cyDIB = (int)pDoc->m_dib.GetHeight(lpDibSection);

		::GlobalUnlock((HGLOBAL)hDIB);

		CRect rcDIB;
		rcDIB.top = rcDIB.left = 0;
		rcDIB.right = cxDIB;
		rcDIB.bottom = cyDIB;

		CRect rcDest = rcDIB;
		// ���DIB
		pDoc->m_dib.DrawDib(pDC->m_hDC, &rcDest, pDoc->GetHObject(),
			&rcDIB, pDoc->GetDocPal());
	}
}


// CDImageProcessView ��ӡ

BOOL CDImageProcessView::OnPreparePrinting(CPrintInfo* pInfo)
{
	// Ĭ��׼��
	return DoPreparePrinting(pInfo);
}

void CDImageProcessView::OnBeginPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO:  ��Ӷ���Ĵ�ӡǰ���еĳ�ʼ������
}

void CDImageProcessView::OnEndPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO:  ��Ӵ�ӡ����е��������
}


// CDImageProcessView ���

#ifdef _DEBUG
void CDImageProcessView::AssertValid() const
{
	CView::AssertValid();
}

void CDImageProcessView::Dump(CDumpContext& dc) const
{
	CView::Dump(dc);
}

CDImageProcessDoc* CDImageProcessView::GetDocument() const // �ǵ��԰汾��������
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CDImageProcessDoc)));
	return (CDImageProcessDoc*)m_pDocument;
}
#endif //_DEBUG


// CDImageProcessView ��Ϣ�������


void CDImageProcessView::OnZoom()
{
	// TODO:  �ڴ���������������
	CDImageProcessDoc* pDoc = GetDocument();
	////////////////////////////////////////////////////////////////////////////////////////////////	
	long lSrcLineBytes;		//ͼ��ÿ�е��ֽ���
	long	lSrcWidth;      //ͼ��Ŀ�Ⱥ͸߶�
	long	lSrcHeight;
	LPSTR	lpSrcDib;		//ָ��Դͼ���ָ��	
	LPSTR	lpSrcStartBits;	//ָ��Դ���ص�ָ��
	long	lDstWidth;      //��ʱͼ��Ŀ�Ⱥ͸߶�
	long	lDstHeight;
	long lDstLineBytes;
	lpSrcDib = (LPSTR) ::GlobalLock((HGLOBAL)pDoc->GetHObject());// ����DIB
	if (pDoc->m_dib.GetColorNum(lpSrcDib) != 256)// �ж��Ƿ���8-bppλͼ
	{
		AfxMessageBox(_T("�Բ��𣬲���256ɫλͼ��"));// ����				
		::GlobalUnlock((HGLOBAL)pDoc->GetHObject());// �������		
		return;									//����
	}
	lpSrcStartBits = pDoc->m_dib.GetBits(lpSrcDib);			// �ҵ�DIBͼ��������ʼλ��	
	lSrcWidth = pDoc->m_dib.GetWidth(lpSrcDib);					// ��ȡͼ��Ŀ��		
	lSrcHeight = pDoc->m_dib.GetHeight(lpSrcDib);					// ��ȡͼ��ĸ߶�		
	lSrcLineBytes = pDoc->m_dib.GetReqByteWidth(lSrcWidth * 8);		// ����ͼ��ÿ�е��ֽ���
	/////////////////////////////////////////////////////////////////////////////////////////////////
	DWORD palSize = pDoc->m_dib.GetPalSize(lpSrcDib);

	CDlgZoom ZoomPara;// �����Ի���,�趨ƽ����
	if (ZoomPara.DoModal() != IDOK)
	{
		return;
	}
	float fX = ZoomPara.m_horZoom;// ��ȡ�趨��ƽ����,���ű���
	float fY = ZoomPara.m_verZoom;

	lDstWidth = (long)(lSrcWidth*fX + 0.5);// �������ź��ͼ��ʵ�ʿ��,��0.5������ǿ������ת��ʱ���������룬����ֱ�ӽ�ȥС������
	lDstLineBytes = pDoc->m_dib.GetReqByteWidth(lDstWidth * 8);	//ת����ͼ��Ӧ�е����ֽ�����Ϊ4�ı���
	lDstHeight = (long)(lSrcHeight * fY + 0.5);// �������ź��ͼ��߶�

	HGLOBAL hDstDIB = NULL;// ������DIB	
	hDstDIB = (HGLOBAL)Zoom(lpSrcDib, lpSrcStartBits, lSrcWidth, lSrcHeight,
		lSrcLineBytes, palSize, lDstWidth, lDstLineBytes, lDstHeight, fX, fY);// ����Zoom()����ת��DIB		

	if (hDstDIB != NULL)// �ж���ת�Ƿ�ɹ�
	{
		pDoc->UpdateObject(hDstDIB);// �滻DIB��ͬʱ�ͷž�DIB����		
		pDoc->SetDib();// ����DIB��С�͵�ɫ��				
		pDoc->SetModifiedFlag(TRUE);								// ��������		
		pDoc->UpdateAllViews(NULL);									// ������ͼ		
		::GlobalUnlock((HGLOBAL)pDoc->GetHObject());					// �������
	}
	else
	{
		AfxMessageBox(_T("�����ڴ�ʧ�ܣ�"));
	}
	// ����	
}


void CDImageProcessView::OnTranslation()
{
	// TODO:  �ڴ���������������
	CDImageProcessDoc* pDoc = GetDocument();
	////////////////////////////////////////////////////////////////////////////////////////////////	
	long lSrcLineBytes;		//ͼ��ÿ�е��ֽ���
	long	lSrcWidth;      //ͼ��Ŀ�Ⱥ͸߶�
	long	lSrcHeight;
	LPSTR	lpSrcDib;		//ָ��Դͼ���ָ��	
	LPSTR	lpSrcStartBits;	//ָ��Դ���ص�ָ��
	long lDstLineBytes;		//��ͼ��ÿ�е��ֽ���
	lpSrcDib = (LPSTR) ::GlobalLock((HGLOBAL)pDoc->GetHObject());// ����DIB
	if (pDoc->m_dib.GetColorNum(lpSrcDib) != 256)// �ж��Ƿ���8-bppλͼ
	{
		AfxMessageBox(_T("�Բ��𣬲���256ɫλͼ��"));// ����				
		::GlobalUnlock((HGLOBAL)pDoc->GetHObject());// �������		
		return;									//����
	}											//�ж��Ƿ���8-bppλͼ,�����򷵻�	
	lpSrcStartBits = pDoc->m_dib.GetBits(lpSrcDib);			// �ҵ�DIBͼ��������ʼλ��	
	lSrcWidth = pDoc->m_dib.GetWidth(lpSrcDib);					// ��ȡͼ��Ŀ��		
	lSrcHeight = pDoc->m_dib.GetHeight(lpSrcDib);					// ��ȡͼ��ĸ߶�		
	lSrcLineBytes = pDoc->m_dib.GetReqByteWidth(lSrcWidth * 8);		// ����ͼ��ÿ�е��ֽ���
	/////////////////////////////////////////////////////////////////////////////////////////////////	
	lDstLineBytes = pDoc->m_dib.GetReqByteWidth(lSrcHeight * 8);	// ������ͼ��ÿ�е��ֽ���	


	CDlgTran TranPara;// �����Ի���	
	if (TranPara.DoModal() != IDOK)// ��ʾ�Ի�����ʾ�û��趨��
		return;
	int temver = TranPara.m_verOff;
	int temhor = TranPara.m_horOff;

	if (Translation(lpSrcStartBits, lSrcWidth, lSrcHeight,// ����Translation()����ƽ��DIB
		temver, temhor, lSrcLineBytes, lDstLineBytes))
	{
		pDoc->SetModifiedFlag(TRUE);								// ��������		
		pDoc->UpdateAllViews(NULL);									// ������ͼ		
		::GlobalUnlock((HGLOBAL)pDoc->GetHObject());					// �������
	}
	else
	{
		AfxMessageBox(_T("�����ڴ�ʧ�ܣ�"));
	}//����	
}


void CDImageProcessView::OnRotation()
{
	// TODO:  �ڴ���������������
	CDImageProcessDoc* pDoc = GetDocument();
	
	long lSrcLineBytes;//ͼ��ÿ�е��ֽ���
	long lSrcWidth;//ͼ��Ŀ��
	long lSrcHeight;//ͼ��ĸ߶�
	LPSTR lpSrcDib;//ָ��Դͼ���ָ��
	LPSTR lpSrcStartBits;//ָ��Դ���ص�ָ��
	long lDstWidth;//��ʱͼ��Ŀ�Ⱥ͸߶�
	long lDstHeight;

	lpSrcDib = (LPSTR)::GlobalLock((HGLOBAL)pDoc->GetHObject());//�����ڴ�

	if (pDoc->m_dib.GetColorNum(lpSrcDib) != 256)//�ж��Ƿ���8-bppλͼ�������򷵻�
	{
		AfxMessageBox(_T("�Բ��𣬲���256ɫλͼ��"));
		::GlobalUnlock((HGLOBAL)pDoc->GetHObject());//�������
		return;
	}

	lpSrcStartBits = pDoc->m_dib.GetBits(lpSrcDib);//�ҵ�DIBͼ��������ʼλ��
	lSrcWidth = pDoc->m_dib.GetWidth(lpSrcDib);//��ȡͼ��Ŀ��
	lSrcHeight = pDoc->m_dib.GetHeight(lpSrcDib);//��ȡͼ��ĸ߶�
	lSrcLineBytes = pDoc->m_dib.GetReqByteWidth(lSrcWidth * 8);//����ͼ��ÿ�е��ֽ���
	
	long lDstLineBytes; //��ͼ��ÿ�е��ֽ���

	CDlgRot RotPara;//�����Ի���
	if (RotPara.DoModal() != IDOK)//��ʾ�Ի����趨��ת�Ƕ�
	{
		return;
	}

	DWORD palSize = pDoc->m_dib.GetPalSize(lpSrcDib);
	//����ת�ǶȴӶ�ת��Ϊ����
	float fRotateAngle = (float)AngleToRation(RotPara.m_rotAngle);
	float fSina = (float)sin((double)fRotateAngle);//������ת�Ƕȵ�������
	float fCosa = (float)cos((double)fRotateAngle);

	//��תǰ4���ǵ����꣬����ͼ������Ϊ����ϵԭ�㣩
	float fSrcX1, fSrcY1, fSrcX2, fSrcY2, fSrcX3, fSrcY3, fSrcX4, fSrcY4;
	//��ת��4���ǵ�����
	float fDstX1, fDstY1, fDstX2, fDstY2, fDstX3, fDstY3, fDstX4, fDstY4;

	//����Դͼ��4���ǵ�����
	fSrcX1 = (float)(-(lSrcWidth - 1) / 2);
	fSrcY1 = (float)((lSrcHeight - 1) / 2);
	fSrcX2 = (float)((lSrcWidth - 1) / 2);
	fSrcY2 = (float)((lSrcHeight - 1) / 2);
	fSrcX3 = (float)(-(lSrcWidth - 1) / 2);
	fSrcY3 = (float)(-(lSrcHeight - 1) / 2);
	fSrcX4 = (float)((lSrcWidth - 1) / 2);
	fSrcY4 = (float)(-(lSrcHeight - 1) / 2);

	//������ͼ��4���ǵ�����
	fDstX1 = fCosa * fSrcX1 + fSina * fSrcY1;
	fDstY1 = -fSina * fSrcX1 + fCosa * fSrcY1;
	fDstX2 = fCosa * fSrcX2 + fSina * fSrcY2;
	fDstY2 = -fSina * fSrcX2 + fCosa * fSrcY2;
	fDstX3 = fCosa * fSrcX3 + fSina * fSrcY3;
	fDstY3 = -fSina * fSrcX3 + fCosa * fSrcY3;
	fDstX4 = fCosa * fSrcX4 + fSina * fSrcY4;
	fDstY4 = -fSina * fSrcX4 + fCosa * fSrcY4;

	//������ת���ͼ��ʵ�ʿ�Ⱥ͸߶�
	lDstWidth = (long)(max(fabs(fDstX4 - fDstX1), fabs(fDstX3 - fDstX2)) + 0.5);
	lDstHeight = (long)(max(fabs(fDstY4 - fDstY1), fabs(fDstY3 - fDstY2)) + 0.5);

	//������ͼ��ÿ�е��ֽ���
	lDstLineBytes = pDoc->m_dib.GetReqByteWidth(lDstWidth * 8);

	HGLOBAL hDstDIB = NULL;//������DIB

	//����Rotate()������תDIB
	hDstDIB = (HGLOBAL)Rotate(lpSrcDib, lpSrcStartBits, lSrcWidth, lSrcHeight, lSrcLineBytes,
		palSize, lDstWidth, lDstHeight, lDstLineBytes, fSina, fCosa);

	//�ж��Ƿ���ת�ɹ�
	if (hDstDIB != NULL)
	{
		pDoc->UpdateObject(hDstDIB);//�滻DIB��ͬʱ�ͷž�DIB����
		pDoc->SetDib();//����DIB��С�͵�ɫ��
		pDoc->SetModifiedFlag(TRUE);//�����޸ı��
		pDoc->UpdateAllViews(NULL);//������ͼ
		::GlobalUnlock((HGLOBAL)pDoc->GetHObject());//�������
	}
	else
	{
		AfxMessageBox(_T("�����ڴ�ʧ�ܣ�"));
	}
}
