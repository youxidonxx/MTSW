// CommonSetDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "MTSW.h"
#include "CommonSetDlg.h"


// CCommonSetDlg 对话框

IMPLEMENT_DYNAMIC(CCommonSetDlg, CDialog)

CCommonSetDlg::CCommonSetDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CCommonSetDlg::IDD, pParent)
{

}

CCommonSetDlg::~CCommonSetDlg()
{
}

void CCommonSetDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(CCommonSetDlg, CDialog)
END_MESSAGE_MAP()


// CCommonSetDlg 消息处理程序
