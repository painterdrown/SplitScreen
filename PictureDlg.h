#pragma once

#include "stdafx.h"

#include "CvvImage.h"

#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <iostream>

using namespace std;
using namespace cv;

class CPictureDlg : public CDialogEx
{
	DECLARE_DYNAMIC(CPictureDlg)

public:
	// ��׼���캯��
	CPictureDlg(CWnd* pParent = NULL);  

	virtual ~CPictureDlg();

	//��ʼ��ͼƬ�Ի���ĺ���
	BOOL OnInitDialog();

	//��ͼƬ�ؼ�����ʾͼƬimage
	void showImage(Mat& image);			

	//�ڿ��ƽ��������ͼ�ؼ�����ʾͼƬframe
	void ShowSmallPic(CDC* pDC, Mat frame, CRect rect);

	//��¼��ǰ�Ի���Ĵ�С
	CRect m_rect;						

	//���ڶ�̬�ı�ؼ��Ĵ�С
	void ChangeSize(CWnd *pWnd, int cx, int cy);

	//�����жϵ�ǰ�Ƿ�ȫ����ʾĳ����Ļ
	bool openExpanseDlg;


// �Ի�������

#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD__PICTURE_SHOW };
#endif


protected:
	// DDX/DDV ֧��
	virtual void DoDataExchange(CDataExchange* pDX);    

	DECLARE_MESSAGE_MAP()

public:
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnClose();
};
