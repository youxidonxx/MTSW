#pragma once


// CCommonSetDlg �Ի���

class CCommonSetDlg : public CDialog
{
	DECLARE_DYNAMIC(CCommonSetDlg)

public:
	CCommonSetDlg(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~CCommonSetDlg();

// �Ի�������
	enum { IDD = IDD_DLG_COMMONSET };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

	DECLARE_MESSAGE_MAP()
};
