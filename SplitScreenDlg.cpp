// SplitScreenDlg.cpp : ʵ���ļ�

#include "stdafx.h"
#include "SplitScreen.h"
#include "SplitScreenDlg.h"
#include "MainDlg.h"
#include "afxdialogex.h"

#include "CvvImage.h"
#include "Pic.h"
#include "UiThread.h"

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

struct winPt 
{
	CDC* pDC;
	CRect rect;
};
winPt* pt = new winPt[4];		//�����ĸ��߳�ʹ�õĲ���
 
CPictureDlg* dlg1 = NULL;
CPictureDlg* dlg2 = NULL;
CPictureDlg* dlg3 = NULL;
CPictureDlg* dlg4 = NULL;

bool close_thread1 = false;
bool close_thread2 = false;
bool close_thread3 = false;
bool close_thread4 = false;


int delay = 1;		//�൱��ÿ��30֡
int total_num_of_screen = GetSystemMetrics(SM_CMONITORS);		//����Ļ��
int num_without_PrimaryScreen = total_num_of_screen - 1;		//��������
CRect * rect = new CRect[total_num_of_screen];					//�洢��Ļ����Ϣ									


//���4������ͼ����֮�󵯳���4��ȫ���Ի���
CPictureDlg* expanse1 = NULL;			
CPictureDlg* expanse2 = NULL;
CPictureDlg* expanse3 = NULL;
CPictureDlg* expanse4 = NULL;

//��Ӧ4�����ŵ���Ƶ����ͼƬ�Ķ���
Pic * pic = new Pic[4];					

//�����Ļ�Ļص�����
BOOL CALLBACK MonitorEnumProc(HMONITOR hMonitor, HDC hdcMonitor, LPRECT lprcMonitor, LPARAM dwData);


// ����Ӧ�ó��򡰹��ڡ��˵���� CAboutDlg �Ի���
class CAboutDlg : public CDialogEx
{
public:
	CAboutDlg();

// �Ի�������
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_ABOUTBOX };
#endif

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

// ʵ��
protected:
	DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() : CDialogEx(IDD_ABOUTBOX)
{
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialogEx)
END_MESSAGE_MAP()

//���캯��
CSplitScreenDlg::CSplitScreenDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(IDD_SPLITSCREEN_DIALOG, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);

}

//��������
CSplitScreenDlg::~CSplitScreenDlg()
{
	
}


void CSplitScreenDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CSplitScreenDlg, CDialogEx)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_STN_DBLCLK(IDC_SMALL_PICTURE1, &CSplitScreenDlg::OnStnDblclickSmallPicture1)
	ON_STN_DBLCLK(IDC_SMALL_PICTURE2, &CSplitScreenDlg::OnStnDblclickSmallPicture2)
	ON_STN_DBLCLK(IDC_SMALL_PICTURE3, &CSplitScreenDlg::OnStnDblclickSmallPicture3)
	ON_STN_DBLCLK(IDC_SMALL_PICTURE4, &CSplitScreenDlg::OnStnDblclickSmallPicture4)
	ON_STN_CLICKED(IDC_SMALL_PICTURE1, &CSplitScreenDlg::OnStnClickedSmallPicture1)
	ON_STN_CLICKED(IDC_SMALL_PICTURE2, &CSplitScreenDlg::OnStnClickedSmallPicture2)
	ON_STN_CLICKED(IDC_SMALL_PICTURE3, &CSplitScreenDlg::OnStnClickedSmallPicture3)
	ON_STN_CLICKED(IDC_SMALL_PICTURE4, &CSplitScreenDlg::OnStnClickedSmallPicture4)
	ON_BN_CLICKED(IDCANCEL, &CSplitScreenDlg::OnBnClickedCancel)
	ON_BN_CLICKED(IDC_CLOSE1, &CSplitScreenDlg::OnBnClickedClose1)
	ON_BN_CLICKED(IDC_CLOSE2, &CSplitScreenDlg::OnBnClickedClose2)
	ON_BN_CLICKED(IDC_CLOSE3, &CSplitScreenDlg::OnBnClickedClose3)
	ON_BN_CLICKED(IDC_CLOSE4, &CSplitScreenDlg::OnBnClickedClose4)
	ON_WM_CLOSE()
	ON_NOTIFY(NM_CLICK, IDC_RETURN, &CSplitScreenDlg::OnNMClickSyslink1)
	ON_BN_CLICKED(IDC_IMPORT1, &CSplitScreenDlg::OnBnClickedImport1)
	ON_BN_CLICKED(IDC_IMPORT2, &CSplitScreenDlg::OnBnClickedImport2)
	ON_BN_CLICKED(IDC_IMPORT3, &CSplitScreenDlg::OnBnClickedImport3)
	ON_BN_CLICKED(IDC_IMPORT4, &CSplitScreenDlg::OnBnClickedImport4)
	ON_BN_CLICKED(IDC_STOP2, &CSplitScreenDlg::OnBnClickedStop2)
	ON_BN_CLICKED(IDC_STOP1, &CSplitScreenDlg::OnBnClickedStop1)
	ON_BN_CLICKED(IDC_STOP3, &CSplitScreenDlg::OnBnClickedStop3)
	ON_BN_CLICKED(IDC_STOP4, &CSplitScreenDlg::OnBnClickedStop4)
	ON_WM_TIMER()
END_MESSAGE_MAP()


// CSplitScreenDlg ��Ϣ�������
BOOL CSplitScreenDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// ��������...���˵�����ӵ�ϵͳ�˵��С�
	// IDM_ABOUTBOX ������ϵͳ���Χ�ڡ�
	ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
	ASSERT(IDM_ABOUTBOX < 0xF000);

	CMenu* pSysMenu = GetSystemMenu(FALSE);
	if (pSysMenu != NULL)
	{
		BOOL bNameValid;
		CString strAboutMenu;
		bNameValid = strAboutMenu.LoadString(IDS_ABOUTBOX);
		ASSERT(bNameValid);
		if (!strAboutMenu.IsEmpty())
		{
			pSysMenu->AppendMenu(MF_SEPARATOR);
			pSysMenu->AppendMenu(MF_STRING, IDM_ABOUTBOX, strAboutMenu);
		}
	}
	// ���ô˶Ի����ͼ�ꡣ��Ӧ�ó��������ڲ��ǶԻ���ʱ����ܽ��Զ�ִ�д˲���
	SetIcon(m_hIcon, TRUE);			// ���ô�ͼ��
	SetIcon(m_hIcon, FALSE);		// ����Сͼ��


	//��ȡ��ʾ�������Ϣ
	EnumDisplayMonitors(NULL, NULL, &MonitorEnumProc, 0);

	//��ʼ���Ի���ʱʹ����ͣ��ť1������
	CButton *pBtn1 = (CButton *)GetDlgItem(IDC_STOP2);
	if (pBtn1 != NULL)
	{
		pBtn1->EnableWindow(FALSE);
	}

	//��ʼ���Ի���ʱʹ����ͣ��ť2������
	CButton *pBtn2 = (CButton *)GetDlgItem(IDC_STOP1);
	if (pBtn2 != NULL)
	{
		pBtn2->EnableWindow(FALSE);
	}

	//��ʼ���Ի���ʱʹ����ͣ��ť3������
	CButton *pBtn3 = (CButton *)GetDlgItem(IDC_STOP3);
	if (pBtn3 != NULL)
	{
		pBtn3->EnableWindow(FALSE);
	}

	//��ʼ���Ի���ʱʹ����ͣ��ť4������
	CButton *pBtn4 = (CButton *)GetDlgItem(IDC_STOP4);
	if (pBtn4 != NULL)
	{
		pBtn4->EnableWindow(FALSE);
	}
	return TRUE;  // ���ǽ��������õ��ؼ������򷵻� TRUE
}


//EnumDisplayMonitors�Ļص�����
BOOL CALLBACK MonitorEnumProc(HMONITOR hMonitor, HDC hdcMonitor, LPRECT lprcMonitor, LPARAM dwData)
{
	DISPLAY_DEVICE ddDisplay;
	ZeroMemory(&ddDisplay, sizeof(ddDisplay));
	ddDisplay.cb = sizeof(ddDisplay);

	DEVMODE dm;
	ZeroMemory(&dm, sizeof(dm));
	dm.dmSize = sizeof(dm);

	int m = 1;
	for (int i = 0; i < total_num_of_screen; i++, m++)//���Ȼ�ȡ����Ļ������Ȼ����б�����������������Ļʱ���Ϊrect[0]��Ȼ������ 
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
			rect[0].left = dm.dmPosition.x;
			rect[0].top = dm.dmPosition.y;
			rect[0].right = dm.dmPelsWidth;
			rect[0].bottom = dm.dmPelsHeight;
			m--;
		}
		else												//��������Ļ
		{
			rect[m].left = dm.dmPosition.x;
			rect[m].top = dm.dmPosition.y;
			rect[m].right = dm.dmPelsWidth;
			rect[m].bottom = dm.dmPelsHeight;
		}	
	}
	return TRUE;
}

