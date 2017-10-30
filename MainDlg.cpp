// MainDlg.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "SplitScreen.h"
#include "SplitScreenDlg.h"
#include "MainDlg.h"
#include "afxdialogex.h"


// CMainDlg �Ի���

IMPLEMENT_DYNAMIC(CMainDlg, CDialogEx)

CMainDlg::CMainDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(IDD_MAINDLG, pParent)
{

}

CMainDlg::~CMainDlg()
{
}

void CMainDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(CMainDlg, CDialogEx)
	ON_BN_CLICKED(IDC_SPLITSCREEN, &CMainDlg::OnBnClickedSplitscreen)
	ON_BN_CLICKED(IDC_ALONE, &CMainDlg::OnBnClickedAlone)
END_MESSAGE_MAP()


// CMainDlg ��Ϣ�������

//һ��ƴ�ӷ�������
void CMainDlg::OnBnClickedSplitscreen()
{
	this->ShowWindow(SW_HIDE);	//��ǰ����������
	CSplitScreenDlg dlg;		
	dlg.DoModal();
	this->ShowWindow(SW_SHOW);
}

//�������Ž���
void CMainDlg::OnBnClickedAlone()
{
	this->ShowWindow(SW_HIDE);	//��ǰ����������
	CSplitScreenDlg dlg;
	dlg.DoModal();
	this->ShowWindow(SW_SHOW);
}
