#pragma once
#include "afxwin.h"


// CComSetDlg �Ի���

class CComSetDlg : public CDialog
{
	DECLARE_DYNAMIC(CComSetDlg)

public:
	CComSetDlg(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~CComSetDlg();

// �Ի�������
	enum { IDD = IDD_DLG_COMSET };

	int		m_nPortNum;
protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��
	void	ScanPort();
	INT_PTR	GetSerialPort(CStringArray &arrCom); //���������������Щ����

	DECLARE_MESSAGE_MAP()
public:
	CComboBox m_cbComSet;
	virtual BOOL OnInitDialog();
public:
	afx_msg void OnCbnDropdownComboComset();
public:
	afx_msg void OnCbnSelchangeComboComset();
public:
	afx_msg void OnBnClickedOk();
public:
	afx_msg void OnBnClickedCancel();
};
