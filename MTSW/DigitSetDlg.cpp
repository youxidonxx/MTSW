// DigitSetDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "MTSW.h"
#include "DigitSetDlg.h"


// CDigitSetDlg 对话框

IMPLEMENT_DYNAMIC(CDigitSetDlg, CDialog)

CDigitSetDlg::CDigitSetDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CDigitSetDlg::IDD, pParent)
{

}

CDigitSetDlg::~CDigitSetDlg()
{
}

void CDigitSetDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(CDigitSetDlg, CDialog)
END_MESSAGE_MAP()


// CDigitSetDlg 消息处理程序
