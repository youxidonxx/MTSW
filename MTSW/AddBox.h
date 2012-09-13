#if !defined(AFX_ADDBOX_H__4DE6BB4D_5AD5_463F_A533_CE9F235EE855__INCLUDED_)
#define AFX_ADDBOX_H__4DE6BB4D_5AD5_463F_A533_CE9F235EE855__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// AddBox.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CAddBox window

class CAddBox : public CComboBox
{
// Construction
public:
	CAddBox();

// Attributes
public:

// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CAddBox)
	public:
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	//}}AFX_VIRTUAL

// Implementation
public:
	void Init(RECT rect,const CString& str);
	void End();
	virtual ~CAddBox();
	// Generated message map functions
protected:
	CString szOrigin;
	//{{AFX_MSG(CAddBox)
		// NOTE - the ClassWizard will add and remove member functions here.
	//}}AFX_MSG

	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_ADDBOX_H__4DE6BB4D_5AD5_463F_A533_CE9F235EE855__INCLUDED_)
