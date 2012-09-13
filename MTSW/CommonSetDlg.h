#pragma once


// CCommonSetDlg 对话框

class CCommonSetDlg : public CDialog
{
	DECLARE_DYNAMIC(CCommonSetDlg)

public:
	CCommonSetDlg(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CCommonSetDlg();

// 对话框数据
	enum { IDD = IDD_DLG_COMMONSET };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
};
