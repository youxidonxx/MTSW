#pragma once


// CAnalogSetPage 对话框

class CAnalogSetPage : public CDialog
{
	DECLARE_DYNAMIC(CAnalogSetPage)

protected:
	BYTE*	m_pData;
	CWnd*	m_pOwner;
public:
	CAnalogSetPage(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CAnalogSetPage();


	void	LoadData();
// 对话框数据
	enum { IDD = IDD_DLG_ANALOGSET };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
};
