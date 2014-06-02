// DlgTran.cpp : 实现文件
//

#include "stdafx.h"
#include "DImageProcess.h"
#include "DlgTran.h"
#include "afxdialogex.h"


// CDlgTran 对话框

IMPLEMENT_DYNAMIC(CDlgTran, CDialog)

CDlgTran::CDlgTran(CWnd* pParent /*=NULL*/)
	: CDialog(CDlgTran::IDD, pParent)
{
	m_horOff = 50;
	m_verOff = 50;
}

CDlgTran::~CDlgTran()
{
}

void CDlgTran::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_horOffSet, m_horOff);
	DDX_Text(pDX, IDC_verOffSet, m_verOff);
}


BEGIN_MESSAGE_MAP(CDlgTran, CDialog)
END_MESSAGE_MAP()


// CDlgTran 消息处理程序

BOOL CDlgTran::OnInitDialog()
{
	CDialog::OnInitDialog();
	CSpinButtonCtrl *pSpinHor = (CSpinButtonCtrl*)GetDlgItem(IDC_SPIN_hor);
	pSpinHor->SetRange(-100, 100);
	CSpinButtonCtrl *pSpinVer = (CSpinButtonCtrl*)GetDlgItem(IDC_SPIN_ver);
	pSpinVer->SetRange(-100, 100);
	return TRUE;
}