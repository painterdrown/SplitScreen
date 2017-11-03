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


//������ĸ���Ļ�ĶԻ���
CPictureDlg* syn_dlg1 = NULL;

CPictureDlg* syn_dlg2 = NULL;

CPictureDlg* syn_dlg3 = NULL;

CPictureDlg* syn_dlg4 = NULL;


//��ȡ��������Ƶ�������Ϣ
CString syn_videoPath;
VideoCapture syn_capture;
long totalframe;
long currentframe = 0;

//ÿһ֡������ͼƬ
Mat syn_frame;

//��ÿһ֡ͼƬ�г�4�ݵ�ͼƬ
Mat imageROI1;
Mat imageROI2;
Mat imageROI3;
Mat imageROI4;

//�����жϵ�ǰ����״̬���ź���
bool syn_isOpen = false;

bool syn_isOpenAndDBClick = false;

bool syn_video_play = false;

//����ÿһ֡ͼƬ���ӳ�
int syn_delay = 1;

//����Ļ��
int syn_total_num_of_screen = GetSystemMetrics(SM_CMONITORS);		

//��������
int syn_num_without_PrimaryScreen = syn_total_num_of_screen - 1;

//�洢��Ļ����Ϣ
CRect * syn_rect = new CRect[syn_total_num_of_screen];						


// CSYNDlg �Ի���
IMPLEMENT_DYNAMIC(CSYNDlg, CDialogEx)


//�����Ļ�Ļص�����
BOOL CALLBACK SYN_MonitorEnumProc(HMONITOR hMonitor, HDC hdcMonitor, LPRECT lprcMonitor, LPARAM dwData);


CSYNDlg::CSYNDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(IDD_SYNDLG, pParent)
{
	//��ʼ����ǰ����״̬Ϊ��
	is_play = false;

	//��ȡ���ӵ����Ե���Ļ�����Ϣ
	EnumDisplayMonitors(NULL, NULL, &SYN_MonitorEnumProc, 0);
}


//��������
CSYNDlg::~CSYNDlg()
{
}


BOOL CSYNDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	//��ʼ���Ի���ʱʹ����ͣ��ť������
	CButton *pBtn = (CButton *)GetDlgItem(IDC_SYN_PLAY);
	if (pBtn != NULL)
	{
		pBtn->EnableWindow(FALSE);
	}

	return TRUE;  // ���ǽ��������õ��ؼ������򷵻� TRUE
}

