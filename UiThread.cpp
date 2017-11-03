#include "stdafx.h"
#include "SplitScreen.h"

#include "UiThread.h"
#include "Pic.h"
#include "PictureDlg.h"

//���캯��

CUiThread::CUiThread()
{
	currentFrame = 0;
}

//��������

CUiThread::~CUiThread()
{
}

//UI�̶߳Ի����ʼ������

BOOL CUiThread::InitInstance()
{
	dlg = new CPictureDlg();
	dlg->Create(IDD__PICTURE_SHOW);			//�������߳���ʾ�ĶԻ��򴰿�

	m_pMainWnd = dlg;						//����CWinThread���m_pMainWnd��Ա����������̲߳������Ž���Ĺرն��˳���

	return true;
}

//UI�̶߳Ի����˳�ʱ�Զ�ִ�е��˳�����

int CUiThread::ExitInstance()
{
	dlg->DestroyWindow();//���ٴ���
	delete dlg;
	return 0;
}

//��д���߳�Run����

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
			Sleep(20);
		}
		else
		{
			break;
		}
			
	}
	return 0;
}

//ѭ��������Ϣ���к���
void CUiThread::message()
{
	MSG msg;

	//ѭ��ȡ����Ϣ�����е���Ϣ���ַ���ȥ
	while (1)
	{
		GetMessage(&msg, NULL, 0, 0);
		DispatchMessage(&msg);
	}
}

//���յ��������̵߳���Ϣ
BOOL CUiThread::PreTranslateMessage(MSG* pMsg)
{
	// TODO: Add your specialized code here and/or call the base class
	if (pMsg->message == WM_SPLASH_NOTIFY)
	{
		message();
	}
	return CWinThread::PreTranslateMessage(pMsg);
}