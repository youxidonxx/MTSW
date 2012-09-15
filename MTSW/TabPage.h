#pragma once


// CTabPage

class CTabPage : public CTabCtrl
{
	DECLARE_DYNAMIC(CTabPage)

	int		m_nPageCount;	//ҳ��

public:
	CTabPage();
	virtual ~CTabPage();

	void	AddPage(CWnd* pPageWnd, LPTSTR lpTitle);	//���ѡ�
	void	ShowCurPage(UINT nShowState);				//��ʾ��ǰҳ��
	CWnd*	GetCurPage();
public:
	afx_msg void OnTcnSelchanging(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnTcnSelchange(NMHDR *pNMHDR, LRESULT *pResult);

	DECLARE_MESSAGE_MAP()
};