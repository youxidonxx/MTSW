// SpanEdit.cpp : implementation file
//

#include "stdafx.h"
//#include "LinkTopSW.h"
#include "SpanEdit.h"
#include "math.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CSpanEdit

CSpanEdit::CSpanEdit()
{
	m_nStyle=CSpanEdit::INTEGER;
}

CSpanEdit::~CSpanEdit()
{
}


BEGIN_MESSAGE_MAP(CSpanEdit, CEdit)
	//{{AFX_MSG_MAP(CSpanEdit)
	ON_WM_CHAR()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CSpanEdit message handlers

void CSpanEdit::SetRange(int min,int max,int nLimit,int delta,int bSpan)
{
	m_nMinValue	= min;
	m_nMaxValue	= max;
	m_nDelta	= delta;
	m_bSpan		= bSpan;
	LimitText(nLimit);
}

int CSpanEdit::DeltaDecimal(int step)
{
	int	n;
	double f;
	CString str;
	GetWindowText(str);
	f=wcstod(str,NULL)+m_nDelta/2.0;
		//atof(str)*10+m_nDelta/2.0;
 	if(step>0)
	{
		if(m_bSpecial&&(str==m_strSpecial)&&m_bSpan)
			n=m_nMinValue;
		else
		{
			f=f+m_nDelta;
			n=((int)(f/m_nDelta))*m_nDelta;
			if(n>m_nMaxValue)
			{
				if(m_bSpan)
				{
					if(m_bSpecial)
						n=m_nSpecialValue;
					else
						n=m_nMinValue;
				}
				else
					n=m_nMaxValue;
			}
		}
	}
	else if(step<0)
	{
		if(m_bSpecial&&(str==m_strSpecial)&&m_bSpan)
			n=m_nMaxValue;
		else
		{
			f=f-m_nDelta;
			n=((int)(f/m_nDelta))*m_nDelta;
			if(n<m_nMinValue)
			{
				if(m_bSpan)
				{
					if(m_bSpecial)
						n=m_nSpecialValue;
					else
						n=m_nMaxValue;
				}
				else
					n=m_nMinValue;
			}
		}
	}
	else
	{
		if(m_bSpecial&&((str==m_strSpecial)||str.IsEmpty()))
			n=m_nSpecialValue;
		else
		{
			n=((int)(f/m_nDelta))*m_nDelta;			
			if(n<m_nMinValue)
				n=m_nMinValue;
			else if(n>m_nMaxValue)
				n=m_nMaxValue;
		}
	}
	SetDecimal(n,m_bSpecial);
	return(n);
}

void CSpanEdit::SetInteger(int value,BOOL special)
{
	CString str;
	m_bSpecial=special;

	if(special&&(value==m_nSpecialValue))
		str=m_strSpecial;
	else
		str.Format(_T("%d"),value);

	SetWindowText(str);
	m_nStyle=CSpanEdit::INTEGER;
}

int CSpanEdit::DeltaInteger(int step)
{
	int	n;
	double f;
	CString str;
	GetWindowText(str);
	f=wcstod(str,NULL);
		//atof(str);	//+m_nDelta/2.0;

	if(step>0)
	{
		if(m_bSpecial&&(str==m_strSpecial)&&m_bSpan)
			n=m_nMinValue;
		else
		{
			f=f+m_nDelta;
			n=((int)(f/m_nDelta))*m_nDelta;
			if(f>m_nMaxValue)
			{
				if(m_bSpan)
				{
					if(m_bSpecial)
						n=m_nSpecialValue;
					else
						n=m_nMinValue;
				}
				else
					n=m_nMaxValue;
			}
		}
	}
	else if(step<0)
	{
		if(m_bSpecial&&(str==m_strSpecial)&&m_bSpan)
			n=m_nMaxValue;
		else
		{
			f=f-m_nDelta;
			n=((int)(f/m_nDelta))*m_nDelta;
			if(f<m_nMinValue)
			{
				if(m_bSpan)
				{
					if(m_bSpecial)
						n=m_nSpecialValue;
					else
						n=m_nMaxValue;
				}
				else
					n=m_nMinValue;
			}
		}
	}
	else
	{
		if(m_bSpecial&&((str==m_strSpecial)||str.IsEmpty()))
			n=m_nSpecialValue;
		else
		{
			n=((int)(f/m_nDelta))*m_nDelta;
			if(f<m_nMinValue)
				n=m_nMinValue;
			else if(f>m_nMaxValue)
				n=m_nMaxValue;
		}
	}
	SetInteger(n,m_bSpecial);
	return(n);
}

