// MainDlg.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "SplitScreen.h"
#include "SplitScreenDlg.h"
#include "MainDlg.h"
#include "SYNDlg.h"
#include "HelpDlg.h"
#include "About.h"
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
	ON_NOTIFY(NM_CLICK, IDC_HELP, &CMainDlg::OnNMClickHelp)
	ON_NOTIFY(NM_CLICK, IDC_ABOUT, &CMainDlg::OnNMClickAbout)
END_MESSAGE_MAP()


// CMainDlg ��Ϣ�������

//һ��ƴ�ӷ�������
void CMainDlg::OnBnClickedSplitscreen()
{

	//���ص�ǰ��¼������
	this->ShowWindow(SW_HIDE);	

	CSYNDlg dlg;
	dlg.DoModal();

	//��ʾƴ�Ӳ��ŵĿ��ƽ���
	this->ShowWindow(SW_SHOW);
}

//�������Ž���
void CMainDlg::OnBnClickedAlone()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	//���ص�ǰ��¼������
	this->ShowWindow(SW_HIDE);	

	CSplitScreenDlg dlg;
	dlg.DoModal();

	//��ʾ�������ŵĿ��ƽ���
	this->ShowWindow(SW_SHOW);
}


//���������ť
void CMainDlg::OnNMClickHelp(NMHDR *pNMHDR, LRESULT *pResult)
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	*pResult = 0;

	//���ص�ǰ��¼������
	this->ShowWindow(SW_HIDE);

	CHelpDlg dlg;
	dlg.DoModal();

	//��ʾ�������ŵĿ��ƽ���
	this->ShowWindow(SW_SHOW);

}


//������ڰ�ť
void CMainDlg::OnNMClickAbout(NMHDR *pNMHDR, LRESULT *pResult)
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	*pResult = 0;

	CAbout dlg;
	dlg.DoModal();

	//��ʾ�������ŵĿ��ƽ���
	this->ShowWindow(SW_SHOW);
}
