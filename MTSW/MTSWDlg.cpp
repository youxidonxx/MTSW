// MTSWDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "MTSW.h"
#include "MTSWDlg.h"
#include "CommonSetDlg.h"
#include "DigitSetDlg.h"
#include "AnalogSetPage.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


static		CString		strTitle[] = {
	_T("公共设置"),
	_T("数字设置"),
	_T("模拟设置")
};
SpMtConnectReq		g_SpMtConn;

long	swapWord(long data,int len)
{
	long	tmp = 0;
	if (len == 4)//0x0000类型
	{
		tmp = (data <<8)|(data>>8);
		return tmp;
	}
	else if (len == 8)//0x0000 0000
	{
		int rightTmp = (int)(data>>16);
		int leftTmp = (int)(data );
		rightTmp = (rightTmp<<8)|(rightTmp>>8);
		leftTmp = (leftTmp<<8)|(leftTmp>>8);

		tmp = (long)rightTmp<<16 |(long)leftTmp;
		return tmp;
	}
	return 0;
}
void	InitCommand()
{
	 	g_SpMtConn.HeaderFrame = 0x5f5f;
	 	g_SpMtConn.MsgLen		 = 0x10;
	 	g_SpMtConn.SpMtConnect.ReceiverMID = swapWord(SYS_MODULE_SP,4);
	 	g_SpMtConn.SpMtConnect.SenderMID   = swapWord(SYS_MODULE_MTSW,4);
	 	g_SpMtConn.SpMtConnect.MsgTYPE	 = MAINTAINANCE;
	 	g_SpMtConn.SpMtConnect.MsgID		 = MT_SP_CONNECT_REQ	;
	 	g_SpMtConn.SpMtConnect.MsgLen		 = 4;
	 	g_SpMtConn.SpMtConnect.connect	 = 0x14511451;
	 	g_SpMtConn.SpMtConnect.CRC			 = 0xffff;
	 	g_SpMtConn.EndFrame			 = 0x55aa;
}


// 用于应用程序“关于”菜单项的 CAboutDlg 对话框

class CAboutDlg : public CDialog
{
public:
	CAboutDlg();

// 对话框数据
	enum { IDD = IDD_ABOUTBOX };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

// 实现
protected:
	DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() : CDialog(CAboutDlg::IDD)
{
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialog)
END_MESSAGE_MAP()


// CMTSWDlg 对话框




CMTSWDlg::CMTSWDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CMTSWDlg::IDD, pParent)
{
	m_nParity = 'N';
	m_nPaud = 115200;
	m_pAnalogDlg = NULL;
	m_pCommonDlg = NULL;
	m_pDigitDlg = NULL;

	m_pSerial = NULL;
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CMTSWDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_TAB, m_TabCtrl);
}

BEGIN_MESSAGE_MAP(CMTSWDlg, CDialog)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	//}}AFX_MSG_MAP
 	ON_BN_CLICKED(IDC_READ, &CMTSWDlg::OnBnClickedRead)
	ON_BN_CLICKED(IDC_WRITE, &CMTSWDlg::OnBnClickedWrite)
	ON_BN_CLICKED(IDC_COMSET, &CMTSWDlg::OnBnClickedComset)
	ON_WM_DESTROY()
	ON_WM_SIZE()
END_MESSAGE_MAP()


// CMTSWDlg 消息处理程序

int CMTSWDlg::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CDialog::OnCreate(lpCreateStruct) == -1)
		return -1;
	int i=0;
// 	m_TabCtrl.InsertItem(i,strTitle[i]);i++;
// 	m_TabCtrl.InsertItem(i,strTitle[i]);i++;
// 	m_TabCtrl.InsertItem(i,strTitle[i]);i++;
	return 0;
}
BOOL CMTSWDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	// 将“关于...”菜单项添加到系统菜单中。

	// IDM_ABOUTBOX 必须在系统命令范围内。
	ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
	ASSERT(IDM_ABOUTBOX < 0xF000);

	CMenu* pSysMenu = GetSystemMenu(FALSE);
	if (pSysMenu != NULL)
	{
		CString strAboutMenu;
		strAboutMenu.LoadString(IDS_ABOUTBOX);
		if (!strAboutMenu.IsEmpty())
		{
			pSysMenu->AppendMenu(MF_SEPARATOR);
			pSysMenu->AppendMenu(MF_STRING, IDM_ABOUTBOX, strAboutMenu);
		}
	}

	// 设置此对话框的图标。当应用程序主窗口不是对话框时，框架将自动
	//  执行此操作
	SetIcon(m_hIcon, TRUE);			// 设置大图标
	SetIcon(m_hIcon, FALSE);		// 设置小图标

	// TODO: 在此添加额外的初始化代码
	CRect	rectPage;

	m_pAnalogDlg = new CAnalogSetPage;
	m_pAnalogDlg->Create(IDD_DLG_ANALOGSET,&m_TabCtrl);

	m_pCommonDlg = new CCommonSetDlg;
	m_pCommonDlg->Create(IDD_DLG_COMMONSET,&m_TabCtrl);

	m_pDigitDlg = new CDigitSetDlg;
	m_pDigitDlg->Create(IDD_DLG_DIGITSET,&m_TabCtrl);

	m_TabCtrl.GetWindowRect(&rectPage);
	ScreenToClient(&rectPage);
	
	m_point.X = rectPage.right - rectPage.left;
	m_point.Y = rectPage.bottom - rectPage.top;

	rectPage.left	-= 40;
	rectPage.top	-= 42;
	rectPage.right	-= 4;
	rectPage.bottom	-= 4;

	m_pAnalogDlg->MoveWindow(&rectPage);
	m_pCommonDlg->MoveWindow(&rectPage);
	m_pDigitDlg->MoveWindow(&rectPage);

	m_TabCtrl.AddPage(m_pCommonDlg,(LPTSTR)(LPCTSTR)strTitle[0]);
	m_TabCtrl.AddPage(m_pDigitDlg,(LPTSTR)(LPCTSTR)strTitle[1]);
	m_TabCtrl.AddPage(m_pAnalogDlg,(LPTSTR)(LPCTSTR)strTitle[2]);


 	return TRUE;  // 除非将焦点设置到控件，否则返回 TRUE
}

