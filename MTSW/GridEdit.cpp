// GridEdit.cpp : implementation file
//

#include "stdafx.h"
 #include "GridEdit.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CGridEdit

CGridEdit::CGridEdit()
{
	m_nStyle = CGridEdit::Text;
}

CGridEdit::~CGridEdit()
{
}


BEGIN_MESSAGE_MAP(CGridEdit, CEdit)
	//{{AFX_MSG_MAP(CGridEdit)
	ON_WM_CHAR()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CGridEdit message handlers

void CGridEdit::OnChar(UINT nChar, UINT nRepCnt, UINT nFlags) 
{
	// TODO: Add your message handler code here and/or call default
	
	CEdit::OnChar(nChar, nRepCnt, nFlags);
}

BOOL CGridEdit::PreTranslateMessage(MSG* pMsg) 
{
	// TODO: Add your specialized code here and/or call the base class
	
	if( pMsg->message == WM_KEYDOWN)
	{
		if( (pMsg->wParam == VK_ESCAPE)||(pMsg->wParam == VK_RETURN) )
		{
			CWnd *pOwner = GetOwner();
			if (pOwner && IsWindow(pOwner->m_hWnd))
				pOwner->SetFocus();
			else
			{
				CWnd *pChild=GetWindow(GW_HWNDNEXT);
				if(pChild!=NULL)
					pChild->SetFocus();
			}
		}
	}
	return CEdit::PreTranslateMessage(pMsg);
}

CString CGridEdit::GetText()
{
	CString str;
	GetWindowText(str);
	return(str);
}

void CGridEdit::Initialize(int style,UINT nLimit,LPCTSTR lpszString)
{
	m_nStyle = style;
	SetLimitText(nLimit);
	SetWindowText(lpszString);
}

void CGridEdit::HideWindow()
{
	EnableWindow(FALSE);
	ShowWindow(SW_HIDE);
}

void CGridEdit::Initialize(int style, UINT nLimit, LPCTSTR lpszString, LPCRECT lpRect)
{
	Initialize(style,nLimit,lpszString);
	EnableWindow();
	ShowWindow(SW_SHOW);
	MoveWindow(lpRect);
	SetSel(0,-1);
	SetFocus();
}