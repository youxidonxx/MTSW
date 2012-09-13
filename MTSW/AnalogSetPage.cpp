// AnalogSetPage.cpp : 实现文件
//

#include "stdafx.h"
#include "MTSW.h"
#include "AnalogSetPage.h"


// CAnalogSetPage 对话框

IMPLEMENT_DYNAMIC(CAnalogSetPage, CDialog)

CAnalogSetPage::CAnalogSetPage(CWnd* pParent /*=NULL*/)
	: CDialog(CAnalogSetPage::IDD, pParent)
{

}

CAnalogSetPage::~CAnalogSetPage()
{
}

void CAnalogSetPage::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(CAnalogSetPage, CDialog)
END_MESSAGE_MAP()


// CAnalogSetPage 消息处理程序
