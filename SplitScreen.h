
// SplitScreen.h : PROJECT_NAME Ӧ�ó������ͷ�ļ�
//

#pragma once

#ifndef __AFXWIN_H__
	#error "�ڰ������ļ�֮ǰ������stdafx.h�������� PCH �ļ�"
#endif

#include "resource.h"		// ������


// CSplitScreenApp: 
// �йش����ʵ�֣������ SplitScreen.cpp

class CSplitScreenApp : public CWinApp
{
public:
	CSplitScreenApp();

public: 
	bool openExpanseDlg = false;		//ȫ�ֱ���
// ��д
public:
	virtual BOOL InitInstance();

// ʵ��

	DECLARE_MESSAGE_MAP()
};

extern CSplitScreenApp theApp;