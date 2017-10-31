// SYNDlg.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "SplitScreen.h"
#include "SYNDlg.h"
#include "afxdialogex.h"

#include "MainDlg.h"

#include "CvvImage.h"
#include "Pic.h"
#include "PictureDlg.h"

#include <windows.h>
#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <iostream>

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

using namespace std;
using namespace cv;


CPictureDlg* syn_dlg1 = NULL;
CPictureDlg* syn_dlg2 = NULL;
CPictureDlg* syn_dlg3 = NULL;
CPictureDlg* syn_dlg4 = NULL;

//����ͬ����
CString syn_videoPath;
VideoCapture syn_capture;
long totalframe;
long currentframe = 0;
Mat syn_frame;
Mat imageROI1, imageROI2, imageROI3, imageROI4;
bool syn_isOpen = false;
bool syn_isOpenAndDBClick = false;
bool syn_video_play = false;

int syn_delay = 1;
int syn_total_num_of_screen = GetSystemMetrics(SM_CMONITORS);		//����Ļ��
int syn_num_without_PrimaryScreen = syn_total_num_of_screen - 1;		//��������
CRect * syn_rect = new CRect[syn_total_num_of_screen];					//�洢��Ļ����Ϣ		

// CSYNDlg �Ի���
IMPLEMENT_DYNAMIC(CSYNDlg, CDialogEx)


//�����Ļ�Ļص�����
BOOL CALLBACK SYN_MonitorEnumProc(HMONITOR hMonitor, HDC hdcMonitor, LPRECT lprcMonitor, LPARAM dwData);


CSYNDlg::CSYNDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(IDD_SYNDLG, pParent)
{
	is_play = false;
	EnumDisplayMonitors(NULL, NULL, &SYN_MonitorEnumProc, 0);
}


CSYNDlg::~CSYNDlg()
{
}


BOOL CALLBACK SYN_MonitorEnumProc(HMONITOR hMonitor, HDC hdcMonitor, LPRECT lprcMonitor, LPARAM dwData)
{
	DISPLAY_DEVICE ddDisplay;
	ZeroMemory(&ddDisplay, sizeof(ddDisplay));
	ddDisplay.cb = sizeof(ddDisplay);

	DEVMODE dm;
	ZeroMemory(&dm, sizeof(dm));
	dm.dmSize = sizeof(dm);

	int m = 1;
	for (int i = 0; i < syn_total_num_of_screen; i++, m++)//���Ȼ�ȡ����Ļ������Ȼ����б�����������������Ļʱ���Ϊrect[0]��Ȼ������ 
	{
		//���ÿ����ʾ��������  
		EnumDisplayDevices(NULL, i, &ddDisplay, 0);
		//��������ʾ���������Ϣ���δ洢��rect������
		EnumDisplaySettings(ddDisplay.DeviceName, ENUM_CURRENT_SETTINGS, &dm);

		//������ʾ����Ϣ
		MONITORINFO monitorInfo;
		monitorInfo.cbSize = sizeof(MONITORINFO);

		//�����ʾ����Ϣ������Ϣ���浽monitorinfo��
		GetMonitorInfo(hMonitor, &monitorInfo);
		if (monitorInfo.dwFlags == MONITORINFOF_PRIMARY)	//�����ǰ��⵽�ĵ�i����Ļ������Ļ
		{
			syn_rect[0].left = dm.dmPosition.x;
			syn_rect[0].top = dm.dmPosition.y;
			syn_rect[0].right = dm.dmPelsWidth;
			syn_rect[0].bottom = dm.dmPelsHeight;
			m--;
		}
		else												//��������Ļ
		{
			syn_rect[m].left = dm.dmPosition.x;
			syn_rect[m].top = dm.dmPosition.y;
			syn_rect[m].right = dm.dmPelsWidth;
			syn_rect[m].bottom = dm.dmPelsHeight;
		}
	}
	return TRUE;
}


void CSYNDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CSYNDlg, CDialogEx)
	ON_BN_CLICKED(IDC_SYN_LODE, &CSYNDlg::OnBnClickedSynLode)
	ON_WM_TIMER()
	ON_WM_CLOSE()
	ON_BN_CLICKED(IDC_SYN_PLAY, &CSYNDlg::OnBnClickedSynPlay)
	ON_BN_CLICKED(IDC_SYN_RETURN, &CSYNDlg::OnBnClickedSynReturn)
END_MESSAGE_MAP()


// CSYNDlg ��Ϣ�������

