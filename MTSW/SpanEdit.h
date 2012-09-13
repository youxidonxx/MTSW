#if !defined(AFX_SPANEDIT_H__3A4BDCA9_3E37_484C_B41B_9D85B11D9D07__INCLUDED_)
#define AFX_SPANEDIT_H__3A4BDCA9_3E37_484C_B41B_9D85B11D9D07__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// SpanEdit.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CSpanEdit window

class CSpanEdit : public CEdit
{
// Construction
public:
	CSpanEdit();

// Attributes
public:
	enum estyle
	{
		INTEGER=0,
		DECIMAL,
		HEX
	};

// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CSpanEdit)
	public:
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	//}}AFX_VIRTUAL

// Implementation
public:
	void FreqLimitInput();
	void	SetSpecialValue(int value,CString& str);
	void	SetDecimal(int value,BOOL special = FALSE);
	void	SetRange(int min,int max,int nLimit=0,int delta=1,int bSpan=TRUE);
	int		DeltaDecimal(int step=0);

	void	SetInteger(int value,BOOL special=FALSE);
	int		DeltaInteger(int step);

	void	SetHex(int value);
	int		DeltaHex(int step);
	virtual ~CSpanEdit();

	// Generated message map functions
protected:
	int	m_nStyle;
	int	m_nMinValue,m_nMaxValue;
	int	m_nDelta,m_nScale;

	int	m_nSpecialValue;
	CString m_strSpecial;

	BOOL m_bSpan;
	BOOL m_bSpecial;
	//{{AFX_MSG(CSpanEdit)
	afx_msg void OnChar(UINT nChar, UINT nRepCnt, UINT nFlags);
	//}}AFX_MSG

	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_SPANEDIT_H__3A4BDCA9_3E37_484C_B41B_9D85B11D9D07__INCLUDED_)
