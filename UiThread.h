#pragma once
#include "afxwin.h"
#include "PictureDlg.h"

#define WM_SPLASH_NOTIFY (WM_USER + 1)

//UI�߳�
class CUiThread : public CWinThread
{
public:
	CUiThread();
	virtual ~CUiThread();

	//���س�ʼ�����˳����Լ���ʼ�����Զ������߳�����������
	virtual BOOL InitInstance();

	virtual int ExitInstance();

	virtual int Run();

public:
	String path;
	CPictureDlg* dlg;
	VideoCapture capture;
	Mat frame;

	CRect rect;

	long totalFrameNumber;
	long currentFrame;


protected:

	//�������̴߳�������Ϣ
	BOOL PreTranslateMessage(MSG* pMsg);

	//ѭ��������Ϣ���к���
	void message();
};