//���������Ƶ��ť
void CSYNDlg::OnBnClickedSynLode()
{
	CString fileFilter = _T("ý���ļ�(*.wmv,*.mp3,*.avi,,*.rm,*.rmvb,*.mkv,*.mp4)|*.wmv;*.mp3;*.avi;*.rm;*.rmvb;*.mkv;*.mp4|");
	CFileDialog filedlg(true, NULL, NULL, OFN_HIDEREADONLY | OFN_ALLOWMULTISELECT | OFN_NOCHANGEDIR, fileFilter);

	if (filedlg.DoModal() == IDOK)
	{
		syn_capture.release();
		syn_videoPath = filedlg.GetPathName();
		String path(CW2A(syn_videoPath.GetString()));

		syn_capture.open(path);
		if (!syn_capture.isOpened())
			MessageBox(L"����Դʧ��!");

		//��ȡ����֡��
		totalframe = static_cast<long>(syn_capture.get(CV_CAP_PROP_FRAME_COUNT));

		syn_dlg1 = new CPictureDlg();
		syn_dlg2 = new CPictureDlg();
		syn_dlg3 = new CPictureDlg();
		syn_dlg4 = new CPictureDlg();
		syn_dlg1->Create(IDD__PICTURE_SHOW, this);
		syn_dlg2->Create(IDD__PICTURE_SHOW, this);
		syn_dlg3->Create(IDD__PICTURE_SHOW, this);
		syn_dlg4->Create(IDD__PICTURE_SHOW, this);
		is_play = true;
		SetTimer(1, syn_delay, NULL);				//������Ӧͬ���Ķ�ʱ������ʱʱ���֡��һ��
		GetDlgItem(IDC_SYN_LODE)->SetWindowText(L"�л���ƵԴ");
	}
	else
		MessageBox(L"δ��⵽��ʾ��", L"����");
}

//��ʱ������
void CSYNDlg::OnTimer(UINT_PTR nIDEvent)
{
	//һ��ͬ��ƴ����ʾ
	if (1 == nIDEvent)
	{
		if (syn_capture.read(syn_frame))		//ѭ����ȡ��Ƶ��ÿһ֡
		{
			if (currentframe <= totalframe)
			{
				imageROI1 = syn_frame(Range(0, syn_frame.rows / 2), Range(0, syn_frame.cols / 2));
				imageROI2 = syn_frame(Range(0, syn_frame.rows / 2), Range((syn_frame.cols / 2) + 1, syn_frame.cols));
				imageROI3 = syn_frame(Range((syn_frame.rows / 2) + 1, syn_frame.rows), Range(0, syn_frame.cols / 2));
				imageROI4 = syn_frame(Range((syn_frame.rows / 2) + 1, syn_frame.rows), Range((syn_frame.cols / 2) + 1, syn_frame.cols));

				syn_dlg1->MoveWindow(syn_rect[1].left, syn_rect[1].top, syn_rect[1].right, syn_rect[1].bottom, true);
				syn_dlg1->showImage(imageROI1);
				syn_dlg2->MoveWindow(syn_rect[2].left, syn_rect[2].top, syn_rect[2].right, syn_rect[2].bottom, true);
				syn_dlg2->showImage(imageROI2);
				syn_dlg3->MoveWindow(syn_rect[3].left, syn_rect[3].top, syn_rect[3].right, syn_rect[3].bottom, true);
				syn_dlg3->showImage(imageROI3);
				syn_dlg4->MoveWindow(syn_rect[4].left, syn_rect[4].top, syn_rect[4].right, syn_rect[4].bottom, true);
				syn_dlg4->showImage(imageROI4);

				//���ĸ�ͼƬ���Կ�����ʾͼƬ
				CDC* pDC = GetDlgItem(IDC_SYN_SMALL1)->GetDC();           //���IDC_SMALL_PICTURE1�ؼ��Ĵ���ָ�룬�ٻ�ȡ��ô��ڹ�����������ָ��  
				HDC hDC = pDC->GetSafeHdc();								 // ��ȡ�豸�����ľ��  
				CRect rect1;
				GetDlgItem(IDC_SYN_SMALL1)->GetClientRect(&rect1);		//��ȡIDC_SMALL_PICTURE4�ؼ�����ʾ��    

				IplImage iplImg1 = IplImage(imageROI1);
				CvvImage cvvImg1;											//����һ��CvvImage����  
				cvvImg1.CopyOf(&iplImg1);
				cvvImg1.DrawToHDC(hDC, &rect1);
				cvvImg1.Destroy();
				ReleaseDC(pDC);

				pDC = GetDlgItem(IDC_SYN_SMALL2)->GetDC();           //���IDC_SMALL_PICTURE1�ؼ��Ĵ���ָ�룬�ٻ�ȡ��ô��ڹ�����������ָ��  
				hDC = pDC->GetSafeHdc();								 // ��ȡ�豸�����ľ��  
				CRect rect2;
				GetDlgItem(IDC_SYN_SMALL2)->GetClientRect(&rect2);		//��ȡIDC_SMALL_PICTURE4�ؼ�����ʾ��    

				IplImage iplImg2 = IplImage(imageROI2);
				CvvImage cvvImg2;											//����һ��CvvImage����  
				cvvImg2.CopyOf(&iplImg2);
				cvvImg2.DrawToHDC(hDC, &rect2);
				cvvImg2.Destroy();
				ReleaseDC(pDC);

				pDC = GetDlgItem(IDC_SYN_SMALL3)->GetDC();           //���IDC_SMALL_PICTURE1�ؼ��Ĵ���ָ�룬�ٻ�ȡ��ô��ڹ�����������ָ��  
				hDC = pDC->GetSafeHdc();								 // ��ȡ�豸�����ľ��  
				CRect rect3;
				GetDlgItem(IDC_SYN_SMALL3)->GetClientRect(&rect3);		//��ȡIDC_SMALL_PICTURE4�ؼ�����ʾ��    

				IplImage iplImg3 = IplImage(imageROI3);
				CvvImage cvvImg3;											//����һ��CvvImage����  
				cvvImg3.CopyOf(&iplImg3);
				cvvImg3.DrawToHDC(hDC, &rect3);
				cvvImg3.Destroy();
				ReleaseDC(pDC);

				pDC = GetDlgItem(IDC_SYN_SMALL4)->GetDC();           //���IDC_SMALL_PICTURE1�ؼ��Ĵ���ָ�룬�ٻ�ȡ��ô��ڹ�����������ָ��  
				hDC = pDC->GetSafeHdc();								 // ��ȡ�豸�����ľ��  
				CRect rect4;
				GetDlgItem(IDC_SYN_SMALL4)->GetClientRect(&rect4);		//��ȡIDC_SMALL_PICTURE4�ؼ�����ʾ��    

				IplImage iplImg4 = IplImage(imageROI4);
				CvvImage cvvImg4;											//����һ��CvvImage����  
				cvvImg4.CopyOf(&iplImg4);
				cvvImg4.DrawToHDC(hDC, &rect4);
				cvvImg4.Destroy();
				ReleaseDC(pDC);
				currentframe++;
			}
		}
	}
	CDialogEx::OnTimer(nIDEvent);
}


