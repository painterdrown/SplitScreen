// About.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "SplitScreen.h"
#include "About.h"
#include "MainDlg.h"
#include "afxdialogex.h"


// CAbout �Ի���

IMPLEMENT_DYNAMIC(CAbout, CDialogEx)

CAbout::CAbout(CWnd* pParent /*=NULL*/)
	: CDialogEx(IDD_ABOUTDLG, pParent)
{

}

CAbout::~CAbout()
{
}

void CAbout::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(CAbout, CDialogEx)
	ON_BN_CLICKED(IDC_QUEDING, &CAbout::OnBnClickedQueding)
END_MESSAGE_MAP()


// CAbout ��Ϣ�������


void CAbout::OnBnClickedQueding()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������

	//����
	CDialogEx::OnCancel();
}
