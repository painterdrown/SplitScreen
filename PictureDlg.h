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
	CPictureDlg(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~CPictureDlg();

	BOOL OnInitDialog();

	//��ͼƬ�ؼ�����ʾͼƬimage
	void showImage(Mat& image);			
	//������ͼ�ؼ�����ʾͼƬframe
	void ShowSmallPic(CDC* pDC, Mat frame, CRect rect);

	CRect m_rect;						//��¼��ǰ�Ի���Ĵ�С
	void ChangeSize(CWnd *pWnd, int cx, int cy);


	bool openExpanseDlg;

// �Ի�������
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD__PICTURE_SHOW };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnClose();
};
