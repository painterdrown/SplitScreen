#pragma once


// CMainDlg �Ի���

class CMainDlg : public CDialogEx
{
	DECLARE_DYNAMIC(CMainDlg)

public:
	CMainDlg(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~CMainDlg();

// �Ի�������
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_MAINDLG };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

	DECLARE_MESSAGE_MAP()

public:
	afx_msg void OnBnClickedSplitscreen();
	afx_msg void OnBnClickedAlone();
	afx_msg void OnNMClickHelp(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnNMClickAbout(NMHDR *pNMHDR, LRESULT *pResult);
};
