// ComSetDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "MTSW.h"
#include "ComSetDlg.h"


// CComSetDlg 对话框

IMPLEMENT_DYNAMIC(CComSetDlg, CDialog)

CComSetDlg::CComSetDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CComSetDlg::IDD, pParent)
{
	m_nPortNum = -1;
}

CComSetDlg::~CComSetDlg()
{
}

void CComSetDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_COMBO_COMSET, m_cbComSet);
}


BEGIN_MESSAGE_MAP(CComSetDlg, CDialog)
	ON_CBN_DROPDOWN(IDC_COMBO_COMSET, &CComSetDlg::OnCbnDropdownComboComset)
	ON_CBN_SELCHANGE(IDC_COMBO_COMSET, &CComSetDlg::OnCbnSelchangeComboComset)
	ON_BN_CLICKED(IDOK, &CComSetDlg::OnBnClickedOk)
	ON_BN_CLICKED(IDCANCEL, &CComSetDlg::OnBnClickedCancel)
END_MESSAGE_MAP()


// CComSetDlg 消息处理程序
INT_PTR		CComSetDlg::GetSerialPort(CStringArray &arrCom)
{
	arrCom.RemoveAll();
	HKEY	hkey;
	LONG32	lRes = RegOpenKeyEx(
		HKEY_LOCAL_MACHINE,_T("HARDWARE\\DEVICEMAP\\SERIALCOMM"),
		NULL,KEY_QUERY_VALUE|KEY_ENUMERATE_SUB_KEYS|KEY_READ,&hkey);
	if (lRes == ERROR_SUCCESS)
	{
		TCHAR	tchKey[MAX_PATH];
		TCHAR	tchValue[20];
		DWORD	dwIndex = 0;
		DWORD	dwType = REG_SZ;
		while(lRes == ERROR_SUCCESS)
		{
			DWORD	dwCnt = MAX_PATH;
			DWORD	dwVCount = 20;
			lRes = RegEnumValue(hkey,dwIndex++,tchKey,&dwCnt,NULL,
				&dwType,(LPBYTE)tchValue,&dwVCount);
			if (lRes == ERROR_SUCCESS)
			{
				if(dwVCount >0 && dwCnt >0)
					arrCom.Add(tchValue);
			}
		}
	}
	RegCloseKey(hkey);
	return arrCom.GetSize();
}
void	CComSetDlg::ScanPort()
{
	((CComboBox*)GetDlgItem(IDC_COMBO_COMSET))->ResetContent();
	CStringArray	arrayComm;
	int i=0;
	for (;i<GetSerialPort(arrayComm);i++)
	{
		((CComboBox*)GetDlgItem(IDC_COMBO_COMSET))->AddString(arrayComm[i]);
	}
	((CComboBox*)GetDlgItem(IDC_COMBO_COMSET))->SetCurSel(i-1);
}



BOOL CComSetDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	// TODO:  在此添加额外的初始化
	ScanPort();
	return TRUE;  // return TRUE unless you set the focus to a control
	// 异常: OCX 属性页应返回 FALSE
}
void CComSetDlg::OnCbnDropdownComboComset()
{
	// TODO: 在此添加控件通知处理程序代码
	ScanPort();
}

void CComSetDlg::OnCbnSelchangeComboComset()
{
	// TODO: 在此添加控件通知处理程序代码
}

void CComSetDlg::OnBnClickedOk()
{
	// TODO: 在此添加控件通知处理程序代码
	int nSel = m_cbComSet.GetCurSel();
	CString		str;
	m_cbComSet.GetLBText(nSel,str);
	str = str.Right(wcslen(str)-3);
	m_nPortNum = _ttoi(str);
	OnOK();
 }

void CComSetDlg::OnBnClickedCancel()
{
	// TODO: 在此添加控件通知处理程序代码
	OnCancel();
}
