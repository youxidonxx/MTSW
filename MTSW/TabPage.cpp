// TabPage.cpp : 实现文件
//

#include "stdafx.h"
 #include "TabPage.h"


// CTabPage

IMPLEMENT_DYNAMIC(CTabPage, CTabCtrl)

CTabPage::CTabPage()
{
	m_nPageCount	= 0;
}

CTabPage::~CTabPage()
{
}

void CTabPage::AddPage(CWnd *pPageWnd, LPTSTR lpTitle)
{
	TCITEM	itemPage;

	itemPage.mask		= TCIF_TEXT|TCIF_PARAM;
	itemPage.lParam		= LPARAM(pPageWnd);
	itemPage.pszText	= lpTitle;

	InsertItem(m_nPageCount, &itemPage);
	m_nPageCount==0?pPageWnd->ShowWindow(SW_SHOW):pPageWnd->ShowWindow(SW_HIDE);
	m_nPageCount++;
}

void CTabPage::ShowCurPage(UINT nShowState)
{
	int		nCurItem;
	TCITEM	itemCur;

	nCurItem		= GetCurSel();
	itemCur.mask	= TCIF_PARAM;

	GetItem(nCurItem, &itemCur);
	((CWnd*)itemCur.lParam)->ShowWindow(nShowState);
}
CWnd*	CTabPage::GetCurPage()
{
	int		nCurItem;
	TCITEM	itemCur;

	nCurItem		= GetCurSel();
	itemCur.mask	= TCIF_PARAM;

	GetItem(nCurItem, &itemCur);
	return	((CWnd*)itemCur.lParam);
}

BEGIN_MESSAGE_MAP(CTabPage, CTabCtrl)
	ON_NOTIFY_REFLECT(TCN_SELCHANGING, &CTabPage::OnTcnSelchanging)
	ON_NOTIFY_REFLECT(TCN_SELCHANGE, &CTabPage::OnTcnSelchange)
END_MESSAGE_MAP()

// CTabPage 消息处理程序

void CTabPage::OnTcnSelchanging(NMHDR *pNMHDR, LRESULT *pResult)
{
	ShowCurPage(SW_HIDE);

	*pResult	= 0;
}

void CTabPage::OnTcnSelchange(NMHDR *pNMHDR, LRESULT *pResult)
{
	ShowCurPage(SW_SHOW);

	*pResult	= 0;
}
