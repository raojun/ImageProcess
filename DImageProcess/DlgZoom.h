#pragma once


// CDlgZoom �Ի���

class CDlgZoom : public CDialog
{
	DECLARE_DYNAMIC(CDlgZoom)

public:
	CDlgZoom(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~CDlgZoom();

// �Ի�������
	enum { IDD = IDD_dlg_zoom };
	float	m_horZoom;
	float	m_verZoom;

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

	DECLARE_MESSAGE_MAP()
};