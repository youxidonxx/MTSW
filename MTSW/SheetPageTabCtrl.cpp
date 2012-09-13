// SheetPageTabCtrl.cpp : implementation file
//

#include "stdafx.h"
#include "SheetPageTabCtrl.h"
 
#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CSheetPageTabCtrl

CSheetPageTabCtrl::CSheetPageTabCtrl()
{
	m_pActiveWnd = NULL;
}

CSheetPageTabCtrl::~CSheetPageTabCtrl()
{
	POSITION pos = m_lstTabCtrlItem.GetHeadPosition();
	int nCount = m_lstTabCtrlItem.GetCount();
	CTabCtrlItem *pItem = NULL;
	for(int i = 0; i < nCount; i++)
    {
		pItem = (CTabCtrlItem*)m_lstTabCtrlItem.GetNext(pos);
		delete pItem;
	}
	m_lstTabCtrlItem.RemoveAll();
}


BEGIN_MESSAGE_MAP(CSheetPageTabCtrl, CTabCtrl)
	//{{AFX_MSG_MAP(CSheetPageTabCtrl)
	ON_WM_SIZE()
	//}}AFX_MSG_MAP
	ON_NOTIFY_REFLECT(TCN_SELCHANGE, OnTcnSelchange)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CSheetPageTabCtrl message handlers

int CSheetPageTabCtrl::AddPage(CRuntimeClass *pRuntimeClass, LPCTSTR lpszLabel, UINT nIDD, int iImage)
{
	ASSERT_VALID(this);
	ASSERT(pRuntimeClass != NULL);
	ASSERT(AfxIsValidAddress(pRuntimeClass, sizeof(CRuntimeClass), FALSE));

	CDialog *pDlg = (CDialog*)pRuntimeClass->CreateObject();
	if(pDlg == NULL) return -1;
	
	if(pDlg->Create(nIDD, this))
	{
		CTabCtrlItem *pItem = new CTabCtrlItem;
		ASSERT(pItem != NULL);
		
		pItem->iImage = iImage;
		int nIndex = m_lstTabCtrlItem.GetCount();
		pItem->pWnd = (CWnd*)pDlg;
		pItem->strLabel = lpszLabel;
		m_lstTabCtrlItem.AddTail(pItem);
		pDlg->ShowWindow(SW_HIDE);
		InsertItem(nIndex, lpszLabel, iImage);

		return nIndex;
	}
	return -1;
}


BOOL CSheetPageTabCtrl::SetActivePage(int nIndex)
{
	ASSERT(nIndex >= 0);

	if(nIndex >= m_lstTabCtrlItem.GetCount()) return FALSE;
	POSITION pos = m_lstTabCtrlItem.FindIndex(nIndex);
	
	CTabCtrlItem *pItem = (CTabCtrlItem*)m_lstTabCtrlItem.GetNext(pos);
	CWnd *pWnd = (CWnd*)pItem->pWnd;
	
	ASSERT(pWnd != NULL);
	ASSERT(pWnd->GetSafeHwnd());
	if(m_pActiveWnd == pWnd) return TRUE;

	if(m_pActiveWnd != NULL) 
	{
		m_pActiveWnd->EnableWindow(FALSE);
		m_pActiveWnd->ShowWindow(SW_HIDE);
	}
	pWnd->EnableWindow(TRUE);
	pWnd->ShowWindow(SW_SHOW);
	pWnd->SetFocus();

	m_pActiveWnd = (CWnd*)pWnd;
	
	SetCurSel(nIndex);

	return TRUE;
}

CWnd * CSheetPageTabCtrl::GetActivePage()
{
	return m_pActiveWnd;
}

CWnd * CSheetPageTabCtrl::GetPage(int nIndex)
{
	ASSERT(nIndex >= 0);
	if(nIndex >= m_lstTabCtrlItem.GetCount()) return NULL;

	POSITION pos = m_lstTabCtrlItem.FindIndex(nIndex);
	CTabCtrlItem *pItem = (CTabCtrlItem*)m_lstTabCtrlItem.GetNext(pos);
	
	return (CWnd*)pItem->pWnd;
}

void CSheetPageTabCtrl::OnSize(UINT nType, int cx, int cy) 
{
	CTabCtrl::OnSize(nType, cx, cy);
	ResizePage();
}

void CSheetPageTabCtrl::ResizePage()
{
	CTabCtrlItem *pItem = NULL;
	CWnd *pWnd = NULL;

	POSITION pos = m_lstTabCtrlItem.GetHeadPosition();
	int nCount = m_lstTabCtrlItem.GetCount();

	CRect rect;
	BOOL bGetRect = GetItemRect(0, &rect);
	int nHeight = 22;
	if(bGetRect) nHeight = rect.Height() + 4;
	
	GetClientRect(&rect);
	int ncx = rect.Width() - 4;
	int ncy = rect.Height() - 4;

    for(int i = 0; i < nCount; i++)
    {
        pItem = (CTabCtrlItem*)m_lstTabCtrlItem.GetNext(pos);
		ASSERT(pItem);
		pWnd = (CWnd*)pItem->pWnd;
		ASSERT_VALID(pWnd);
		pWnd->ShowWindow(SW_SHOW);

		if(GetStyle() & TCS_BOTTOM)	
			pWnd->MoveWindow(2, 2, ncx, ncy - nHeight);
		else	
			pWnd->MoveWindow(2, nHeight, ncx, ncy - nHeight);

		pWnd->ShowWindow(SW_HIDE);
    }
	if(m_pActiveWnd) m_pActiveWnd->ShowWindow(SW_SHOW);
}
void CSheetPageTabCtrl::OnTcnSelchange(NMHDR *pNMHDR, LRESULT *pResult)
{
	// TODO: 在此添加控件通知处理程序代码
	int nIndex = GetCurSel();
	SetActivePage(nIndex);

	*pResult = 0;
}