void CMTSWDlg::OnSysCommand(UINT nID, LPARAM lParam)
{
	if ((nID & 0xFFF0) == IDM_ABOUTBOX)
	{
		CAboutDlg dlgAbout;
		dlgAbout.DoModal();
	}
	else
	{
		CDialog::OnSysCommand(nID, lParam);
	}
}

// 如果向对话框添加最小化按钮，则需要下面的代码
//  来绘制该图标。对于使用文档/视图模型的 MFC 应用程序，
//  这将由框架自动完成。

void CMTSWDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // 用于绘制的设备上下文

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// 使图标在工作矩形中居中
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// 绘制图标
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialog::OnPaint();
	}
}

//当用户拖动最小化窗口时系统调用此函数取得光标显示。
//
HCURSOR CMTSWDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}


BOOL	CMTSWDlg::IsConnectRadio()
{
	return TRUE;
}
void CMTSWDlg::OnBnClickedRead()
{
	// TODO: 在此添加控件通知处理程序代码
}

void CMTSWDlg::OnBnClickedWrite()
{
	// TODO: 在此添加控件通知处理程序代码
}

void CMTSWDlg::OnBnClickedComset()
{
	// TODO: 在此添加控件通知处理程序代码
	CComSetDlg		comdlg;
	if (comdlg.DoModal() == IDOK)
	{
		m_nComNum = comdlg.m_nPortNum;
 	}
	else
		AfxMessageBox(IDS_STRING_COMNOTSEL);	
}

void CMTSWDlg::OnDestroy()
{
	CDialog::OnDestroy();

	m_pAnalogDlg->DestroyWindow();
	m_pCommonDlg->DestroyWindow();
	m_pDigitDlg->DestroyWindow();

// 	if(m_pSerial->IsOpen())
// 		m_pSerial->CloseComm();
	m_pSerial = NULL;
	// TODO: 在此处添加消息处理程序代码
}

void	CMTSWDlg::Resize()
{
	float	fsp[2];
	CRect	rect;
	Point	NewPoint;

	m_TabCtrl.GetWindowRect(&rect);
	NewPoint.X = rect.right - rect.left;
	NewPoint.Y = rect.bottom - rect.top;
	
	fsp[0] = (float)NewPoint.X/m_point.X;
	fsp[1] = (float)NewPoint.Y/NewPoint.Y;

	int		nID;
	CPoint OldTLPoint,TLPoint; //左上角
	CPoint OldBRPoint,BRPoint; //右下角
	CRect		rectChild;
	HWND	hwndChild = ::GetWindow(m_hWnd,GW_CHILD);
	while (hwndChild)
	{
		nID = ::GetDlgCtrlID(hwndChild);
		GetDlgItem(nID)->GetWindowRect(rectChild);
		ScreenToClient(rectChild);
		OldTLPoint = rectChild.TopLeft();
		TLPoint.x = long(OldTLPoint.x*fsp[0]);
		TLPoint.y = long(OldTLPoint.y*fsp[1]);

		OldBRPoint = rectChild.BottomRight();
		BRPoint.x = long(OldBRPoint.x*fsp[0]);
		BRPoint.y = long(OldBRPoint.y*fsp[1]);

		rectChild.SetRect(TLPoint,BRPoint);
		GetDlgItem(nID)->MoveWindow(Rect,TRUE);
		hwndChild=::GetWindow(hwndChild, GW_HWNDNEXT);   
	}
}
void CMTSWDlg::OnSize(UINT nType, int cx, int cy)
{
	CDialog::OnSize(nType, cx, cy);

	// TODO: 在此处添加消息处理程序代码
	CRect	rect;
	if(m_TabCtrl)
	{
	}

}
