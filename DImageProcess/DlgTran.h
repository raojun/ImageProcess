#pragma once


// CDlgTran �Ի���

class CDlgTran : public CDialog
{
	DECLARE_DYNAMIC(CDlgTran)

public:
	CDlgTran(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~CDlgTran();

// �Ի�������
	enum { IDD = IDD_dlg_translation };
	int m_horOff;
	int m_verOff;

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��
	virtual BOOL OnInitDialog();

	DECLARE_MESSAGE_MAP()
};
