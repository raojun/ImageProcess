// Dlgsvd.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "DImageProcess.h"
#include "Dlgsvd.h"
#include "afxdialogex.h"
#include "Dlgsvd.h"

// CDlgsvd �Ի���

IMPLEMENT_DYNAMIC(CDlgsvd, CDialog)

CDlgsvd::CDlgsvd(CWnd* pParent /*=NULL*/)
	: CDialog(CDlgsvd::IDD, pParent)
{
	m_svd = 50;
}

CDlgsvd::~CDlgsvd()
{
}

void CDlgsvd::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_rot_svd, m_svd);
}


BEGIN_MESSAGE_MAP(CDlgsvd, CDialog)
END_MESSAGE_MAP()


// CDlgsvd ��Ϣ�������
BOOL CDlgsvd::OnInitDialog()
{
	CSpinButtonCtrl* pSpinsvd = (CSpinButtonCtrl*)GetDlgItem(IDC_SPIN_svd);
	pSpinsvd->SetRange(1, 100);
	return TRUE;
}