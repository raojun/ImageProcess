#pragma once


// CDlgRot �Ի���

class CDlgRot : public CDialog
{
	DECLARE_DYNAMIC(CDlgRot)

public:
	CDlgRot(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~CDlgRot();

// �Ի�������
	enum { IDD = IDD_dlg_rotation };
	int m_rotAngle;

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��
	virtual BOOL OnInitDialog();

	DECLARE_MESSAGE_MAP()
};
