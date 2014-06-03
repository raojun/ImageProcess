#pragma once


// CDlgsvd 对话框

class CDlgsvd : public CDialog
{
	DECLARE_DYNAMIC(CDlgsvd)

public:
	CDlgsvd(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CDlgsvd();

// 对话框数据
	enum { IDD = IDD_dlg_svd };
	int m_svd;

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持
	virtual BOOL OnInitDialog();

	DECLARE_MESSAGE_MAP()
};