//BOOL CALLBACK MonitorEnumProc(HMONITOR hMonitor, HDC hdcMonitor, LPRECT lprcMonitor, LPARAM dwData)
//{
//	//������ʾ����Ϣ
//	MONITORINFO monitorinfo;
//	monitorinfo.cbSize = sizeof(MONITORINFO);
//
//	//�����ʾ����Ϣ������Ϣ���浽monitorinfo��
//	GetMonitorInfo(hMonitor, &monitorinfo);
//
//	//����⵽����
//	if (monitorinfo.dwFlags == MONITORINFOF_PRIMARY)
//	{
//		//����ʾ���ķֱ�����Ϣ���浽rect[0]
//		rect[0] = monitorinfo.rcMonitor;
//	}
//	else //��⵽������Ļ
//	{
//		rect[m] = monitorinfo.rcMonitor;
//		m++;
//	}
//	return TRUE;
//}

void CSplitScreenDlg::OnSysCommand(UINT nID, LPARAM lParam)
{
	if ((nID & 0xFFF0) == IDM_ABOUTBOX)
	{
		CAboutDlg dlgAbout;
		dlgAbout.DoModal();
	}
	else
	{
		CDialogEx::OnSysCommand(nID, lParam);
	}
}

// �����Ի��������С����ť������Ҫ����Ĵ���
//  �����Ƹ�ͼ�ꡣ  ����ʹ���ĵ�/��ͼģ�͵� MFC Ӧ�ó���
//  �⽫�ɿ���Զ���ɡ�

void CSplitScreenDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // ���ڻ��Ƶ��豸������

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// ʹͼ���ڹ����������о���
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// ����ͼ��
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialogEx::OnPaint();
	}
}


//���û��϶���С������ʱϵͳ���ô˺���ȡ�ù����ʾ��
HCURSOR CSplitScreenDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}


//�̺߳���1
UINT ThreadProc1(LPVOID pM)
{
	//��ȡ�̴߳������Ĳ���
	winPt* pt = (winPt*)pM;			

	pic[0].capture.open(pic[0].videoPath);
	if (!pic[0].capture.isOpened())
		AfxMessageBox(L"����Դʧ��!");

	//��ȡ��Ƶ��֡��
	pic[0].totalFrameNumber = static_cast<long>(pic[0].capture.get(CV_CAP_PROP_FRAME_COUNT));

	//���Ի��������������ʾ��
	dlg1->MoveWindow(rect[1].left, rect[1].top, rect[1].right, rect[1].bottom, true);

	while (1)
	{
		//ѭ����ȡ��Ƶ��ÿһ֡
		if (pic[0].capture.read(pic[0].frame))	
		{
			//����Ļ1����ʾ
			dlg1->showImage(pic[0].frame);

			//�������ڵĵ�1��ͼƬ���Կ�����ʾ
			dlg1->ShowSmallPic(pt->pDC, pic[0].frame, pt->rect);

			//˫��������ͼ1��Ŵ�����ͼ1
			if (expanse1 != NULL && expanse1->openExpanseDlg == true)
			{
				expanse1->showImage(pic[0].frame);
			}
		}
		else		//��Ƶ������ϻ��߶�ȡ֡����
		{
			break;
		}			
		if (close_thread1 == true)
		{
			return 0;
		}

		//��Ƶ����ÿ֡�������λ:ms��
		Sleep(delay);		
	}
	return 0;
}


//�̺߳���2
UINT ThreadProc2(LPVOID pM)
{
	//��ȡ�̴߳������Ĳ���
	winPt* pt = (winPt*)pM;			

	pic[1].capture.open(pic[1].videoPath);
	if (!pic[1].capture.isOpened())
		AfxMessageBox(L"����Դʧ��!");

	//��ȡ��Ƶ��֡��
	pic[1].totalFrameNumber = static_cast<long>(pic[1].capture.get(CV_CAP_PROP_FRAME_COUNT));

	//���Ի��������������ʾ��
	dlg2->MoveWindow(rect[2].left, rect[2].top, rect[2].right, rect[2].bottom, true);

	while (close_thread2 == false)
	{
		//ѭ����ȡ��Ƶ��ÿһ֡
		if (pic[1].capture.read(pic[1].frame))	
		{
			//����Ļ2����ʾ
			dlg2->showImage(pic[1].frame);

			//�������ڵĵ�2��ͼƬ���Կ�����ʾ
			dlg2->ShowSmallPic(pt->pDC, pic[1].frame, pt->rect);

			//˫��������ͼ2��Ŵ�����ͼ2
			if (expanse2 != NULL && expanse2->openExpanseDlg == true)
			{
				expanse2->showImage(pic[1].frame);
			}
		}
		else		//��Ƶ������ϻ��߶�ȡ֡����
		{
			break;
		}		
		if (close_thread2 == true)
		{
			return 0;
		}

		//��Ƶ����ÿ֡�������λ:ms��
		Sleep(delay);		
	}
	return 0;
}


//�̺߳���3
UINT ThreadProc3(LPVOID pM)
{
	//��ȡ�̴߳������Ĳ���
	winPt* pt = (winPt*)pM;

	pic[2].capture.open(pic[2].videoPath);
	if (!pic[2].capture.isOpened())
		AfxMessageBox(L"����Դʧ��!");

	//��ȡ��Ƶ��֡��
	pic[2].totalFrameNumber = static_cast<long>(pic[2].capture.get(CV_CAP_PROP_FRAME_COUNT));

	//���Ի��������������ʾ��
	dlg3->MoveWindow(rect[3].left, rect[3].top, rect[3].right, rect[3].bottom, true);

	while (close_thread3 == false)
	{
		//ѭ����ȡ��Ƶ��ÿһ֡
		if (pic[2].capture.read(pic[2].frame))	
		{
			//����Ļ3����ʾ
			dlg3->showImage(pic[2].frame);

			//�������ڵĵ�3��ͼƬ���Կ�����ʾ
			dlg3->ShowSmallPic(pt->pDC, pic[2].frame, pt->rect);

			//˫��������ͼ3��Ŵ�����ͼ3
			if (expanse3 != NULL && expanse3->openExpanseDlg == true)
			{
				expanse3->showImage(pic[2].frame);
			}
		}
		else		//��Ƶ������ϻ��߶�ȡ֡����
		{
			break;
		}
		if (close_thread3 == true)
		{
			return 0;
		}

		//��Ƶ����ÿ֡�������λ:ms��
		Sleep(delay);		
	}
	return 0;
}

//�̺߳���4
UINT ThreadProc4(LPVOID pM)
{
	//��ȡ�̴߳������Ĳ���
	winPt* pt = (winPt*)pM;

	pic[3].capture.open(pic[3].videoPath);
	if (!pic[3].capture.isOpened())
		AfxMessageBox(L"����Դʧ��!");

	//��ȡ��Ƶ��֡��
	pic[3].totalFrameNumber = static_cast<long>(pic[3].capture.get(CV_CAP_PROP_FRAME_COUNT));

	//���Ի��������������ʾ��
	dlg4->MoveWindow(rect[4].left, rect[4].top, rect[4].right, rect[4].bottom, true);

	while (close_thread4 == false)
	{
		//ѭ����ȡ��Ƶ��ÿһ֡
		if (pic[3].capture.read(pic[3].frame))	
		{
			//����Ļ4����ʾ
			dlg4->showImage(pic[3].frame);

			//�������ڵĵ�4��ͼƬ���Կ�����ʾ
			dlg4->ShowSmallPic(pt->pDC, pic[3].frame, pt->rect);

			//˫��������ͼ4��Ŵ�����ͼ4
			if (expanse4 != NULL && expanse4->openExpanseDlg == true)
			{
				expanse4->showImage(pic[3].frame);
			}
		}
		else		//��Ƶ������ϻ��߶�ȡ֡����			
		{
			break;
		}		
		if (close_thread4 == true)
		{
			return 0;
		}

		//��Ƶ����ÿ֡�������λ:ms��
		Sleep(delay);		
	}
	return 0;
}


//������ͼ1��˫��
void CSplitScreenDlg::OnStnDblclickSmallPicture1()
{
	//�������ͼ1�Ѿ��ڲ�����Ƶ�ˣ���˫����֮���Ŵ�����ͼ����
	if (pic[0].isOpen == true)
	{
		if (expanse1 != NULL)
		{
			expanse1->CloseWindow();
			delete expanse1;
			expanse1 = NULL;
		}
		expanse1 = new CPictureDlg();
		expanse1->Create(IDD__PICTURE_SHOW, this);
		expanse1->SetWindowText(L"��Ļһ��ȫ��");

		//������Ļ��ȫ����ʾ
		expanse1->MoveWindow(rect[0].left, rect[0].top, rect[0].right, rect[0].bottom, true);	
		SetWindowLong(expanse1->GetSafeHwnd(), GWL_STYLE, GetWindowLong(expanse1->m_hWnd, GWL_STYLE) + WS_CAPTION);

		expanse1->openExpanseDlg = true;
	}
	//�������ͼ1û���ڲ�����Ƶ����˫����֮�����ʾ�û�������Դ
	else
	{
		CString fileFilter = _T("ý���ļ�(*.wmv,*.mp3,*.avi,,*.rm,*.rmvb,*.mkv,*.mp4)|*.wmv;*.mp3;*.avi;*.rm;*.rmvb;*.mkv;*.mp4|");
		CFileDialog filedlg(true, NULL, NULL, OFN_HIDEREADONLY | OFN_ALLOWMULTISELECT | OFN_NOCHANGEDIR, fileFilter);

		if (filedlg.DoModal() == IDOK)
		{
			if (num_without_PrimaryScreen >= 1 && pic[0].isOpen == false)
			{
				if (NULL == dlg1)
				{
					dlg1 = new CPictureDlg();
					dlg1->Create(IDD__PICTURE_SHOW, this);
				}

				CString cpath = filedlg.GetPathName();
				pic[0].videoPath = CW2A(cpath.GetString());				
				pic[0].isOpen = true;									
				pic[0].video_play = true;								
																			
				CDC* pDC = GetDlgItem(IDC_SMALL_PICTURE1)->GetDC();         
				CRect rect;
				GetDlgItem(IDC_SMALL_PICTURE1)->GetClientRect(&rect);		
				pt[0].pDC = pDC;
				pt[0].rect = rect;

				//�����߳�1��������Ƶ1
				close_thread1 = false;
				pic_pthread1 = AfxBeginThread(ThreadProc1, &pt[0]);

				//ʹ�ܵ��밴ť
				CButton *pBtn = (CButton *)GetDlgItem(IDC_IMPORT1);
				if (pBtn != NULL)
				{
					pBtn->EnableWindow(FALSE);
				}

				//ʹ����ͣ��ť
				CButton *pBtn1 = (CButton *)GetDlgItem(IDC_STOP2);
				if (pBtn1 != NULL)
				{
					pBtn1->EnableWindow(TRUE);
				}
			}
			else
			{
				MessageBox(L"δ��⵽��ʾ��1", L"����");
			}
		}
	}
}


//������ͼ2��˫��
void CSplitScreenDlg::OnStnDblclickSmallPicture2()
{
	//�������ͼ2�Ѿ��ڲ�����Ƶ�ˣ���˫����֮���Ŵ�����ͼ����
	if (pic[1].isOpen == true)
	{
		if (expanse2 != NULL)
		{
			expanse2->CloseWindow();
			delete expanse2;
			expanse2 = NULL;
		}
		expanse2 = new CPictureDlg();
		expanse2->Create(IDD__PICTURE_SHOW, this);
		expanse2->SetWindowText(L"��Ļ����ȫ��");

		//������Ļ��ȫ����ʾ
		expanse2->MoveWindow(rect[0].left, rect[0].top, rect[0].right, rect[0].bottom, true);	
		SetWindowLong(expanse2->GetSafeHwnd(), GWL_STYLE, GetWindowLong(expanse2->m_hWnd, GWL_STYLE) + WS_CAPTION);

		expanse2->openExpanseDlg = true;
	}
	//�������ͼ2û���ڲ�����Ƶ����˫����֮�����ʾ�û�������Դ
	else
	{
		CString fileFilter = _T("ý���ļ�(*.wmv,*.mp3,*.avi,,*.rm,*.rmvb,*.mkv,*.mp4)|*.wmv;*.mp3;*.avi;*.rm;*.rmvb;*.mkv;*.mp4|");
		CFileDialog filedlg(true, NULL, NULL, OFN_HIDEREADONLY | OFN_ALLOWMULTISELECT | OFN_NOCHANGEDIR, fileFilter);

		if (filedlg.DoModal() == IDOK)
		{
			if (num_without_PrimaryScreen >= 2 && pic[1].isOpen == false)
			{
				if (NULL == dlg2)
				{
					dlg2 = new CPictureDlg();
					dlg2->Create(IDD__PICTURE_SHOW, this);
				}

				CString cpath = filedlg.GetPathName();
				pic[1].videoPath = CW2A(cpath.GetString());					
				pic[1].isOpen = true;										
				pic[1].video_play = true;									
										
				CDC* pDC = GetDlgItem(IDC_SMALL_PICTURE2)->GetDC();          
				CRect rect;
				GetDlgItem(IDC_SMALL_PICTURE2)->GetClientRect(&rect);		
				pt[1].pDC = pDC;
				pt[1].rect = rect;

				//�����߳�2��������Ƶ2
				close_thread2 = false;
				pic_pthread2 = AfxBeginThread(ThreadProc2, &pt[1]);

				//ʹ�ܵ��밴ť
				CButton *pBtn = (CButton *)GetDlgItem(IDC_IMPORT2);
				if (pBtn != NULL)
				{
					pBtn->EnableWindow(FALSE);
				}

				//ʹ����ͣ��ť
				CButton *pBtn2 = (CButton *)GetDlgItem(IDC_STOP1);
				if (pBtn2 != NULL)
				{
					pBtn2->EnableWindow(TRUE);
				}
			}
			else
			{
				MessageBox(L"δ��⵽��ʾ��2", L"����");
			}
		}
	}
}


//������ͼ3��˫��
void CSplitScreenDlg::OnStnDblclickSmallPicture3()
{
	//�������ͼ3�Ѿ��ڲ�����Ƶ�ˣ���˫����֮���Ŵ�����ͼ����
	if (pic[2].isOpen == true)
	{
		if (expanse3 != NULL)
		{
			expanse3->CloseWindow();
			delete expanse3;
			expanse3 = NULL;
		}
		expanse3 = new CPictureDlg();
		expanse3->Create(IDD__PICTURE_SHOW, this);
		expanse3->SetWindowText(L"��Ļ����ȫ��");

		//������Ļ��ȫ����ʾ
		expanse3->MoveWindow(rect[0].left, rect[0].top, rect[0].right, rect[0].bottom, true);	
		SetWindowLong(expanse3->GetSafeHwnd(), GWL_STYLE, GetWindowLong(expanse3->m_hWnd, GWL_STYLE) + WS_CAPTION);

		expanse3->openExpanseDlg = true;
	}
	//�������ͼ3û���ڲ�����Ƶ����˫����֮�����ʾ�û�������Դ
	else
	{
		CString fileFilter = _T("ý���ļ�(*.wmv,*.mp3,*.avi,,*.rm,*.rmvb,*.mkv,*.mp4)|*.wmv;*.mp3;*.avi;*.rm;*.rmvb;*.mkv;*.mp4|");
		CFileDialog filedlg(true, NULL, NULL, OFN_HIDEREADONLY | OFN_ALLOWMULTISELECT | OFN_NOCHANGEDIR, fileFilter);

		if (filedlg.DoModal() == IDOK)
		{
			if (num_without_PrimaryScreen >= 3 && pic[2].isOpen == false)
			{
				if (NULL == dlg3)
				{
					dlg3 = new CPictureDlg();
					dlg3->Create(IDD__PICTURE_SHOW, this);
				}

				CString cpath = filedlg.GetPathName();
				pic[2].videoPath = CW2A(cpath.GetString());					
				pic[2].isOpen = true;										
				pic[2].video_play = true;									
									
				CDC* pDC = GetDlgItem(IDC_SMALL_PICTURE3)->GetDC();         
				CRect rect;
				GetDlgItem(IDC_SMALL_PICTURE3)->GetClientRect(&rect);		
				pt[2].pDC = pDC;
				pt[2].rect = rect;

				//�����߳�3��������Ƶ3
				close_thread3 = false;
				pic_pthread3 = AfxBeginThread(ThreadProc3, &pt[2]);

				//ʹ�ܵ��밴ť
				CButton *pBtn = (CButton *)GetDlgItem(IDC_IMPORT3);
				if (pBtn != NULL)
				{
					pBtn->EnableWindow(FALSE);
				}

				//ʹ����ͣ��ť
				CButton *pBtn3 = (CButton *)GetDlgItem(IDC_STOP3);
				if (pBtn3 != NULL)
				{
					pBtn3->EnableWindow(TRUE);
				}
			}
			else
			{
				MessageBox(L"δ��⵽��ʾ��3", L"����");
			}
		}
	}
}


//������ͼ4��˫��
void CSplitScreenDlg::OnStnDblclickSmallPicture4()
{
	//�������ͼ4�Ѿ��ڲ�����Ƶ�ˣ���˫����֮���Ŵ�����ͼ����
	if (pic[3].isOpen == true)
	{
		if (expanse4 != NULL)
		{
			expanse4->CloseWindow();
			delete expanse4;
			expanse4 = NULL;
		}
		expanse4 = new CPictureDlg();
		expanse4->Create(IDD__PICTURE_SHOW, this);
		expanse4->SetWindowText(L"��Ļ�ĵ�ȫ��");

		//������Ļ��ȫ����ʾ
		expanse4->MoveWindow(rect[0].left, rect[0].top, rect[0].right, rect[0].bottom, true);	
		SetWindowLong(expanse4->GetSafeHwnd(), GWL_STYLE, GetWindowLong(expanse4->m_hWnd, GWL_STYLE) + WS_CAPTION);

		expanse4->openExpanseDlg = true;
	}
	//�������ͼ4û���ڲ�����Ƶ����˫����֮�����ʾ�û�������Դ
	else
	{
		CString fileFilter = _T("ý���ļ�(*.wmv,*.mp3,*.avi,,*.rm,*.rmvb,*.mkv,*.mp4)|*.wmv;*.mp3;*.avi;*.rm;*.rmvb;*.mkv;*.mp4|");
		CFileDialog filedlg(true, NULL, NULL, OFN_HIDEREADONLY | OFN_ALLOWMULTISELECT | OFN_NOCHANGEDIR, fileFilter);

		if (filedlg.DoModal() == IDOK)
		{
			if (num_without_PrimaryScreen >= 4 && pic[3].isOpen == false)
			{
				if (NULL == dlg4)
				{
					dlg4 = new CPictureDlg();
					dlg4->Create(IDD__PICTURE_SHOW, this);
				}

				CString cpath = filedlg.GetPathName();
				pic[3].videoPath = CW2A(cpath.GetString());					
				pic[3].isOpen = true;										
				pic[3].video_play = true;									

				//��ȡ�������ϵ�4������ͼ����ؾ����ָ��
				CDC* pDC = GetDlgItem(IDC_SMALL_PICTURE4)->GetDC();         
				CRect rect;
				GetDlgItem(IDC_SMALL_PICTURE4)->GetClientRect(&rect);	
				pt[3].pDC = pDC;
				pt[3].rect = rect;

				//�����߳�4��������Ƶ4
				close_thread4 = false;
				pic_pthread4 = AfxBeginThread(ThreadProc4, &pt[3]);

				//ʹ�ܵ��밴ť
				CButton *pBtn = (CButton *)GetDlgItem(IDC_IMPORT4);
				if (pBtn != NULL)
				{
					pBtn->EnableWindow(FALSE);
				}

				//ʹ����ͣ��ť
				CButton *pBtn4 = (CButton *)GetDlgItem(IDC_STOP4);
				if (pBtn4 != NULL)
				{
					pBtn4->EnableWindow(TRUE);
				}
			}
			else
			{
				MessageBox(L"δ��⵽��ʾ��4", L"����");
			}
		}
	}
}


//������ͼ1�ϵ���
void CSplitScreenDlg::OnStnClickedSmallPicture1()
{
	MSG message;
	DWORD st = GetTickCount();

	while (1)
	{
		if (::PeekMessage(&message, NULL, 0, 0, PM_REMOVE))
		{
			::TranslateMessage(&message);
			::DispatchMessage(&message);

			//����ʱ�����˫���¼��򷵻�ִ��˫���¼�
			if (message.message == WM_LBUTTONDBLCLK)
			{
				return;
			}			
		}
		DWORD et = GetTickCount();
		if (et - st > 200)
		{
			break;
		}		
	}

	//��������ͼһ��ʱ�������Ƶ���ڲ��ţ�����ͣ����֮�򲥷�
	if (pic[0].isOpen == true && pic[0].video_play == true)
	{
		//������ͣ
		pic_pthread1->SuspendThread();
		pic[0].video_play = false;

		GetDlgItem(IDC_STOP2)->SetWindowText(L"����");
	}
	else if (pic[0].isOpen == true && pic[0].video_play == false)
	{
		//������������
		pic_pthread1->ResumeThread();
		pic[0].video_play = true;

		GetDlgItem(IDC_STOP2)->SetWindowText(L"��ͣ");
	}
}


//������ͼ2�ϵ���
void CSplitScreenDlg::OnStnClickedSmallPicture2()
{
	MSG message;
	DWORD st = GetTickCount();

	while (1)
	{
		if (::PeekMessage(&message, NULL, 0, 0, PM_REMOVE))
		{
			::TranslateMessage(&message);
			::DispatchMessage(&message);

			//����ʱ�����˫���¼��򷵻�ִ��˫���¼�
			if (message.message == WM_LBUTTONDBLCLK)
			{
				return;
			}
		}
		DWORD et = GetTickCount();
		if (et - st > 200)
		{
			break;
		}			
	}

	//��������ͼһ��ʱ�������Ƶ���ڲ��ţ�����ͣ����֮�򲥷�
	if (pic[1].isOpen == true && pic[1].video_play == true)
	{
		//������ͣ
		pic_pthread2->SuspendThread();
		pic[1].video_play = false;

		GetDlgItem(IDC_STOP1)->SetWindowText(L"����");
	}
	else if (pic[1].isOpen == true && pic[1].video_play == false)
	{
		//������������
		pic_pthread2->ResumeThread();
		pic[1].video_play = true;

		GetDlgItem(IDC_STOP1)->SetWindowText(L"��ͣ");
	}
}


//������ͼ3�ϵ���
void CSplitScreenDlg::OnStnClickedSmallPicture3()
{
	MSG message;
	DWORD st = GetTickCount();

	while (1)
	{
		if (::PeekMessage(&message, NULL, 0, 0, PM_REMOVE))
		{
			::TranslateMessage(&message);
			::DispatchMessage(&message);

			//����ʱ�����˫���¼��򷵻�ִ��˫���¼�
			if (message.message == WM_LBUTTONDBLCLK)
			{
				return;
			}			
		}
		DWORD et = GetTickCount();
		if (et - st > 200)
		{
			break;
		}		
	}

	//��������ͼһ��ʱ�������Ƶ���ڲ��ţ�����ͣ����֮�򲥷�
	if (pic[2].isOpen == true && pic[2].video_play == true)
	{
		//������ͣ
		pic_pthread3->SuspendThread();
		pic[2].video_play = false;

		GetDlgItem(IDC_STOP3)->SetWindowText(L"����");
	}
	else if (pic[2].isOpen == true && pic[2].video_play == false)
	{
		//������������
		pic_pthread3->ResumeThread();
		pic[2].video_play = true;

		GetDlgItem(IDC_STOP3)->SetWindowText(L"��ͣ");
	}
}


//������ͼ4�ϵ���
void CSplitScreenDlg::OnStnClickedSmallPicture4()
{
	MSG message;
	DWORD st = GetTickCount();

	while (1)
	{
		if (::PeekMessage(&message, NULL, 0, 0, PM_REMOVE))
		{
			::TranslateMessage(&message);
			::DispatchMessage(&message);

			//����ʱ�����˫���¼��򷵻�ִ��˫���¼�
			if (message.message == WM_LBUTTONDBLCLK)
			{
				return;
			}	
		}
		DWORD et = GetTickCount();
		if (et - st > 200)
		{
			break;
		}		
	}

	//��������ͼһ��ʱ�������Ƶ���ڲ��ţ�����ͣ����֮�򲥷�
	if (pic[3].isOpen == true && pic[3].video_play == true)
	{
		//������ͣ
		pic_pthread4->SuspendThread();
		pic[3].video_play = false;

		GetDlgItem(IDC_STOP4)->SetWindowText(L"����");
	}
	else if (pic[3].isOpen == true && pic[3].video_play == false)
	{
		//������������
		pic_pthread4->ResumeThread();
		pic[3].video_play = true;

		GetDlgItem(IDC_STOP4)->SetWindowText(L"��ͣ");
	}
}


//��ʱ�������ڶ�ʱ�����Ƶ��ÿһ֡
void CSplitScreenDlg::OnTimer(UINT_PTR nIDEvent)
{
	// TODO: �ڴ������Ϣ�����������/�����Ĭ��ֵ

	//��Ļһ�Ķ�ʱ��
	if (1 == nIDEvent)
	{
		if (pic[0].capture.read(pic[0].frame))	
		{
			if (pic[0].currentFrame <= pic[0].totalFrameNumber)
			{
				//����Ļһ����ʾ
				dlg1->MoveWindow(rect[1].left, rect[1].top, rect[1].right, rect[1].bottom, true);
				dlg1->showImage(pic[0].frame);

				//�ڵ�һ��ͼƬ���Կ�����ʾ
				CDC* pDC = GetDlgItem(IDC_SMALL_PICTURE1)->GetDC();         
				HDC hDC = pDC->GetSafeHdc();								 
				CRect rect;
				GetDlgItem(IDC_SMALL_PICTURE1)->GetClientRect(&rect);		 

				IplImage iplImg = IplImage(pic[0].frame);
				CvvImage cvvImg;										
				cvvImg.CopyOf(&iplImg);
				cvvImg.DrawToHDC(hDC, &rect);
				cvvImg.Destroy();
				ReleaseDC(pDC);

				//˫��������ͼ1��Ŵ�����ͼ1
				if (expanse1 != NULL && expanse1->openExpanseDlg == true)
				{
					expanse1->showImage(pic[0].frame);
				}
				pic[0].currentFrame++;
			}
		}
	}
	//��Ļ���Ķ�ʱ��
	else if (2 == nIDEvent)
	{
		if (pic[1].capture.read(pic[1].frame))
		{
			if (pic[1].currentFrame <= pic[1].totalFrameNumber)
			{
				//����Ļ������ʾ
				dlg2->MoveWindow(rect[2].left, rect[2].top, rect[2].right, rect[2].bottom, true);
				dlg2->showImage(pic[1].frame);


				//�ڵڶ���ͼƬ���Կ�����ʾͼƬ
				CDC* pDC = GetDlgItem(IDC_SMALL_PICTURE2)->GetDC();            
				HDC hDC = pDC->GetSafeHdc();								
				CRect rect;
				GetDlgItem(IDC_SMALL_PICTURE2)->GetClientRect(&rect);	

				IplImage iplImg = IplImage(pic[1].frame);
				CvvImage cvvImg;											
				cvvImg.CopyOf(&iplImg);
				cvvImg.DrawToHDC(hDC, &rect);
				cvvImg.Destroy();
				ReleaseDC(pDC);

				//˫��������ͼ2��Ŵ�����ͼ2
				if (expanse2 != NULL && expanse2->openExpanseDlg == true)
				{
					expanse2->showImage(pic[1].frame);
				}
				pic[1].currentFrame++;
			}
		}
	}
	//��Ļ���Ķ�ʱ��
	else if (3 == nIDEvent)
	{
		if (pic[2].capture.read(pic[2].frame))	
		{
			if (pic[2].currentFrame <= pic[2].totalFrameNumber)
			{
				//����Ļ������ʾ
				dlg3->MoveWindow(rect[3].left, rect[3].top, rect[3].right, rect[3].bottom, true);
				dlg3->showImage(pic[2].frame);


				//�ڵ�����ͼƬ���Կ�����ʾͼƬ
				CDC* pDC = GetDlgItem(IDC_SMALL_PICTURE3)->GetDC();           
				HDC hDC = pDC->GetSafeHdc();								
				CRect rect;
				GetDlgItem(IDC_SMALL_PICTURE3)->GetClientRect(&rect);		

				IplImage iplImg = IplImage(pic[2].frame);
				CvvImage cvvImg;											 
				cvvImg.CopyOf(&iplImg);
				cvvImg.DrawToHDC(hDC, &rect);
				cvvImg.Destroy();
				ReleaseDC(pDC);

				//˫��������ͼ3��Ŵ�����ͼ3
				if (expanse3 != NULL && expanse3->openExpanseDlg == true)
				{
					expanse3->showImage(pic[2].frame);
				}
				pic[2].currentFrame++;
			}
		}
		else
		{

		}
	}
	//��Ļ�ĵĶ�ʱ��
	else if (4 == nIDEvent)
	{
		if (pic[3].capture.read(pic[3].frame))		
		{
			if (pic[3].currentFrame <= pic[3].totalFrameNumber)
			{
				//����Ļ������ʾ
				dlg4->MoveWindow(rect[4].left, rect[4].top, rect[4].right, rect[4].bottom, true);
				dlg4->showImage(pic[3].frame);

				//�ڵ��ĸ�ͼƬ���Կ�����ʾͼƬ
				CDC* pDC = GetDlgItem(IDC_SMALL_PICTURE4)->GetDC();          
				HDC hDC = pDC->GetSafeHdc();							
				CRect rect;
				GetDlgItem(IDC_SMALL_PICTURE4)->GetClientRect(&rect);		 

				IplImage iplImg = IplImage(pic[3].frame);
				CvvImage cvvImg;										
				cvvImg.CopyOf(&iplImg);
				cvvImg.DrawToHDC(hDC, &rect);
				cvvImg.Destroy();
				ReleaseDC(pDC);

				//˫��������ͼ4��Ŵ�����ͼ4
				if (expanse4 != NULL && expanse4->openExpanseDlg == true)
				{
					expanse4->showImage(pic[3].frame);
				}
				pic[3].currentFrame++;
			}
		}
	}
	CDialogEx::OnTimer(nIDEvent);
}


//���������Ƶ1��ť
void CSplitScreenDlg::OnBnClickedImport1()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������

	CString fileFilter = _T("ý���ļ�(*.wmv,*.mp3,*.avi,,*.rm,*.rmvb,*.mkv,*.mp4)|*.wmv;*.mp3;*.avi;*.rm;*.rmvb;*.mkv;*.mp4|");
	CFileDialog filedlg(true, NULL, NULL, OFN_HIDEREADONLY | OFN_ALLOWMULTISELECT | OFN_NOCHANGEDIR, fileFilter);

	if (filedlg.DoModal() == IDOK)
	{
		if (num_without_PrimaryScreen >= 1 && pic[0].isOpen == false)
		{
			if (NULL == dlg1)
			{
				dlg1 = new CPictureDlg();
				dlg1->Create(IDD__PICTURE_SHOW, this);
			}

			CString cpath = filedlg.GetPathName();
			pic[0].videoPath = CW2A(cpath.GetString());					//��ȡ��Ļ1����Ƶ����·��
			pic[0].isOpen = true;										//��Ļ1����
			pic[0].video_play = true;									//��Ļ1���ڲ���

																		//��ȡ�������ϵ�1������ͼ����ؾ����ָ��
			CDC* pDC = GetDlgItem(IDC_SMALL_PICTURE1)->GetDC();         //�������ͼ1�Ͽؼ��Ĵ���ָ�룬�ٻ�ȡ��ô��ڹ�����������ָ�� 
			CRect rect;
			GetDlgItem(IDC_SMALL_PICTURE1)->GetClientRect(&rect);		//��ȡ����ͼ1�Ͽؼ�����ʾ�� 
			pt[0].pDC = pDC;
			pt[0].rect = rect;

			//�����߳�1��������Ƶ1
			close_thread1 = false;
			pic_pthread1 = AfxBeginThread(ThreadProc1, &pt[0]);			

			//ʹ�ܵ��밴ť
			CButton *pBtn = (CButton *)GetDlgItem(IDC_IMPORT1);
			if (pBtn != NULL)
			{
				pBtn->EnableWindow(FALSE);
			}

			//ʹ����ͣ��ť
			CButton *pBtn1 = (CButton *)GetDlgItem(IDC_STOP2);
			if (pBtn1 != NULL)
			{
				pBtn1->EnableWindow(TRUE);
			}
		}
		else
		{
			MessageBox(L"δ��⵽��ʾ��1", L"����");
		}
	}
}


//���������Ƶ2��ť
void CSplitScreenDlg::OnBnClickedImport2()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������

	CString fileFilter = _T("ý���ļ�(*.wmv,*.mp3,*.avi,,*.rm,*.rmvb,*.mkv,*.mp4)|*.wmv;*.mp3;*.avi;*.rm;*.rmvb;*.mkv;*.mp4|");
	CFileDialog filedlg(true, NULL, NULL, OFN_HIDEREADONLY | OFN_ALLOWMULTISELECT | OFN_NOCHANGEDIR, fileFilter);

	if (filedlg.DoModal() == IDOK)
	{
		if (num_without_PrimaryScreen >= 2 && pic[1].isOpen == false)
		{
			if (NULL == dlg2)
			{
				dlg2 = new CPictureDlg();
				dlg2->Create(IDD__PICTURE_SHOW, this);
			}

			CString cpath = filedlg.GetPathName();
			pic[1].videoPath = CW2A(cpath.GetString());					//��ȡ��Ļ2����Ƶ����·��
			pic[1].isOpen = true;										//��Ļ2����
			pic[1].video_play = true;									//��Ļ2���ڲ���

																		//��ȡ�������ϵ�2������ͼ����ؾ����ָ��
			CDC* pDC = GetDlgItem(IDC_SMALL_PICTURE2)->GetDC();         //�������ͼ2�Ͽؼ��Ĵ���ָ�룬�ٻ�ȡ��ô��ڹ�����������ָ�� 
			CRect rect;
			GetDlgItem(IDC_SMALL_PICTURE2)->GetClientRect(&rect);		//��ȡ����ͼ2�Ͽؼ�����ʾ�� 
			pt[1].pDC = pDC;
			pt[1].rect = rect;

			//�����߳�2��������Ƶ2
			close_thread2 = false;
			pic_pthread2 = AfxBeginThread(ThreadProc2, &pt[1]);			

			//ʹ�ܵ��밴ť
			CButton *pBtn = (CButton *)GetDlgItem(IDC_IMPORT2);
			if (pBtn != NULL)
			{
				pBtn->EnableWindow(FALSE);
			}

			//ʹ����ͣ��ť
			CButton *pBtn2 = (CButton *)GetDlgItem(IDC_STOP1);
			if (pBtn2 != NULL)
			{
				pBtn2->EnableWindow(TRUE);
			}
		}
		else
		{
			MessageBox(L"δ��⵽��ʾ��2", L"����");
		}	
	}
}


//���������Ƶ3��ť
void CSplitScreenDlg::OnBnClickedImport3()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������

	CString fileFilter = _T("ý���ļ�(*.wmv,*.mp3,*.avi,,*.rm,*.rmvb,*.mkv,*.mp4)|*.wmv;*.mp3;*.avi;*.rm;*.rmvb;*.mkv;*.mp4|");
	CFileDialog filedlg(true, NULL, NULL, OFN_HIDEREADONLY | OFN_ALLOWMULTISELECT | OFN_NOCHANGEDIR, fileFilter);

	if (filedlg.DoModal() == IDOK)
	{
		if (num_without_PrimaryScreen >= 3 && pic[2].isOpen == false)
		{
			if (NULL == dlg3)
			{
				dlg3 = new CPictureDlg();
				dlg3->Create(IDD__PICTURE_SHOW, this);
			}

			CString cpath = filedlg.GetPathName();
			pic[2].videoPath = CW2A(cpath.GetString());					//��ȡ��Ļ3����Ƶ����·��
			pic[2].isOpen = true;										//��Ļ3����
			pic[2].video_play = true;									//��Ļ3���ڲ���

																		//��ȡ�������ϵ�3������ͼ����ؾ����ָ��
			CDC* pDC = GetDlgItem(IDC_SMALL_PICTURE3)->GetDC();         //�������ͼ3�Ͽؼ��Ĵ���ָ�룬�ٻ�ȡ��ô��ڹ�����������ָ�� 
			CRect rect;
			GetDlgItem(IDC_SMALL_PICTURE3)->GetClientRect(&rect);		//��ȡ����ͼ3�Ͽؼ�����ʾ�� 
			pt[2].pDC = pDC;
			pt[2].rect = rect;

			//�����߳�3��������Ƶ3
			close_thread3 = false;
			pic_pthread3 = AfxBeginThread(ThreadProc3, &pt[2]);		

			//ʹ�ܵ��밴ť
			CButton *pBtn = (CButton *)GetDlgItem(IDC_IMPORT3);
			if (pBtn != NULL)
			{
				pBtn->EnableWindow(FALSE);
			}

			//ʹ����ͣ��ť
			CButton *pBtn3 = (CButton *)GetDlgItem(IDC_STOP3);
			if (pBtn3 != NULL)
			{
				pBtn3->EnableWindow(TRUE);
			}
		}
		else
		{
			MessageBox(L"δ��⵽��ʾ��3", L"����");
		}
	}
}


//���������Ƶ4��ť
void CSplitScreenDlg::OnBnClickedImport4()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������

	CString fileFilter = _T("ý���ļ�(*.wmv,*.mp3,*.avi,,*.rm,*.rmvb,*.mkv,*.mp4)|*.wmv;*.mp3;*.avi;*.rm;*.rmvb;*.mkv;*.mp4|");
	CFileDialog filedlg(true, NULL, NULL, OFN_HIDEREADONLY | OFN_ALLOWMULTISELECT | OFN_NOCHANGEDIR, fileFilter);

	if (filedlg.DoModal() == IDOK)
	{
		if (num_without_PrimaryScreen >= 4 && pic[3].isOpen == false)
		{
			if (NULL == dlg4)
			{
				dlg4 = new CPictureDlg();
				dlg4->Create(IDD__PICTURE_SHOW, this);
			}

			CString cpath = filedlg.GetPathName();
			pic[3].videoPath = CW2A(cpath.GetString());					//��ȡ��Ļ4����Ƶ����·��
			pic[3].isOpen = true;										//��Ļ4����
			pic[3].video_play = true;									//��Ļ4���ڲ���

																		//��ȡ�������ϵ�4������ͼ����ؾ����ָ��
			CDC* pDC = GetDlgItem(IDC_SMALL_PICTURE4)->GetDC();         //�������ͼ4�Ͽؼ��Ĵ���ָ�룬�ٻ�ȡ��ô��ڹ�����������ָ�� 
			CRect rect;
			GetDlgItem(IDC_SMALL_PICTURE4)->GetClientRect(&rect);		//��ȡ����ͼ4�Ͽؼ�����ʾ�� 
			pt[3].pDC = pDC;
			pt[3].rect = rect;

			//�����߳�4��������Ƶ4
			close_thread4 = false;
			pic_pthread4 = AfxBeginThread(ThreadProc4, &pt[3]);			

			//ʹ�ܵ��밴ť
			CButton *pBtn = (CButton *)GetDlgItem(IDC_IMPORT4);
			if (pBtn != NULL)
			{
				pBtn->EnableWindow(FALSE);
			}

			//ʹ����ͣ��ť
			CButton *pBtn4 = (CButton *)GetDlgItem(IDC_STOP4);
			if (pBtn4 != NULL)
			{
				pBtn4->EnableWindow(TRUE);
			}
		}
		else
		{
			MessageBox(L"δ��⵽��ʾ��4", L"����");
		}
	}
}


//�ر���Ļ1
void CSplitScreenDlg::OnBnClickedClose1()
{
	if (dlg1 != NULL)
	{
		//�ر��߳�1
		close_thread1 = true;

		//˯��ʹ�߳�1��ֹر�
		Sleep(100);

		dlg1->CloseWindow();
		delete dlg1;
		dlg1 = NULL;

		//�ͷ���Ƶ��Դ
		pic[0].capture.release();
		pic[0].isOpen = false;
		pic[0].video_play = false;

		CDC* pDC = GetDlgItem(IDC_SMALL_PICTURE1)->GetDC();           //���IDC_SMALL_PICTURE1�ؼ��Ĵ���ָ�룬�ٻ�ȡ��ô��ڹ�����������ָ��  
		HDC hDC = pDC->GetSafeHdc();								  // ��ȡ�豸�����ľ��  
		CRect rect;
		GetDlgItem(IDC_SMALL_PICTURE1)->GetClientRect(&rect);		  //��ȡIDC_SMALL_PICTURE1�ؼ�����ʾ��   


		//�������ͼ1�ϵ�����
		DWORD dw = GetSysColor(COLOR_BTNFACE);
		CBrush br(dw);
		pDC->FillRect(rect, &br);

		CStatic *pStatic = (CStatic *)GetDlgItem(IDC_SMALL_PICTURE1);
		pStatic->ModifyStyleEx(WS_EX_STATICEDGE | WS_EX_DLGMODALFRAME, WS_EX_CLIENTEDGE, SWP_DRAWFRAME);

		ReleaseDC(pDC);

		CButton *pBtn = (CButton *)GetDlgItem(IDC_IMPORT1);
		if (pBtn != NULL)
		{
			pBtn->EnableWindow(TRUE);
		}

		GetDlgItem(IDC_STOP2)->SetWindowText(L"��ͣ");

		//ʹ�ܰ�ť
		CButton *pBtn1 = (CButton *)GetDlgItem(IDC_STOP2);
		if (pBtn1 != NULL)
		{
			pBtn1->EnableWindow(FALSE);
		}
	}
}


//�ر���Ļ2
void CSplitScreenDlg::OnBnClickedClose2()
{
	if (dlg2 != NULL)
	{
		//�ر��߳�2
		close_thread2 = true;

		//˯��ʹ�߳�2��ֹر�
		Sleep(100);

		dlg2->CloseWindow();
		delete dlg2;
		dlg2 = NULL;

		//�ͷ���Ƶ��Դ
		pic[1].capture.release();
		pic[1].isOpen = false;
		pic[1].video_play = false;

		CDC* pDC = GetDlgItem(IDC_SMALL_PICTURE2)->GetDC();           //���IDC_SMALL_PICTURE1�ؼ��Ĵ���ָ�룬�ٻ�ȡ��ô��ڹ�����������ָ��  
		HDC hDC = pDC->GetSafeHdc();								 // ��ȡ�豸�����ľ��  
		CRect rect;
		GetDlgItem(IDC_SMALL_PICTURE2)->GetClientRect(&rect);		//��ȡIDC_SMALL_PICTURE2�ؼ�����ʾ��   

		
		//�������ͼ4�ϵ�����
		DWORD dw = GetSysColor(COLOR_BTNFACE);
		CBrush br(dw);
		pDC->FillRect(rect, &br);

		CStatic *pStatic = (CStatic *)GetDlgItem(IDC_SMALL_PICTURE2);
		pStatic->ModifyStyleEx(WS_EX_STATICEDGE | WS_EX_DLGMODALFRAME, WS_EX_CLIENTEDGE, SWP_DRAWFRAME);

		ReleaseDC(pDC);

		CButton *pBtn = (CButton *)GetDlgItem(IDC_IMPORT2);
		if (pBtn != NULL)
		{
			pBtn->EnableWindow(TRUE);
		}

		GetDlgItem(IDC_STOP1)->SetWindowText(L"��ͣ");

		//ʹ�ܰ�ť
		CButton *pBtn2 = (CButton *)GetDlgItem(IDC_STOP1);
		if (pBtn2 != NULL)
		{
			pBtn2->EnableWindow(FALSE);
		}
	}
}


//�ر���Ļ3
void CSplitScreenDlg::OnBnClickedClose3()
{
	if (dlg3 != NULL)
	{
		//�ر��߳�3
		close_thread3 = true;

		//˯��ʹ�߳�3��ֹر�
		Sleep(100);

		dlg3->CloseWindow();
		delete dlg3;
		dlg3 = NULL;

		//�ͷ���Ƶ��Դ
		pic[2].capture.release();
		pic[2].isOpen = false;
		pic[2].video_play = false;

		CDC* pDC = GetDlgItem(IDC_SMALL_PICTURE3)->GetDC();           //���IDC_SMALL_PICTURE1�ؼ��Ĵ���ָ�룬�ٻ�ȡ��ô��ڹ�����������ָ��  
		HDC hDC = pDC->GetSafeHdc();								 // ��ȡ�豸�����ľ��  
		CRect rect;
		GetDlgItem(IDC_SMALL_PICTURE3)->GetClientRect(&rect);		//��ȡIDC_SMALL_PICTURE3�ؼ�����ʾ��   


		//�������ͼ3�ϵ�����
		DWORD dw = GetSysColor(COLOR_BTNFACE);
		CBrush br(dw);
		pDC->FillRect(rect, &br);

		CStatic *pStatic = (CStatic *)GetDlgItem(IDC_SMALL_PICTURE3);
		pStatic->ModifyStyleEx(WS_EX_STATICEDGE | WS_EX_DLGMODALFRAME, WS_EX_CLIENTEDGE, SWP_DRAWFRAME);

		ReleaseDC(pDC);

		CButton *pBtn = (CButton *)GetDlgItem(IDC_IMPORT3);
		if (pBtn != NULL)
		{
			pBtn->EnableWindow(TRUE);
		}

		GetDlgItem(IDC_STOP3)->SetWindowText(L"��ͣ");

		//ʹ�ܰ�ť
		CButton *pBtn3 = (CButton *)GetDlgItem(IDC_STOP3);
		if (pBtn3 != NULL)
		{
			pBtn3->EnableWindow(FALSE);
		}
	}
}


//�ر���Ļ4
void CSplitScreenDlg::OnBnClickedClose4()
{
	if (dlg4 != NULL)
	{
		//�ر��߳�4
		close_thread4 = true;

		//˯��ʹ�߳�4��ֹر�
		Sleep(100);

		dlg4->CloseWindow();
		delete dlg4;
		dlg4 = NULL;

		//�ͷ���Ƶ��Դ
		pic[3].capture.release();
		pic[3].isOpen = false;
		pic[3].video_play = false;

		CDC* pDC = GetDlgItem(IDC_SMALL_PICTURE4)->GetDC();           //���IDC_SMALL_PICTURE1�ؼ��Ĵ���ָ�룬�ٻ�ȡ��ô��ڹ�����������ָ��  
		HDC hDC = pDC->GetSafeHdc();								 // ��ȡ�豸�����ľ��  
		CRect rect;
		GetDlgItem(IDC_SMALL_PICTURE4)->GetClientRect(&rect);		//��ȡIDC_SMALL_PICTURE1�ؼ�����ʾ��   


		//�������ͼ4�ϵ�����
		DWORD dw = GetSysColor(COLOR_BTNFACE);
		CBrush br(dw);
		pDC->FillRect(rect, &br);

		CStatic *pStatic = (CStatic *)GetDlgItem(IDC_SMALL_PICTURE4);
		pStatic->ModifyStyleEx(WS_EX_STATICEDGE | WS_EX_DLGMODALFRAME, WS_EX_CLIENTEDGE, SWP_DRAWFRAME);
	
		ReleaseDC(pDC);

		CButton *pBtn = (CButton *)GetDlgItem(IDC_IMPORT4);
		if (pBtn != NULL)
		{
			pBtn->EnableWindow(TRUE);
		}

		GetDlgItem(IDC_STOP4)->SetWindowText(L"��ͣ");

		//ʹ�ܰ�ť
		CButton *pBtn4 = (CButton *)GetDlgItem(IDC_STOP4);
		if (pBtn4 != NULL)
		{
			pBtn4->EnableWindow(FALSE);
		}
	}
}


//�����ͣ��ť1
void CSplitScreenDlg::OnBnClickedStop2()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������

	//��������ͼһ��ʱ�������Ƶ���ڲ��ţ�����ͣ����֮�򲥷�
	if (pic[0].isOpen == true && pic[0].video_play == true)
	{
		//������ͣ
		pic_pthread1->SuspendThread();
		pic[0].video_play = false;

		GetDlgItem(IDC_STOP2)->SetWindowText(L"����");
	}
	else if (pic[0].isOpen == true && pic[0].video_play == false)
	{
		//������������
		pic_pthread1->ResumeThread();
		pic[0].video_play = true;

		GetDlgItem(IDC_STOP2)->SetWindowText(L"��ͣ");
	}
}


//�����ͣ��ť2
void CSplitScreenDlg::OnBnClickedStop1()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������

	//��������ͼһ��ʱ�������Ƶ���ڲ��ţ�����ͣ����֮�򲥷�
	if (pic[1].isOpen == true && pic[1].video_play == true)
	{
		//������ͣ
		pic_pthread2->SuspendThread();
		pic[1].video_play = false;

		GetDlgItem(IDC_STOP1)->SetWindowText(L"����");
	}
	else if (pic[1].isOpen == true && pic[1].video_play == false)
	{
		//������������
		pic_pthread2->ResumeThread();
		pic[1].video_play = true;

		GetDlgItem(IDC_STOP1)->SetWindowText(L"��ͣ");
	}
}


//�����ͣ��ť3
void CSplitScreenDlg::OnBnClickedStop3()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������

	//��������ͼһ��ʱ�������Ƶ���ڲ��ţ�����ͣ����֮�򲥷�
	if (pic[2].isOpen == true && pic[2].video_play == true)
	{
		//������ͣ
		pic_pthread3->SuspendThread();
		pic[2].video_play = false;

		GetDlgItem(IDC_STOP3)->SetWindowText(L"����");
	}
	else if (pic[2].isOpen == true && pic[2].video_play == false)
	{
		//������������
		pic_pthread3->ResumeThread();
		pic[2].video_play = true;

		GetDlgItem(IDC_STOP3)->SetWindowText(L"��ͣ");
	}
}


//�����ͣ��ť4
void CSplitScreenDlg::OnBnClickedStop4()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������

	//��������ͼһ��ʱ�������Ƶ���ڲ��ţ�����ͣ����֮�򲥷�
	if (pic[3].isOpen == true && pic[3].video_play == true)
	{
		//������ͣ
		pic_pthread4->SuspendThread();
		pic[3].video_play = false;

		GetDlgItem(IDC_STOP4)->SetWindowText(L"����");
	}
	else if (pic[3].isOpen == true && pic[3].video_play == false)
	{
		//������������
		pic_pthread4->ResumeThread();
		pic[3].video_play = true;

		GetDlgItem(IDC_STOP4)->SetWindowText(L"��ͣ");
	}
}


//����������˵���ť
void CSplitScreenDlg::OnBnClickedCancel()
{
}


//������Ͻǹرհ�ť
void CSplitScreenDlg::OnClose()
{
	close_thread1 = true;
	close_thread2 = true;
	close_thread3 = true;
	close_thread4 = true;

	//�ȴ�200ms��Źر���Դ��Ŀ���������߳��г����ʱ��ȥ�˳����ͷ���Դ
	Sleep(200);			

	if (dlg1 != NULL)
	{
		delete dlg1;
		dlg1 = NULL;
	}
	if (dlg2 != NULL)
	{
		delete dlg2;
		dlg2 = NULL;
	}
	if (dlg3 != NULL)
	{
		delete dlg3;
		dlg3 = NULL;
	}
	if (dlg4 != NULL)
	{
		delete dlg4;
		dlg4 = NULL;
	}
	if (pt != NULL)
	{
		delete[] pt;
		pt = NULL;
	}
	//�رյ�ǰ����
	CDialogEx::OnCancel();

	//�ر������洰��
	AfxGetMainWnd()->SendMessage(WM_CLOSE);		
}


//����������˵�����
void CSplitScreenDlg::OnNMClickSyslink1(NMHDR *pNMHDR, LRESULT *pResult)
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	//*pResult = 0;
	
	if (dlg1 != NULL)
	{
		//�ر��߳�1
		close_thread1 = true;
		//˯��ʹ�߳�1��ֹر�
		Sleep(100);

		dlg1->CloseWindow();
		delete dlg1;
		dlg1 = NULL;

		pic[0].capture.release();
		pic[0].isOpen = false;
		pic[0].video_play = false;

		CDC* pDC = GetDlgItem(IDC_SMALL_PICTURE1)->GetDC();
		HDC hDC = pDC->GetSafeHdc();
		CRect rect;
		GetDlgItem(IDC_SMALL_PICTURE1)->GetClientRect(&rect);

		DWORD dw = GetSysColor(COLOR_BTNFACE);
		CBrush br(dw);
		pDC->FillRect(rect, &br);

		CStatic *pStatic = (CStatic *)GetDlgItem(IDC_SMALL_PICTURE1);
		pStatic->ModifyStyleEx(WS_EX_STATICEDGE | WS_EX_DLGMODALFRAME, WS_EX_CLIENTEDGE, SWP_DRAWFRAME);

		ReleaseDC(pDC);

		CButton *pBtn = (CButton *)GetDlgItem(IDC_IMPORT1);
		if (pBtn != NULL)
		{
			pBtn->EnableWindow(TRUE);
		}

		GetDlgItem(IDC_STOP2)->SetWindowText(L"��ͣ");
	}

	if (dlg2 != NULL)
	{
		//�ر��߳�2
		close_thread2 = true;
		//˯��ʹ�߳�2��ֹر�
		Sleep(100);

		dlg2->CloseWindow();
		delete dlg2;
		dlg2 = NULL;

		pic[1].capture.release();
		pic[1].isOpen = false;
		pic[1].video_play = false;

		CDC* pDC = GetDlgItem(IDC_SMALL_PICTURE2)->GetDC();
		HDC hDC = pDC->GetSafeHdc();
		CRect rect;
		GetDlgItem(IDC_SMALL_PICTURE2)->GetClientRect(&rect);

		DWORD dw = GetSysColor(COLOR_BTNFACE);
		CBrush br(dw);
		pDC->FillRect(rect, &br);

		CStatic *pStatic = (CStatic *)GetDlgItem(IDC_SMALL_PICTURE2);
		pStatic->ModifyStyleEx(WS_EX_STATICEDGE | WS_EX_DLGMODALFRAME, WS_EX_CLIENTEDGE, SWP_DRAWFRAME);

		ReleaseDC(pDC);

		CButton *pBtn = (CButton *)GetDlgItem(IDC_IMPORT2);
		if (pBtn != NULL)
		{
			pBtn->EnableWindow(TRUE);
		}

		GetDlgItem(IDC_STOP1)->SetWindowText(L"��ͣ");
	}

	if (dlg3 != NULL)
	{
		//�ر��߳�3
		close_thread3 = true;
		//˯��ʹ�߳�3��ֹر�
		Sleep(100);

		dlg3->CloseWindow();
		delete dlg3;
		dlg3 = NULL;

		pic[2].capture.release();
		pic[2].isOpen = false;
		pic[2].video_play = false;

		CDC* pDC = GetDlgItem(IDC_SMALL_PICTURE3)->GetDC();
		HDC hDC = pDC->GetSafeHdc();
		CRect rect;
		GetDlgItem(IDC_SMALL_PICTURE3)->GetClientRect(&rect);

		DWORD dw = GetSysColor(COLOR_BTNFACE);
		CBrush br(dw);
		pDC->FillRect(rect, &br);

		CStatic *pStatic = (CStatic *)GetDlgItem(IDC_SMALL_PICTURE3);
		pStatic->ModifyStyleEx(WS_EX_STATICEDGE | WS_EX_DLGMODALFRAME, WS_EX_CLIENTEDGE, SWP_DRAWFRAME);

		ReleaseDC(pDC);

		CButton *pBtn = (CButton *)GetDlgItem(IDC_IMPORT3);
		if (pBtn != NULL)
		{
			pBtn->EnableWindow(TRUE);
		}

		GetDlgItem(IDC_STOP3)->SetWindowText(L"��ͣ");
	}


	if (dlg4 != NULL)
	{
		//�ر��߳�4
		close_thread4 = true;
		//˯��ʹ�߳�4��ֹر�
		Sleep(100);

		dlg4->CloseWindow();
		delete dlg4;
		dlg4 = NULL;

		pic[3].capture.release();
		pic[3].isOpen = false;
		pic[3].video_play = false;

		CDC* pDC = GetDlgItem(IDC_SMALL_PICTURE4)->GetDC();
		HDC hDC = pDC->GetSafeHdc();
		CRect rect;
		GetDlgItem(IDC_SMALL_PICTURE4)->GetClientRect(&rect);

		DWORD dw = GetSysColor(COLOR_BTNFACE);
		CBrush br(dw);
		pDC->FillRect(rect, &br);

		CStatic *pStatic = (CStatic *)GetDlgItem(IDC_SMALL_PICTURE4);
		pStatic->ModifyStyleEx(WS_EX_STATICEDGE | WS_EX_DLGMODALFRAME, WS_EX_CLIENTEDGE, SWP_DRAWFRAME);

		ReleaseDC(pDC);

		CButton *pBtn = (CButton *)GetDlgItem(IDC_IMPORT4);
		if (pBtn != NULL)
		{
			pBtn->EnableWindow(TRUE);
		}

		GetDlgItem(IDC_STOP4)->SetWindowText(L"��ͣ");
	}

	//�رյ�ǰ����
	CDialogEx::OnCancel();
}