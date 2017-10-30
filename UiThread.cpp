#include "stdafx.h"
#include "SplitScreen.h"

#include "UiThread.h"
#include "Pic.h"
#include "PictureDlg.h"

CUiThread::CUiThread()
{
	currentFrame = 0;
}


CUiThread::~CUiThread()
{
	//delete dlg;
}

BOOL CUiThread::InitInstance()
{
	//dlg = new CPictureDlg();
	//dlg->Create(IDD__PICTURE_SHOW);			//�������߳���ʾ�ĶԻ��򴰿�

	m_pMainWnd = dlg;						//����CWinThread���m_pMainWnd��Ա����������̲߳������Ž���Ĺرն��˳���

	return true;
}

int CUiThread::ExitInstance()
{
	dlg->DestroyWindow();//���ٴ���
	delete dlg;
	return 0;
}

int CUiThread::Run()
{
	capture.open(path);
	if (!capture.isOpened())
		AfxMessageBox(L"����Դʧ��!");
	//��ȡ��Ƶ��֡��
	totalFrameNumber = static_cast<long>(capture.get(CV_CAP_PROP_FRAME_COUNT));
	dlg->MoveWindow(rect.left, rect.top, rect.right, rect.bottom, true);
	while (1)
	{
		if (capture.read(frame))	//ѭ����ȡ��Ƶ��ÿһ֡
		{
			if (currentFrame <= totalFrameNumber)
			{
				//���̶߳�Ӧ����Ļ����ʾ
				dlg->showImage(frame);
				currentFrame++;
			}
			else
			{
				break;
			}
			Sleep(1);
		}
		else
			break;
	}
	return 0;
}


//HWND hWnd = (HWND)ptr;
/*for (int i = 0; i<100; ++i)
{
::PostMessage(hWnd, WM_USER_MSG, WPARAM(i), LPARAM(0));
Sleep(100);
}
AfxMessageBox(L"���");
::PostMessage(hWnd, WM_USER_MSG, WPARAM(0), LPARAM(0));
_endthread();*/

//void CUiThread::run()
//{
//	MSG msg;
//	while (1)
//	{
//		GetMessage(&msg, NULL, 0, 0);
//		DispatchMessage(&msg);
//	}
//}

//BOOL CUiThread::PreTranslateMessage(MSG* pMsg)
//{
//	// TODO: Add your specialized code here and/or call the base class
//	if (pMsg->message == WM_SPLASH_NOTIFY)
//	{
//	}
//	return CWinThread::PreTranslateMessage(pMsg);
//}