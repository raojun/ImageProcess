// DlgZoom.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "DImageProcess.h"
#include "DlgZoom.h"
#include "afxdialogex.h"


// CDlgZoom �Ի���

IMPLEMENT_DYNAMIC(CDlgZoom, CDialog)

CDlgZoom::CDlgZoom(CWnd* pParent /*=NULL*/)
	: CDialog(CDlgZoom::IDD, pParent)
{

	m_horZoom = 1.0f;
	m_verZoom = 1.0f;
}

CDlgZoom::~CDlgZoom()
{
}

void CDlgZoom::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);

	DDX_Text(pDX, IDC_EDIT_XZoom, m_horZoom);
	DDX_Text(pDX, IDC_EDIT_YZoom, m_verZoom);
}


BEGIN_MESSAGE_MAP(CDlgZoom, CDialog)
END_MESSAGE_MAP()


// CDlgZoom ��Ϣ�������
