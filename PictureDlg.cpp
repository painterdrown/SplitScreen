// PictureDlg.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "SplitScreen.h"
#include "PictureDlg.h"
#include "afxdialogex.h"

using namespace std;
using namespace cv;

IMPLEMENT_DYNAMIC(CPictureDlg, CDialogEx)

CPictureDlg::CPictureDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(IDD__PICTURE_SHOW, pParent)
{
	openExpanseDlg = false;
}

CPictureDlg::~CPictureDlg()
{
	
}

void CPictureDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(CPictureDlg, CDialogEx)
	ON_WM_SIZE()
	ON_WM_CLOSE()
END_MESSAGE_MAP()


BOOL CPictureDlg::OnInitDialog()
{
	//��ȡ��ʼ��ʱ�ĶԻ����С
	GetClientRect(&m_rect);  

	return TRUE;  // return TRUE  unless you set the focus to a control
}

// CPictureDlg ��Ϣ�������

//����Ƶ��ÿһ֡ͼƬ���
void CPictureDlg::showImage(Mat& image)
{
	//����ID��ô���ָ���ٻ�ȡ��ô��ڹ�����������ָ��  
	CDC* pDC = GetDlgItem(IDC_PICTURE_SHOW)->GetDC();    
	// ��ȡ�豸�����ľ��  
	HDC hDC = pDC->GetSafeHdc();		

	//��ȡ��ʾ��  
	CRect winRect;	  
	GetDlgItem(IDC_PICTURE_SHOW)->GetClientRect(&winRect);        

	IplImage iplImg;

	//resize���frame
	Mat newImg;
	int newWidth;
	int newHeight;
	bool is_resize = false;

	//���ͼƬ�ķֱ��ʱ���Ļ�ķֱ��ʴ���resize
	if (winRect.Width() < image.cols)		
	{
		is_resize = true;
		newWidth = image.cols;
	}
	if (winRect.Height() < image.rows)
	{
		is_resize = true;
		newHeight = image.rows;
	}
	if (is_resize == true)
	{
		//��ԭͼ��resizeΪҪ��ʾ�Ŀͻ����ķֱ��ʴ�С������ʹ�õ����ڲ�ֵ�ķ���
		resize(image, newImg, Size(winRect.Width(), winRect.Height()), 0, 0, INTER_CUBIC);
		iplImg = IplImage(image);
	}
	else
	{
		iplImg = IplImage(image);
	}

	//����һ��CvvImage����  
	CvvImage cvvImg;											
	cvvImg.CopyOf(&iplImg);
	cvvImg.DrawToHDC(hDC, &winRect);
	cvvImg.Destroy();

	//��ͼƬ����ͼƬ�ؼ�����ʾ��ͼƬ�Ի���
	ShowWindow(SW_SHOW);

	ReleaseDC(pDC);
}


//����Ƶ��ÿһ֡ͼƬ���
void CPictureDlg::ShowSmallPic(CDC* pDC, Mat frame, CRect rect)
{
	// ��ȡ�豸�����ľ��
	HDC hDC = pDC->GetSafeHdc();

	//resize���frame
	Mat newImg;
	int newWidth = rect.Width();
	int newHeight = rect.Height();

	//��ԭͼ��resizeΪҪ��ʾ�Ŀͻ��������������Կ�Ĵ�С���ķֱ��ʴ�С������ʹ�õ��ǲ�ֵ�ķ���
	resize(frame, newImg, Size(newWidth, newHeight), 0, 0, INTER_CUBIC);

	IplImage iplImg = IplImage(newImg);

	//����һ��CvvImage���� 
	CvvImage cvvImg;										 
	cvvImg.CopyOf(&iplImg);
	cvvImg.DrawToHDC(hDC, &rect);
	cvvImg.Destroy();
}

//�ڶԻ���Ĵ�С�ı�ʱ��̬�ı�ؼ���λ�úʹ�С
void CPictureDlg::ChangeSize(CWnd *pWnd, int cx, int cy)
{
	//�ж��Ƿ�Ϊ�գ���Ϊ�Ի��򴴽�ʱ����ô˺���������ʱ�ؼ���δ����
	if (pWnd)    
	{
		//��ȡ�����Ի�����仯ǰ�Ĵ�С�����浽rect��
		CRect rect;					  
		pWnd->GetWindowRect(&rect);	

		//���ؼ���Сת��Ϊ�ڶԻ����е���������  
		ScreenToClient(&rect);			

		// cx / m_rect.Width()Ϊ�Ի����ں���ı仯���� 
		//�����ؼ���С  
		rect.left = rect.left * cx / m_rect.Width();
		rect.right = rect.right * cx / m_rect.Width();
		rect.top = rect.top * cy / m_rect.Height();
		rect.bottom = rect.bottom * cy / m_rect.Height();

		//���ÿؼ���λ�úʹ�С  
		pWnd->MoveWindow(rect);
	}

}

//���Ի����������ͬ��Ļʱ��̬�޸ĶԻ���Ĵ�С
void CPictureDlg::OnSize(UINT nType, int cx, int cy)
{
	CDialogEx::OnSize(nType, cx, cy);

	//��С���Ի���ʱʲô������ 
	if (nType == SIZE_MINIMIZED) return;

	//����ID��ø�ͼƬ�ؼ��Ĵ���ָ��
	CWnd *pWnd;
	pWnd = GetDlgItem(IDC_PICTURE_SHOW);

	//���ú����޸ĸ�ͼƬ�ؼ���̬��Ի���ĸı���ı�λ�úʹ�С
	ChangeSize(pWnd, cx, cy);

	//���±仯��ĶԻ����С   
	GetClientRect(&m_rect);
}

//������Ͻǹر�ʱ
void CPictureDlg::OnClose()
{
	//�޸ı���openExpanseDlg����ʾ������ȫ����ʾ�ĶԻ���ر���
	openExpanseDlg = false;		

	CDialogEx::OnClose();
}
