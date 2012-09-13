// AddBox.cpp : implementation file
//

#include "stdafx.h"
 #include "AddBox.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CAddBox

CAddBox::CAddBox()
{
}

CAddBox::~CAddBox()
{
}

BEGIN_MESSAGE_MAP(CAddBox, CComboBox)
	//{{AFX_MSG_MAP(CAddBox)
		// NOTE - the ClassWizard will add and remove mapping macros here.
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CAddBox message handlers
void CAddBox::Init(RECT rect, const CString &str)
{
	CRect rectMe;
	ShowWindow(SW_SHOW);
	EnableWindow();
	SetWindowText(str);
	szOrigin=str;
	SelectString(-1,str);
	GetWindowRect(&rectMe);
	rect.bottom=rect.top+rectMe.Height();
	MoveWindow(&rect);
	SetFocus();
}

void CAddBox::End()
{
	EnableWindow(FALSE);
	ShowWindow(SW_HIDE);
}

BOOL CAddBox::PreTranslateMessage(MSG* pMsg) 
{
	// TODO: Add your specialized code here and/or call the base class
	if( pMsg->message == WM_KEYDOWN)
	{
		if(pMsg->wParam == VK_ESCAPE)
			SelectString(-1,szOrigin);
		if( (pMsg->wParam == VK_ESCAPE)||(pMsg->wParam == VK_RETURN) )
		{
			CWnd *pOwner = GetOwner();
			if (pOwner && IsWindow(pOwner->m_hWnd))
				pOwner->SetFocus();
		}
	}
	return CComboBox::PreTranslateMessage(pMsg);
}