//��ȡ��Ļ��Ϣ�Ļص�����
BOOL CALLBACK SYN_MonitorEnumProc(HMONITOR hMonitor, HDC hdcMonitor, LPRECT lprcMonitor, LPARAM dwData)
{
	//���ڼ�¼��Ļ��Ϣ
	DISPLAY_DEVICE ddDisplay;
	ZeroMemory(&ddDisplay, sizeof(ddDisplay));
	ddDisplay.cb = sizeof(ddDisplay);

	DEVMODE dm;
	ZeroMemory(&dm, sizeof(dm));
	dm.dmSize = sizeof(dm);

	int m = 1;
	//���Ȼ�ȡ����Ļ������Ȼ����б�����������������Ļʱ���Ϊrect[0]��Ȼ������ 
	for (int i = 0; i < syn_total_num_of_screen; i++, m++)
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

		//�����ǰ��⵽�ĵ�i����Ļ������Ļ
		if (monitorInfo.dwFlags == MONITORINFOF_PRIMARY)	
		{
			syn_rect[0].left = dm.dmPosition.x;
			syn_rect[0].top = dm.dmPosition.y;
			syn_rect[0].right = dm.dmPelsWidth;
			syn_rect[0].bottom = dm.dmPelsHeight;
			m--;
		}
		else	 //��������Ļ
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
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	CString fileFilter = _T("ý���ļ�(*.wmv,*.mp3,*.avi,,*.rm,*.rmvb,*.mkv,*.mp4)|*.wmv;*.mp3;*.avi;*.rm;*.rmvb;*.mkv;*.mp4|");
	CFileDialog filedlg(true, NULL, NULL, OFN_HIDEREADONLY | OFN_ALLOWMULTISELECT | OFN_NOCHANGEDIR, fileFilter);

	if (filedlg.DoModal() == IDOK)
	{
		if(syn_num_without_PrimaryScreen <= 0)
		{
			MessageBox(L"δ��⵽��ʾ��", L"����");
		}
		else
		{
			syn_videoPath = filedlg.GetPathName();
			String path(CW2A(syn_videoPath.GetString()));

			//���ͷ���Դ�������´�capture��
			syn_capture.release();
			syn_capture.open(path);
			if (!syn_capture.isOpened())
				MessageBox(L"����Դʧ��!");

			//��ȡ������Ƶ��֡��
			totalframe = static_cast<long>(syn_capture.get(CV_CAP_PROP_FRAME_COUNT));

			if (syn_dlg1 != NULL)
			{
				delete syn_dlg1;
				syn_dlg1 = NULL;
			}
			if (syn_dlg2 != NULL)
			{
				delete syn_dlg2;
				syn_dlg2 = NULL;
			}
			if (syn_dlg3 != NULL)
			{
				delete syn_dlg3;
				syn_dlg3 = NULL;
			}
			if (syn_dlg4 != NULL)
			{
				delete syn_dlg4;
				syn_dlg4 = NULL;
			}

			//����4���Ի���
			if (syn_num_without_PrimaryScreen >= 1)
			{
				syn_dlg1 = new CPictureDlg();
				syn_dlg1->Create(IDD__PICTURE_SHOW, this);
			}
			if (syn_num_without_PrimaryScreen >= 2)
			{
				syn_dlg2 = new CPictureDlg();
				syn_dlg2->Create(IDD__PICTURE_SHOW, this);
			}
			if (syn_num_without_PrimaryScreen >= 3)
			{
				syn_dlg3 = new CPictureDlg();
				syn_dlg3->Create(IDD__PICTURE_SHOW, this);
			}
			if (syn_num_without_PrimaryScreen >= 4)
			{
				syn_dlg4 = new CPictureDlg();
				syn_dlg4->Create(IDD__PICTURE_SHOW, this);
			}
			is_play = true;

			if (syn_num_without_PrimaryScreen == 1)
			{
				MessageBox(L"δ��⵽��ʾ��2, 3, 4", L"����");
			}
			else if (syn_num_without_PrimaryScreen == 2)
			{
				MessageBox(L"δ��⵽��ʾ��3, 4", L"����");
			}
			else if (syn_num_without_PrimaryScreen == 3)
			{
				MessageBox(L"δ��⵽��ʾ��4", L"����");
			}
				
			//������Ӧͬ���Ķ�ʱ������ʱʱ������õ��ӳ�ʱ��һ��
			SetTimer(1, syn_delay, NULL);

			//�ı䰴ť��ʹ��״̬
			GetDlgItem(IDC_SYN_LODE)->SetWindowText(L"�л���ƵԴ");

			GetDlgItem(IDC_SYN_PLAY)->SetWindowText(L"��ͣ");

			//ʹ�ܰ�ť
			CButton *pBtn = (CButton *)GetDlgItem(IDC_SYN_PLAY);
			if (pBtn != NULL)
			{
				pBtn->EnableWindow(TRUE);
			}
		}
	}
}

