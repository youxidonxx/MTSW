#pragma once


// CDigitSetDlg �Ի���

class CDigitSetDlg : public CDialog
{
	DECLARE_DYNAMIC(CDigitSetDlg)

public:
	CDigitSetDlg(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~CDigitSetDlg();

// �Ի�������
	enum { IDD = IDD_DLG_DIGITSET };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

	DECLARE_MESSAGE_MAP()
};
