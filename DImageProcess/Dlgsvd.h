#pragma once


// CDlgsvd �Ի���

class CDlgsvd : public CDialog
{
	DECLARE_DYNAMIC(CDlgsvd)

public:
	CDlgsvd(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~CDlgsvd();

// �Ի�������
	enum { IDD = IDD_dlg_svd };
	int m_svd;

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��
	virtual BOOL OnInitDialog();

	DECLARE_MESSAGE_MAP()
};
