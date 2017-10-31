// SplitScreenDlg.h : ͷ�ļ�

#pragma once
#include "PictureDlg.h"
#include "UiThread.h"

using namespace std;

//�ĸ��̺߳���
UINT ThreadProc1(LPVOID pM);
UINT ThreadProc2(LPVOID pM);
UINT ThreadProc3(LPVOID pM);
UINT ThreadProc4(LPVOID pM);

// CSplitScreenDlg �Ի���
class CSplitScreenDlg : public CDialogEx
{
// ����
public:
	CSplitScreenDlg(CWnd* pParent = NULL);	// ��׼���캯��
	~CSplitScreenDlg();						//��������
// �Ի�������
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_SPLITSCREEN_DIALOG };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV ֧��

public:
	//�ĸ��������߳�
	CWinThread* pic_pthread1;
	CWinThread* pic_pthread2;
	CWinThread* pic_pthread3;
	CWinThread* pic_pthread4;

// ʵ��
protected:
	HICON m_hIcon;

	// ���ɵ���Ϣӳ�亯��
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnStnDblclickSmallPicture1();
	afx_msg void OnStnDblclickSmallPicture2();
	afx_msg void OnStnDblclickSmallPicture3();
	afx_msg void OnStnDblclickSmallPicture4();
	afx_msg void OnStnClickedSmallPicture1();
	afx_msg void OnStnClickedSmallPicture2();
	afx_msg void OnStnClickedSmallPicture3();
	afx_msg void OnStnClickedSmallPicture4();
	afx_msg void OnBnClickedCancel();
	afx_msg void OnBnClickedClose1();
	afx_msg void OnBnClickedClose2();
	afx_msg void OnBnClickedClose3();
	afx_msg void OnBnClickedClose4();
	afx_msg void OnClose();
	afx_msg void OnNMClickSyslink1(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnBnClickedImport1();
	afx_msg void OnBnClickedImport2();
	afx_msg void OnBnClickedImport3();
	afx_msg void OnBnClickedImport4();
	afx_msg void OnBnClickedStop2();
	afx_msg void OnBnClickedStop1();
	afx_msg void OnBnClickedStop3();
	afx_msg void OnBnClickedStop4();
};