//�����ͣ��ť
void CSYNDlg::OnBnClickedSynPlay()
{
	//������ťһ��ʱ�������Ƶ���ڲ��ţ�����ͣ����֮�򲥷�
	if (is_play == true)
	{
		KillTimer(1);				//������ͣ
		is_play = false;
		GetDlgItem(IDC_SYN_PLAY)->SetWindowText(L"����");
	}
	else
	{
		SetTimer(1, syn_delay, NULL);	//������������
		is_play = true;
		GetDlgItem(IDC_SYN_PLAY)->SetWindowText(L"��ͣ");
	}
}


//����˳���ť
void CSYNDlg::OnBnClickedSynReturn()
{
	if (syn_dlg1 != NULL)
	{
		KillTimer(1);
		syn_dlg1->CloseWindow();
		delete syn_dlg1;
		syn_dlg1 = NULL;
	}
	if (syn_dlg2 != NULL)
	{
		KillTimer(1);
		syn_dlg2->CloseWindow();
		delete syn_dlg2;
		syn_dlg2 = NULL;
	}
	if (syn_dlg3 != NULL)
	{
		KillTimer(1);
		syn_dlg3->CloseWindow();
		delete syn_dlg3;
		syn_dlg3 = NULL;
	}
	if (syn_dlg4 != NULL)
	{
		KillTimer(1);
		syn_dlg4->CloseWindow();
		delete syn_dlg4;
		syn_dlg4 = NULL;
	}
	is_play = false;
	CDialogEx::OnCancel();
}


//������Ͻǹرհ�ť
void CSYNDlg::OnClose()
{
	if (syn_dlg1 != NULL)
	{
		KillTimer(1);
		syn_dlg1->CloseWindow();
		delete syn_dlg1;
		syn_dlg1 = NULL;
	}
	if (syn_dlg2 != NULL)
	{
		KillTimer(1);
		syn_dlg2->CloseWindow();
		delete syn_dlg2;
		syn_dlg2 = NULL;
	}
	if (syn_dlg3 != NULL)
	{
		KillTimer(1);
		syn_dlg3->CloseWindow();
		delete syn_dlg3;
		syn_dlg3 = NULL;
	}
	if (syn_dlg4 != NULL)
	{
		KillTimer(1);
		syn_dlg4->CloseWindow();
		delete syn_dlg4;
		syn_dlg4 = NULL;
	}
	is_play = false;
	CDialogEx::OnClose();						//�رյ�ǰ����
	AfxGetMainWnd()->SendMessage(WM_CLOSE);		//�ر������洰��
}