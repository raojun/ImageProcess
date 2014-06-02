#pragma once


// CDlgRot 对话框

class CDlgRot : public CDialog
{
	DECLARE_DYNAMIC(CDlgRot)

public:
	CDlgRot(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CDlgRot();

// 对话框数据
	enum { IDD = IDD_dlg_rotation };
	int m_rotAngle;

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持
	virtual BOOL OnInitDialog();

	DECLARE_MESSAGE_MAP()
};