BOOL CSpanEdit::PreTranslateMessage(MSG* pMsg) 
{
	// TODO: Add your specialized code here and/or call the base class
	if( pMsg->message == WM_KEYDOWN)
	{
		if( (pMsg->wParam == VK_ESCAPE)||(pMsg->wParam == VK_RETURN) )
		{
			CWnd *pOwner = GetOwner();
			if (pOwner && IsWindow(pOwner->m_hWnd))
				pOwner->SetFocus();
		}
	}
	return CEdit::PreTranslateMessage(pMsg);
}

void CSpanEdit::SetHex(int value)
{
	CString str;
	str.Format(_T("%1X"),value);
	SetWindowText(str);
	m_nStyle=CSpanEdit::HEX;
}

int CSpanEdit::DeltaHex(int step)
{
	int n;
	CString str;
	GetWindowText(str);
	if(str.IsEmpty())
		n='0';
	else
		n=toupper(str.GetAt(0));
	if((n>='0')&&(n<='9'))
		n=n-'0';
	else if((n>='A')&&(n<='F'))
		n=n-'A'+10;
	else
		n=0;
	if(step>0)
	{
		if(n<15)
			n++;
		else if(m_bSpan)
			n=0;
	}
	else if(step<0)
	{
		if(n>0)
			n--;
		else if(m_bSpan)
			n=15;
	}
	SetHex(n);
	return(n);
}

void CSpanEdit::SetDecimal(int value, BOOL special)
{
	CString str;
	m_bSpecial=special;
	if(special&&(value==m_nSpecialValue))
		str=m_strSpecial;
	else
		str.Format(_T("%d.%d"),value/10,value%10);
	SetWindowText(str);
	m_nStyle=CSpanEdit::DECIMAL;
}

void CSpanEdit::SetSpecialValue(int value,CString& str)
{
	m_nSpecialValue	= value;
	m_strSpecial	= str;
}

void CSpanEdit::OnChar(UINT nChar, UINT nRepCnt, UINT nFlags) 
{
	// TODO: Add your message handler code here and/or call default
	BOOL bRet = FALSE;
	switch(nChar)
	{
	case '0':
	case '1':
	case '2':
	case '3':
	case '4':
	case '5':
	case '6':
	case '7':
	case '8':
	case '9':
		bRet=TRUE;
		break;
	case 'A':
	case 'a':
	case 'B':
	case 'b':
	case 'C':
	case 'c':
	case 'D':
	case 'd':
	case 'E':
	case 'e':
	case 'F':
	case 'f':
		if(m_nStyle==CSpanEdit::HEX)
			bRet=TRUE;
		break;
	case VK_BACK:
		bRet=TRUE;
		break;
	case '.':
		if(m_nStyle==CSpanEdit::DECIMAL)
		{
			CString str;
			GetWindowText(str);
			if(str.Find('.')<0)
				bRet=TRUE;
		}
		break;
	default:
		break;
	}
	if(bRet)
		CEdit::OnChar(nChar, nRepCnt, nFlags);
}

void CSpanEdit::FreqLimitInput()
{
	SetLimitText(10);
	m_nStyle = CSpanEdit::DECIMAL;
}
