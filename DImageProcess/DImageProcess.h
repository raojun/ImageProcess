
// DImageProcess.h : DImageProcess Ӧ�ó������ͷ�ļ�
//
#pragma once

#ifndef __AFXWIN_H__
	#error "�ڰ������ļ�֮ǰ������stdafx.h�������� PCH �ļ�"
#endif

#include "resource.h"       // ������


// CDImageProcessApp:
// �йش����ʵ�֣������ DImageProcess.cpp
//

class CDImageProcessApp : public CWinApp
{
public:
	CDImageProcessApp();


// ��д
public:
	virtual BOOL InitInstance();
	virtual int ExitInstance();

// ʵ��
	afx_msg void OnAppAbout();
	DECLARE_MESSAGE_MAP()
};

extern CDImageProcessApp theApp;
