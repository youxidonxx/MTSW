#pragma once


// CTabPage

class CTabPage : public CTabCtrl
{
	DECLARE_DYNAMIC(CTabPage)

	int		m_nPageCount;	//页数

public:
	CTabPage();
	virtual ~CTabPage();

	void	AddPage(CWnd* pPageWnd, LPTSTR lpTitle);	//添加选项卡
	void	ShowCurPage(UINT nShowState);				//显示当前页面
	CWnd*	GetCurPage();
public:
	afx_msg void OnTcnSelchanging(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnTcnSelchange(NMHDR *pNMHDR, LRESULT *pResult);

	DECLARE_MESSAGE_MAP()
};