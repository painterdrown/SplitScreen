#pragma once


// CSYNDlg �Ի���

class CSYNDlg : public CDialogEx
{
	DECLARE_DYNAMIC(CSYNDlg)

public:
	CSYNDlg(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~CSYNDlg();

	bool is_play;

// �Ի�������
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_SYNDLG };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedSynLode();
	afx_msg void OnTimer(UINT_PTR nIDEvent);
	afx_msg void OnClose();
	afx_msg void OnBnClickedSynPlay();
	afx_msg void OnBnClickedSynReturn();
};
