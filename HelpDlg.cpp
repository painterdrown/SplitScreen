// HelpDlg.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "SplitScreen.h"
#include "HelpDlg.h"
#include "MainDlg.h"
#include "afxdialogex.h"


// CHelpDlg �Ի���

IMPLEMENT_DYNAMIC(CHelpDlg, CDialogEx)

CHelpDlg::CHelpDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(IDD_HELPDLG, pParent)
{

}

CHelpDlg::~CHelpDlg()
{
}

void CHelpDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(CHelpDlg, CDialogEx)
	ON_BN_CLICKED(IDC_HELP_RETURN, &CHelpDlg::OnBnClickedHelpReturn)
END_MESSAGE_MAP()


// CHelpDlg ��Ϣ�������


void CHelpDlg::OnBnClickedHelpReturn()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������

	//����
	CDialogEx::OnCancel();
}
