#pragma once


// CSYNDlg �Ի���

class CSYNDlg : public CDialogEx
{
	DECLARE_DYNAMIC(CSYNDlg)

public:
	// ��׼���캯��
	CSYNDlg(CWnd* pParent = NULL);  
	//��������
	virtual ~CSYNDlg();

	//�����жϵ�ǰһ��ƴ�ӽ����Ƿ����ڲ�����Ƶ
	bool is_play;

	BOOL OnInitDialog();

// �Ի�������
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_SYNDLG };
#endif

protected:
	// DDX/DDV ֧��
	virtual void DoDataExchange(CDataExchange* pDX);    

	DECLARE_MESSAGE_MAP()

public:
	afx_msg void OnBnClickedSynLode();
	afx_msg void OnTimer(UINT_PTR nIDEvent);
	afx_msg void OnClose();
	afx_msg void OnBnClickedSynPlay();
	afx_msg void OnBnClickedSynReturn();
};
