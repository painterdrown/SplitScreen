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


// CPictureDlg ��Ϣ�������
void CPictureDlg::showImage(Mat& image)
{
	CDC* pDC = GetDlgItem(IDC_PICTURE_SHOW)->GetDC();           //����ID��ô���ָ���ٻ�ȡ��ô��ڹ�����������ָ��  
	HDC hDC = pDC->GetSafeHdc();								// ��ȡ�豸�����ľ��  
	CRect winRect;
	GetDlgItem(IDC_PICTURE_SHOW)->GetClientRect(&winRect);         //��ȡ��ʾ��    

	IplImage iplImg;
	//resize���frame
	Mat newImg;
	int newWidth, newHeight;
	bool is_resize = false;
	if (winRect.Width() < image.cols)		//���ͼƬ�ķֱ��ʱ���Ļ�ķֱ��ʴ���resize
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
		resize(image, newImg, Size(winRect.Width(), winRect.Height()), 0, 0, CV_INTER_LINEAR);
		iplImg = IplImage(image);
	}
	else
	{
		iplImg = IplImage(image);
	}
	CvvImage cvvImg;											//����һ��CvvImage����  
	cvvImg.CopyOf(&iplImg);
	cvvImg.DrawToHDC(hDC, &winRect);
	cvvImg.Destroy();

	ShowWindow(SW_SHOW);
	ReleaseDC(pDC);
}

void CPictureDlg::ShowSmallPic(CDC* pDC, Mat frame, CRect rect)
{
	HDC hDC = pDC->GetSafeHdc();							 // ��ȡ�豸�����ľ��  	
															 //resize���frame
	Mat newImg;
	//��ԭͼ��resizeΪҪ��ʾ�Ŀͻ��������������Կ�Ĵ�С���ķֱ��ʴ�С������ʹ�õ����ڲ�ֵ�ķ���
	resize(frame, newImg, Size(rect.Width(), rect.Height()), 0, 0, CV_INTER_LINEAR);

	IplImage iplImg = IplImage(newImg);
	CvvImage cvvImg;										//����һ��CvvImage����  
	cvvImg.CopyOf(&iplImg);
	cvvImg.DrawToHDC(hDC, &rect);
	cvvImg.Destroy();
}

void CPictureDlg::ChangeSize(CWnd *pWnd, int cx, int cy)
{
	if (pWnd)  //�ж��Ƿ�Ϊ�գ���Ϊ�Ի��򴴽�ʱ����ô˺���������ʱ�ؼ���δ����   
	{
		CRect rect;					  
		pWnd->GetWindowRect(&rect);		//��ȡ�����Ի�����仯ǰ�Ĵ�С�����浽rect��  
		ScreenToClient(&rect);			//���ؼ���Сת��Ϊ�ڶԻ����е���������  

		//cx/m_rect.Width()Ϊ�Ի����ں���ı仯����  
		rect.left = rect.left * cx / m_rect.Width();//�����ؼ���С  
		rect.right = rect.right * cx / m_rect.Width();
		rect.top = rect.top * cy / m_rect.Height();
		rect.bottom = rect.bottom * cy / m_rect.Height();
		pWnd->MoveWindow(rect);//���ÿؼ���С  
	}
}

void CPictureDlg::OnSize(UINT nType, int cx, int cy)
{
	if (nType == 1) return;//��С����ʲô������ 

	CWnd *pWnd;
	pWnd = GetDlgItem(IDC_PICTURE_SHOW);
	ChangeSize(pWnd, cx, cy);
	GetClientRect(&m_rect);// ���仯��ĶԻ����С��Ϊ�ɴ�С   

	CDialogEx::OnSize(nType, cx, cy);
}

void CPictureDlg::OnClose()
{
	openExpanseDlg = false;		//�޸ı���openExpanseDlg����������ȫ����ʾ�ر���

	CDialogEx::OnClose();
}
