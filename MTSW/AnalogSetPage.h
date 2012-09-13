#pragma once


// CAnalogSetPage 对话框

class CAnalogSetPage : public CDialog
{
	DECLARE_DYNAMIC(CAnalogSetPage)

public:
	CAnalogSetPage(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CAnalogSetPage();

// 对话框数据
	enum { IDD = IDD_DLG_ANALOGSET };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
};
