// SplitScreenDlg.h : ͷ�ļ�

#pragma once
#include "PictureDlg.h"
#include "UiThread.h"

using namespace std;

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

	bool videoCapture(CString videoPath);	


	CUiThread* pThread1;
	CUiThread* pThread2;
	CUiThread* pThread3;
	CUiThread* pThread4;



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
	afx_msg void OnTimer(UINT_PTR nIDEvent);
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
	afx_msg void OnBnClickedSyn();
	afx_msg void OnClose();
};
