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
	CRect rect;
	GetDlgItem(IDC_PICTURE_SHOW)->GetClientRect(&rect);         //��ȡ��ʾ��    

	IplImage iplImg = IplImage(image);
	CvvImage cvvImg;											//����һ��CvvImage����  
	cvvImg.CopyOf(&iplImg);
	cvvImg.DrawToHDC(hDC, &rect);
	cvvImg.Destroy();

	ShowWindow(SW_SHOW);
	ReleaseDC(pDC);
}

void CPictureDlg::ChangeSize(CWnd *pWnd, int cx, int cy)
{
	if (pWnd)  //�ж��Ƿ�Ϊ�գ���Ϊ�Ի��򴴽�ʱ����ô˺���������ʱ�ؼ���δ����   
	{
		CRect rect;   //��ȡ�ؼ��仯ǰ�Ĵ�С    
		pWnd->GetWindowRect(&rect);
		ScreenToClient(&rect);//���ؼ���Сת��Ϊ�ڶԻ����е���������  

		//    cx/m_rect.Width()Ϊ�Ի����ں���ı仯����  
		rect.left = rect.left * cx / m_rect.Width();//�����ؼ���С  
		rect.right = rect.right * cx / m_rect.Width();
		rect.top = rect.top * cy / m_rect.Height();
		rect.bottom = rect.bottom * cy / m_rect.Height();
		pWnd->MoveWindow(rect);//���ÿؼ���С  
	}
}

void CPictureDlg::OnSize(UINT nType, int cx, int cy)
{
	CDialogEx::OnSize(nType, cx, cy);

	if (nType == 1) return;//��С����ʲô������ 

	CWnd *pWnd;
	pWnd = GetDlgItem(IDC_PICTURE_SHOW);
	ChangeSize(pWnd, cx, cy);
	GetClientRect(&m_rect);// ���仯��ĶԻ����С��Ϊ�ɴ�С   
}

void CPictureDlg::OnClose()
{
	//CSplitScreenApp *app = (CSplitScreenApp *)AfxGetApp(); //����ָ��Ӧ�ó������ָ�� 
	openExpanseDlg = false;		//�޸ı���openExpanseDlg
	CDialogEx::OnClose();
}
