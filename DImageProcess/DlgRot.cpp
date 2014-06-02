// DlgRot.cpp : 实现文件
//

#include "stdafx.h"
#include "DImageProcess.h"
#include "DlgRot.h"
#include "afxdialogex.h"


// CDlgRot 对话框

IMPLEMENT_DYNAMIC(CDlgRot, CDialog)

CDlgRot::CDlgRot(CWnd* pParent /*=NULL*/)
	: CDialog(CDlgRot::IDD, pParent)
{
	m_rotAngle = 90;
}

CDlgRot::~CDlgRot()
{
}

void CDlgRot::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_rot_angle, m_rotAngle);
}


BEGIN_MESSAGE_MAP(CDlgRot, CDialog)
END_MESSAGE_MAP()


// CDlgRot 消息处理程序
BOOL CDlgRot::OnInitDialog()
{
	CDialog::OnInitDialog();
	CSpinButtonCtrl *sPinAng = (CSpinButtonCtrl*)GetDlgItem(IDC_SPIN_ang);
	sPinAng->SetRange(-360, 360);
	return TRUE;
}