
// DImageProcessDoc.cpp : CDImageProcessDoc ���ʵ��
//

#include "stdafx.h"
// SHARED_HANDLERS ������ʵ��Ԥ��������ͼ������ɸѡ�������
// ATL ��Ŀ�н��ж��壬�����������Ŀ�����ĵ����롣
#ifndef SHARED_HANDLERS
#include "DImageProcess.h"
#endif

#include "DImageProcessDoc.h"

#include <propkey.h>

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

// CDImageProcessDoc

IMPLEMENT_DYNCREATE(CDImageProcessDoc, CDocument)

BEGIN_MESSAGE_MAP(CDImageProcessDoc, CDocument)
END_MESSAGE_MAP()


// CDImageProcessDoc ����/����

CDImageProcessDoc::CDImageProcessDoc()
{
	// TODO:  �ڴ����һ���Թ������
	m_hDIB = NULL;// ��ʼ������
	m_palDIB = NULL;
	m_sizeDoc = CSize(1, 1);
}

CDImageProcessDoc::~CDImageProcessDoc()
{
	if (m_hDIB != NULL)// �ж��Ƿ���DIB����
	{
		::GlobalFree((HGLOBAL)m_hDIB);
	}


	if (m_palDIB != NULL)// �жϵ�ɫ���Ƿ����
	{
		delete m_palDIB;
	}
}

BOOL CDImageProcessDoc::OnNewDocument()
{
	if (!CDocument::OnNewDocument())
		return FALSE;

	// TODO:  �ڴ�������³�ʼ������
	// (SDI �ĵ������ø��ĵ�)

	return TRUE;
}




// CDImageProcessDoc ���л�

void CDImageProcessDoc::Serialize(CArchive& ar)
{
	if (ar.IsStoring())
	{
		// TODO:  �ڴ���Ӵ洢����
	}
	else
	{
		// TODO:  �ڴ���Ӽ��ش���
	}
}

#ifdef SHARED_HANDLERS

// ����ͼ��֧��
void CDImageProcessDoc::OnDrawThumbnail(CDC& dc, LPRECT lprcBounds)
{
	// �޸Ĵ˴����Ի����ĵ�����
	dc.FillSolidRect(lprcBounds, RGB(255, 255, 255));

	CString strText = _T("TODO: implement thumbnail drawing here");
	LOGFONT lf;

	CFont* pDefaultGUIFont = CFont::FromHandle((HFONT) GetStockObject(DEFAULT_GUI_FONT));
	pDefaultGUIFont->GetLogFont(&lf);
	lf.lfHeight = 36;

	CFont fontDraw;
	fontDraw.CreateFontIndirect(&lf);

	CFont* pOldFont = dc.SelectObject(&fontDraw);
	dc.DrawText(strText, lprcBounds, DT_CENTER | DT_WORDBREAK);
	dc.SelectObject(pOldFont);
}

// ������������֧��
void CDImageProcessDoc::InitializeSearchContent()
{
	CString strSearchContent;
	// ���ĵ����������������ݡ�
	// ���ݲ���Ӧ�ɡ�;���ָ�

	// ����:     strSearchContent = _T("point;rectangle;circle;ole object;")��
	SetSearchContent(strSearchContent);
}

void CDImageProcessDoc::SetSearchContent(const CString& value)
{
	if (value.IsEmpty())
	{
		RemoveChunk(PKEY_Search_Contents.fmtid, PKEY_Search_Contents.pid);
	}
	else
	{
		CMFCFilterChunkValueImpl *pChunk = NULL;
		ATLTRY(pChunk = new CMFCFilterChunkValueImpl);
		if (pChunk != NULL)
		{
			pChunk->SetTextValue(PKEY_Search_Contents, value, CHUNK_TEXT);
			SetChunkValue(pChunk);
		}
	}
}

#endif // SHARED_HANDLERS

// CDImageProcessDoc ���

#ifdef _DEBUG
void CDImageProcessDoc::AssertValid() const
{
	CDocument::AssertValid();
}

void CDImageProcessDoc::Dump(CDumpContext& dc) const
{
	CDocument::Dump(dc);
}
#endif //_DEBUG


// CDImageProcessDoc ����

BOOL CDImageProcessDoc::OnOpenDocument(LPCTSTR lpszPathName)
{
	CFile file;
	if (!file.Open(lpszPathName, CFile::modeRead |
		CFile::shareDenyWrite))// ���ļ�
	{
		return FALSE;// ����FALSE
	}
	DeleteContents();
	m_hDIB = m_dib.LoadFile(file);
	if (m_hDIB == NULL)// ����ReadDIBFile()��ȡͼ��
	{
		return FALSE;
	}
	SetDib();// ��ʼ��DIB
	if (m_hDIB == NULL)	// �ж϶�ȡ�ļ��Ƿ�ɹ�
	{
		AfxMessageBox(_T("��ȡͼ��ʱ����"));// ��ʾ����				
		return FALSE;// ����FALSE
	}
	SetPathName(lpszPathName);// �����ļ�����		
	SetModifiedFlag(FALSE);// ��ʼ���ͱ��ΪFALSE		
	return TRUE;// ����TRUE
}

BOOL CDImageProcessDoc::OnSaveDocument(LPCTSTR lpszPathName)
{
	// TODO: Add your specialized code here and/or call the base class
	CFile file;
	if (!file.Open(lpszPathName, CFile::modeCreate |// ���ļ�
		CFile::modeReadWrite | CFile::shareExclusive))
	{
		return FALSE;// ����FALSE
	}
	BOOL bSuccess = FALSE;
	bSuccess = m_dib.SaveFile(m_hDIB, file);	// ����ͼ��	
	file.Close();// �ر��ļ�	
	SetModifiedFlag(FALSE);// �����ͱ��ΪFALSE
	if (!bSuccess)
	{
		AfxMessageBox(_T("����BMPͼ��ʱ����"));// ��ʾ����
	}
	return bSuccess;
}

void CDImageProcessDoc::SetDib()
{
	LPSTR lpdib = (LPSTR) ::GlobalLock((HGLOBAL)m_hDIB);

	if (m_dib.GetWidth(lpdib) > INT_MAX || m_dib.GetHeight(lpdib) > INT_MAX)// �ж�ͼ���Ƿ����
	{
		::GlobalUnlock((HGLOBAL)m_hDIB);
		::GlobalFree((HGLOBAL)m_hDIB);	// �ͷ�DIB����			
		m_hDIB = NULL;// ����DIBΪ��		
		AfxMessageBox(_T("��ʼ��ʧ��"));
		return;
	}
	m_sizeDoc = CSize((int)m_dib.GetWidth(lpdib), (int)m_dib.GetHeight(lpdib));// �����ĵ���С	
	::GlobalUnlock((HGLOBAL)m_hDIB);
	m_palDIB = new CPalette;// �����µ�ɫ��		
	if (m_palDIB == NULL)// �ж��Ƿ񴴽��ɹ�
	{
		::GlobalFree((HGLOBAL)m_hDIB);	// ʧ��		
		m_hDIB = NULL;// ����DIB����Ϊ��
		return;
	}
	// ����CreateDIBPalette��������ɫ��
	if (m_dib.ConstructPalette(m_hDIB, m_palDIB) == NULL)
	{
		delete m_palDIB;// ɾ��				
		m_palDIB = NULL;// ����Ϊ��	
		return;// ���ؿ�
	}
}

void CDImageProcessDoc::UpdateObject(HGLOBAL hDIB)
{
	if (m_hDIB != NULL)               // �ж�DIB�Ƿ�Ϊ��
	{
		::GlobalFree((HGLOBAL)m_hDIB);// �ǿգ������
	}
	m_hDIB = hDIB;                     // �滻���µ�DIB����	
}
