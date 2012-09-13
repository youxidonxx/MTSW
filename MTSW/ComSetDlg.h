#pragma once
#include "afxwin.h"


// CComSetDlg 对话框

class CComSetDlg : public CDialog
{
	DECLARE_DYNAMIC(CComSetDlg)

public:
	CComSetDlg(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CComSetDlg();

// 对话框数据
	enum { IDD = IDD_DLG_COMSET };

	int		m_nPortNum;
protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持
	void	ScanPort();
	INT_PTR	GetSerialPort(CStringArray &arrCom); //遍历计算机中有哪些串口

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
