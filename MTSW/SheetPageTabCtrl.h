#if !defined(AFX_SHEETPAGETABCTRL_H__CD21A505_C505_4B37_815C_0D0BAE21037D__INCLUDED_)
#define AFX_SHEETPAGETABCTRL_H__CD21A505_C505_4B37_815C_0D0BAE21037D__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// SheetPageTabCtrl.h : header file
//

#include <afxtempl.h>
/////////////////////////////////////////////////////////////////////////////
// CSheetPageTabCtrl window
class CTabCtrlItem
{	
public:
	CWnd *pWnd;
	CString strLabel;
	int iImage;
};

class CSheetPageTabCtrl : public CTabCtrl
{
// Construction
public:
	CSheetPageTabCtrl();

// Attributes
public:

// Operations
public:
//
protected:
	CList<CTabCtrlItem *, CTabCtrlItem *> m_lstTabCtrlItem;
	CWnd *m_pActiveWnd;
// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CSheetPageTabCtrl)
	//}}AFX_VIRTUAL

// Implementation
public:
	CWnd *GetPage(int nIndex);
	CWnd *GetActivePage();
	BOOL SetActivePage(int nIndex);
	int   AddPage(CRuntimeClass *pRuntimeClass, LPCTSTR lpszLabel, UINT nIDD, int iImage);
	void ResizePage();

	virtual ~CSheetPageTabCtrl();

	// Generated message map functions
protected:
	//{{AFX_MSG(CSheetPageTabCtrl)
	afx_msg void OnSize(UINT nType, int cx, int cy);
	//}}AFX_MSG

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnTcnSelchange(NMHDR *pNMHDR, LRESULT *pResult);
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_SHEETPAGETABCTRL_H__CD21A505_C505_4B37_815C_0D0BAE21037D__INCLUDED_)
