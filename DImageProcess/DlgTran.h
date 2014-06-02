#pragma once


// CDlgTran 对话框

class CDlgTran : public CDialog
{
	DECLARE_DYNAMIC(CDlgTran)

public:
	CDlgTran(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CDlgTran();

// 对话框数据
	enum { IDD = IDD_dlg_translation };
	int m_horOff;
	int m_verOff;

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持
	virtual BOOL OnInitDialog();

	DECLARE_MESSAGE_MAP()
};