//��ʱ������
void CSYNDlg::OnTimer(UINT_PTR nIDEvent)
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	//һ��ͬ��ƴ����ʾ
	if (1 == nIDEvent)
	{
		//ѭ����ȡ��Ƶ��ÿһ֡
		if (syn_capture.read(syn_frame))		
		{
			if (currentframe <= totalframe)
			{
				imageROI1 = syn_frame(Range(0, syn_frame.rows / 2), Range(0, syn_frame.cols / 2));
				imageROI2 = syn_frame(Range(0, syn_frame.rows / 2), Range((syn_frame.cols / 2) + 1, syn_frame.cols));
				imageROI3 = syn_frame(Range((syn_frame.rows / 2) + 1, syn_frame.rows), Range(0, syn_frame.cols / 2));
				imageROI4 = syn_frame(Range((syn_frame.rows / 2) + 1, syn_frame.rows), Range((syn_frame.cols / 2) + 1, syn_frame.cols));

				if (syn_num_without_PrimaryScreen >= 1)
				{
					syn_dlg1->MoveWindow(syn_rect[1].left, syn_rect[1].top, syn_rect[1].right, syn_rect[1].bottom, true);
					syn_dlg1->showImage(imageROI1);

					//��ͼƬ���Կ�1����ʾͼƬ
					CDC* pDC1 = GetDlgItem(IDC_SYN_SMALL1)->GetDC();           //���IDC_SMALL_PICTURE1�ؼ��Ĵ���ָ�룬�ٻ�ȡ��ô��ڹ�����������ָ��  
					HDC hDC1 = pDC1->GetSafeHdc();							  // ��ȡ�豸�����ľ��  
					CRect rect1;
					GetDlgItem(IDC_SYN_SMALL1)->GetClientRect(&rect1);		  //��ȡIDC_SMALL_PICTURE4�ؼ�����ʾ��    

					IplImage iplImg1 = IplImage(imageROI1);
					CvvImage cvvImg1;										  //����һ��CvvImage����  
					cvvImg1.CopyOf(&iplImg1);
					cvvImg1.DrawToHDC(hDC1, &rect1);
					cvvImg1.Destroy();
					ReleaseDC(pDC1);
				}

				if (syn_num_without_PrimaryScreen >= 2)
				{
					syn_dlg2->MoveWindow(syn_rect[2].left, syn_rect[2].top, syn_rect[2].right, syn_rect[2].bottom, true);
					syn_dlg2->showImage(imageROI2);

					//��ͼƬ���Կ�2����ʾͼƬ
					CDC* pDC2 = GetDlgItem(IDC_SYN_SMALL2)->GetDC();				 //���IDC_SMALL_PICTURE1�ؼ��Ĵ���ָ�룬�ٻ�ȡ��ô��ڹ�����������ָ��  
					HDC hDC2 = pDC2->GetSafeHdc();								 // ��ȡ�豸�����ľ��  
					CRect rect2;
					GetDlgItem(IDC_SYN_SMALL2)->GetClientRect(&rect2);		 //��ȡIDC_SMALL_PICTURE4�ؼ�����ʾ��    

					IplImage iplImg2 = IplImage(imageROI2);
					CvvImage cvvImg2;										 //����һ��CvvImage����  
					cvvImg2.CopyOf(&iplImg2);
					cvvImg2.DrawToHDC(hDC2, &rect2);
					cvvImg2.Destroy();
					ReleaseDC(pDC2);
				}

				if (syn_num_without_PrimaryScreen >= 3)
				{
					syn_dlg3->MoveWindow(syn_rect[3].left, syn_rect[3].top, syn_rect[3].right, syn_rect[3].bottom, true);
					syn_dlg3->showImage(imageROI3);

					//��ͼƬ���Կ�3����ʾͼƬ
					CDC* pDC3 = GetDlgItem(IDC_SYN_SMALL3)->GetDC();				//���IDC_SMALL_PICTURE1�ؼ��Ĵ���ָ�룬�ٻ�ȡ��ô��ڹ�����������ָ��  
					HDC hDC3 = pDC3->GetSafeHdc();								 // ��ȡ�豸�����ľ��  
					CRect rect3;
					GetDlgItem(IDC_SYN_SMALL3)->GetClientRect(&rect3);		//��ȡIDC_SMALL_PICTURE4�ؼ�����ʾ��    

					IplImage iplImg3 = IplImage(imageROI3);
					CvvImage cvvImg3;										//����һ��CvvImage����  
					cvvImg3.CopyOf(&iplImg3);
					cvvImg3.DrawToHDC(hDC3, &rect3);
					cvvImg3.Destroy();
					ReleaseDC(pDC3);
				}

				if (syn_num_without_PrimaryScreen >= 4)
				{
					syn_dlg4->MoveWindow(syn_rect[4].left, syn_rect[4].top, syn_rect[4].right, syn_rect[4].bottom, true);
					syn_dlg4->showImage(imageROI4);

					//��ͼƬ���Կ�3����ʾͼƬ
					CDC* pDC4 = GetDlgItem(IDC_SYN_SMALL4)->GetDC();				 //���IDC_SMALL_PICTURE1�ؼ��Ĵ���ָ�룬�ٻ�ȡ��ô��ڹ�����������ָ��  
					HDC hDC4 = pDC4->GetSafeHdc();								 // ��ȡ�豸�����ľ��  
					CRect rect4;
					GetDlgItem(IDC_SYN_SMALL4)->GetClientRect(&rect4);		//��ȡIDC_SMALL_PICTURE4�ؼ�����ʾ��    

					IplImage iplImg4 = IplImage(imageROI4);
					CvvImage cvvImg4;										//����һ��CvvImage����  
					cvvImg4.CopyOf(&iplImg4);
					cvvImg4.DrawToHDC(hDC4, &rect4);
					cvvImg4.Destroy();
					ReleaseDC(pDC4);
				}
				currentframe++;
			}
		}
	}
	CDialogEx::OnTimer(nIDEvent);
}


//�����ͣ��ť
void CSYNDlg::OnBnClickedSynPlay()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	//������ťһ��ʱ�������Ƶ���ڲ��ţ�����ͣ����֮�򲥷�
	if (is_play == true)
	{
		//������ͣ
		KillTimer(1);				
		is_play = false;
		GetDlgItem(IDC_SYN_PLAY)->SetWindowText(L"����");
	}
	else
	{
		//������������
		SetTimer(1, syn_delay, NULL);	
		is_play = true;
		GetDlgItem(IDC_SYN_PLAY)->SetWindowText(L"��ͣ");
	}
}


//����˳���ť
void CSYNDlg::OnBnClickedSynReturn()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	//������Դ
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
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	//������Դ
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

	//�رյ�ǰ����
	CDialogEx::OnClose();	

	//�ر������洰��
	AfxGetMainWnd()->SendMessage(WM_CLOSE);		
}