#if !defined(AFX_GRIDEDIT_H__C98A5F46_C163_43DA_8BC4_85306296D17A__INCLUDED_)
#define AFX_GRIDEDIT_H__C98A5F46_C163_43DA_8BC4_85306296D17A__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// GridEdit.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CGridEdit window

class CGridEdit : public CEdit
{
// Construction
public:
	CGridEdit();

// Attributes
public:
	enum estyle
	{
		Text=0,
		Name=0,
		Freq=0
	};
// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CGridEdit)
	public:
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~CGridEdit();
	void Initialize(int style,UINT nLimit,LPCTSTR lpszString,LPCRECT lpRect);
	void HideWindow();
	void Initialize(int style,UINT nLimit,LPCTSTR lpszString);
	CString	GetText();
	// Generated message map functions
protected:
	int m_nStyle;
	//{{AFX_MSG(CGridEdit)
	afx_msg void OnChar(UINT nChar, UINT nRepCnt, UINT nFlags);
	//}}AFX_MSG

	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_GRIDEDIT_H__C98A5F46_C163_43DA_8BC4_85306296D17A__INCLUDED_)
