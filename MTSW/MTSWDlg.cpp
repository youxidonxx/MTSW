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
SpMtReadReq		g_SpMtRead;
SpMtEffectReq		g_SpMtEffect;
SpMtMaxvolReq		g_SpMtMaxvol;
SpMtAdReq			g_SpMtAd;
MtSchAnaRXCalReq	g_SpSchRxCal;
MtSchAnaTXCalReq	g_MtSchAnaTxCal;
MtSchDitTXCalReq	g_MtSchDigTxCal;

#ifdef _COM

int i=0,k=0;
Sleep(100);
for (i=0;i<100;i++)
{
	if(m_pSerial->IsOpen())
	{
		for (k=0;k<3;k++)
		{
			memcpy(szBuff,&g_SpMtConn,sizeof(SpMtConnectReq));
			m_pSerial->WriteCommData(szBuff,22,10);
			m_pSerial->WriteCommData(szBuff,22,10);

			Sleep(10);
			// 				while(m_pSerial->ReadCommData(1)==0);
			//此处应返回22字节数据
			nRecChar = m_pSerial->ReadCommData(szBuff,22,100);
			if((nRecChar==22) && (szBuff[14]==0x01))
				break;
		}
	}
	//若是最后依然数据不匹配
	if((nRecChar==22) && (szBuff[14]==0x01))
		break;
	//则关闭后重新尝试每个串口
	m_pSerial->CloseComm();
	if (m_pSerial->OpenComm(i,m_nPaud,m_nParity))
	{
		m_nComNum = i;
	}
	else
		return FALSE;
}
#endif 

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
	 	g_SpMtConn.SpMtConnect.ReceiverMID =(uint16)swapWord(SYS_MODULE_SP,4);
	 	g_SpMtConn.SpMtConnect.SenderMID   = (uint16)swapWord(SYS_MODULE_MTSW,4);
	 	g_SpMtConn.SpMtConnect.MsgTYPE	 = MAINTAINANCE;
	 	g_SpMtConn.SpMtConnect.MsgID		 = MT_SP_CONNECT_REQ	;
	 	g_SpMtConn.SpMtConnect.MsgLen		 = 4;
	 	g_SpMtConn.SpMtConnect.connect	 = 0x14511451;
	 	g_SpMtConn.SpMtConnect.CRC			 = 0xffff;
	 	g_SpMtConn.EndFrame			 = 0xaa55;

		g_SpMtRead.HeaderFrame = 0x5f5f;
		g_SpMtRead.MsgLen = 0x10;
		g_SpMtRead.SpMtRead.ReceiverMID = (uint16)swapWord(SYS_MODULE_SP,4);
		g_SpMtRead.SpMtRead.SenderMID	= (uint16)swapWord(SYS_MODULE_MTSW,4);
		g_SpMtRead.SpMtRead.MsgTYPE		= MAINTAINANCE;
		g_SpMtRead.SpMtRead.MsgID			= MT_SP_READ_REQ;
		g_SpMtRead.SpMtRead.MsgLen		= 4;
		g_SpMtRead.SpMtRead.read			= 0x14511451;
		g_SpMtRead.SpMtRead.CRC			= 0xffff;
		g_SpMtRead.EndFrame				= 0xaa55;

		g_SpMtEffect.HeaderFrame = 0x5f5f;
		g_SpMtEffect.MsgLen		= 0x10;
		g_SpMtEffect.SpMtEffect.ReceiverMID = (uint16)swapWord(SYS_MODULE_SP,4);
		g_SpMtEffect.SpMtEffect.SenderMID = (uint16)swapWord(SYS_MODULE_MTSW,4);
		g_SpMtEffect.SpMtEffect.MsgTYPE = MAINTAINANCE;
		g_SpMtEffect.SpMtEffect.MsgID = MT_SP_EFFECT_REQ;
		g_SpMtEffect.SpMtEffect.MsgLen = 4;
		g_SpMtEffect.SpMtEffect.reset = 0x02210221;
		g_SpMtEffect.SpMtEffect.CRC = 0xffff;
		g_SpMtEffect.EndFrame = 0xaa55;

		g_SpMtMaxvol.HeaderFrame = 0x5f5f;
		g_SpMtMaxvol.MsgLen	= 0x10;
		g_SpMtMaxvol.SpMtMaxvol.ReceiverMID = (uint16)swapWord(SYS_MODULE_SP,4);
		g_SpMtMaxvol.SpMtMaxvol.SenderMID = (uint16)swapWord(SYS_MODULE_MTSW,4);
		g_SpMtMaxvol.SpMtMaxvol.MsgTYPE = MAINTAINANCE;
		g_SpMtMaxvol.SpMtMaxvol.MsgID = MT_SP_MAXVOL_REQ;
		g_SpMtMaxvol.SpMtMaxvol.MsgLen = 4;
		g_SpMtMaxvol.SpMtMaxvol.operate = 0x0213;
		g_SpMtMaxvol.SpMtMaxvol.volgain = 0;
		g_SpMtMaxvol.SpMtMaxvol.CRC = 0xffff;
		g_SpMtMaxvol.EndFrame = 0xaa55;

		g_SpMtAd.HeaderFrame = 0x5f5f;
		g_SpMtAd.MsgLen	= 0x10;
		g_SpMtAd.SpMtAd.ReceiverMID = (uint16)swapWord(SYS_MODULE_SP,4);
		g_SpMtAd.SpMtAd.SenderMID = (uint16)swapWord(SYS_MODULE_MTSW,4);
		g_SpMtAd.SpMtAd.MsgTYPE = MAINTAINANCE;
		g_SpMtAd.SpMtAd.MsgID = MT_SP_ADCAL_REQ;
		g_SpMtAd.SpMtAd.MsgLen = 4;
		g_SpMtAd.SpMtAd.operate = 0x2046;
		g_SpMtAd.SpMtAd.checkitem = 0;
		g_SpMtAd.SpMtAd.CRC = 0xffff;
		g_SpMtAd.EndFrame = 0xaa55;

		g_SpSchRxCal.HeaderFrame = 0x5f5f;
		g_SpSchRxCal.MsgLen	= 0x12;
		g_SpSchRxCal.ReceiverMID = (uint16)swapWord(SYS_MODULE_SCH,4);
		g_SpSchRxCal.SenderMID = (uint16)swapWord(SYS_MODULE_MTSW,4);
		g_SpSchRxCal.MsgTYPE = MAINTAINANCE;
		g_SpSchRxCal.MsgID = MT_SCH_ANA_RXCAL_REQ;
		g_SpSchRxCal.MsgContLen = 6;
		g_SpSchRxCal.txananum = 0x00000000;
		g_SpSchRxCal.TV = 0;
		g_SpSchRxCal.CRC = 0xffff;
		g_SpSchRxCal.EndFrame = 0xaa55;

		g_MtSchAnaTxCal.HeaderFrame	= 0x5f5f;
		g_MtSchAnaTxCal.MsgLen		= 0x1e;	
		g_MtSchAnaTxCal.ReceiverMID	= (uint16)swapWord(SYS_MODULE_SCH,4);
		g_MtSchAnaTxCal.SenderMID	= (uint16)swapWord(SYS_MODULE_MTSW,4);
		g_MtSchAnaTxCal.MsgTYPE		= MAINTAINANCE;
		g_MtSchAnaTxCal.MsgID			= MT_SCH_ANA_TXCAL_REQ;
		g_MtSchAnaTxCal.MsgContLen	= 0x12;
		g_MtSchAnaTxCal.txananum		=	0x00000000;
		g_MtSchAnaTxCal.PC			= 0;
		g_MtSchAnaTxCal.Interval		= 0;
		g_MtSchAnaTxCal.SigType		= 0;	
		g_MtSchAnaTxCal.SigNum		= 0;		
		g_MtSchAnaTxCal.MOD1			= 0;		
		g_MtSchAnaTxCal.MOD2			= 0;		
		g_MtSchAnaTxCal.CRC			= 0xffff;
		g_MtSchAnaTxCal.EndFrame		= 0xaa55;

		g_MtSchDigTxCal.HeaderFrame	= 0x5f5f;
		g_MtSchDigTxCal.MsgLen		= 0x18;	
		g_MtSchDigTxCal.ReceiverMID	=  (uint16)swapWord(SYS_MODULE_SCH,4);
		g_MtSchDigTxCal.SenderMID	=	(uint16)swapWord(SYS_MODULE_MTSW,4);
		g_MtSchDigTxCal.MsgTYPE		= MAINTAINANCE;
		g_MtSchDigTxCal.MsgID			= MT_SCH_DIG_TXCAL_REQ;
		g_MtSchDigTxCal.MsgContLen	= 0x0c;
		g_MtSchDigTxCal.txananum		= 0x00000000;
		g_MtSchDigTxCal.PC			= 0;
		g_MtSchDigTxCal.Interval		= 0;
		g_MtSchDigTxCal.MOD1			= 0;		
		g_MtSchDigTxCal.MOD2			= 0;		
		g_MtSchDigTxCal.CRC			= 0xffff;
		g_MtSchDigTxCal.EndFrame		= 0xaa55;

}


void	OutputDebugInfo(int	nInfo,CString	strInfo)
{
	CString		str;
	str.Format("%d:",nInfo);
	str = str+strInfo+'\n';
	OutputDebugString(str);
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
	m_pPort	= NULL;
	m_pComm = new CSerialPort;
	m_pMyComm = new CMySerial;

	m_pComData = new BYTE[0x100];
	m_pCalData = new BYTE[0x1000];
	m_nFreqCnt = 2;
	ZeroMemory(m_pComData,0x100);
	ZeroMemory(m_pCalData,0x1000);
	ZeroMemory(&m_Cal,sizeof(m_Cal));
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void	 CMTSWDlg::DoDataExchange(CDataExchange* pDX)
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
	ON_MESSAGE(WM_COMM_RXCHAR,OnRcvData)
END_MESSAGE_MAP()


// CMTSWDlg 消息处理程序

int		CMTSWDlg::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CDialog::OnCreate(lpCreateStruct) == -1)
		return -1;
	int i=0;
// 	m_TabCtrl.InsertItem(i,strTitle[i]);i++;
// 	m_TabCtrl.InsertItem(i,strTitle[i]);i++;
// 	m_TabCtrl.InsertItem(i,strTitle[i]);i++;
	return 0;
}
BOOL	CMTSWDlg::OnInitDialog()
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
	}

	// 设置此对话框的图标。当应用程序主窗口不是对话框时，框架将自动
	//  执行此操作
	SetIcon(m_hIcon, TRUE);			// 设置大图标
	SetIcon(m_hIcon, FALSE);		// 设置小图标

	// TODO: 在此添加额外的初始化代码
	CRect	rectPage,rectDlg;

	m_pAnalogDlg = new CAnalogSetPage;
	m_pAnalogDlg->Create(IDD_DLG_ANALOGSET,&m_TabCtrl);

	m_pCommonDlg = new CCommonSetDlg;
	m_pCommonDlg->Create(IDD_DLG_COMMONSET,&m_TabCtrl);

	m_pDigitDlg = new CDigitSetDlg;
	m_pDigitDlg->Create(IDD_DLG_DIGITSET,&m_TabCtrl);

	//保存整个对话框大小
	::GetClientRect(m_hWnd,&rectDlg);
	ScreenToClient(&rectDlg);
	m_point.X = rectDlg.right - rectDlg.left;
	m_point.Y = rectDlg.bottom - rectDlg.top;

	m_TabCtrl.GetWindowRect(&rectPage);
	ScreenToClient(&rectPage);
	

	m_pointPage.X = rectPage.left - rectPage.right;
	m_pointPage.Y = rectPage.bottom - rectPage.top;


	CRect	rectItem;
	m_TabCtrl.GetItemRect(0,rectItem);
	
	rectPage.left = rectItem.left;
	rectPage.top = rectItem.bottom;

	m_pAnalogDlg->MoveWindow(&rectPage);
	m_pCommonDlg->MoveWindow(&rectPage);
	m_pDigitDlg->MoveWindow(&rectPage);

	m_TabCtrl.AddPage(m_pCommonDlg,(LPTSTR)(LPCTSTR)strTitle[0]);
	m_TabCtrl.AddPage(m_pDigitDlg,(LPTSTR)(LPCTSTR)strTitle[1]);
	m_TabCtrl.AddPage(m_pAnalogDlg,(LPTSTR)(LPCTSTR)strTitle[2]);

#ifdef	_SerialPC	
	m_pPort = new CSerialPortPC;
#endif
#ifdef _SerialPort_2
	m_pSerial = new CSerial;
#else
#endif

 	InitCommand();
  	return TRUE;  // 除非将焦点设置到控件，否则返回 TRUE
}

void	CMTSWDlg::OnSysCommand(UINT nID, LPARAM lParam)
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

void	CMTSWDlg::OnPaint()
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
#ifdef	_SerialPC	
	m_pPort->m_nPortNum = m_nComNum;
	if(m_pPort->InitPort(this,m_nComNum))
	{
		m_pPort->StartMonitoring();
	}
	else
	{
		CString strcom;
		strcom.Format(_T("串口COM%d没有发现或被占用!"),m_nComNum);
		AfxMessageBox(strcom);
		return FALSE;
	}
#endif
#ifdef _SerialPort_2
	m_pSerial->m_nPort = m_nComNum;
	m_pSerial->m_nPaud = m_nPaud;

// 	m_pMyComm->m_nPort = m_nComNum;
// 	m_pMyComm->m_nPaud = m_nPaud;
// 	if(!m_pMyComm->IsOpen())
// 		m_pMyComm->InitComm(m_nComNum,m_nPaud,m_nParity);

 	if (!m_pSerial->IsOpen())
 	{
 		if(m_pSerial->OpenComm(m_nComNum,m_nPaud,m_nParity) == FALSE)
		{
			AfxMessageBox(IDS_STRING_COMM_BUSY);
			return FALSE;
		}
 	}		
	if(ConnectToRadio())
// 	{
		return TRUE;
// 	}
#else
 	m_pComm->Open(m_nComNum,115200);
		//,CSerialPort::MarkParity,8,
		//CSerialPort::OnePointFiveStopBits,CSerialPort::CtsFlowControlEnable);
	if(ConnectToRadio())
		return TRUE;	
#endif


 	return FALSE;
}
bool	CMTSWDlg::ConnectToRadio()
{
	BYTE	szBuff[0x100];
	int		nRecChar;
	memcpy(szBuff,&g_SpMtConn,sizeof(SpMtConnectReq));
#ifdef	_SerialPC
	m_pPort->WriteToPort((char*)szBuff,22);
#endif
#ifdef _SerialPort_2
// 	if (m_pMyComm->Write(szBuff,22))
// 	{
// 		m_pMyComm->Read(szBuff,22);
// 	}
 	nRecChar = m_pSerial->Write(szBuff,22);
//  	Sleep(10);
 	nRecChar = m_pSerial->Read(szBuff,22);
#else
	m_pComm->Write(szBuff,22);
// 	Sleep(10);
	nRecChar = m_pComm->BytesWaiting();
		//Read(szBuff,22);
	if((nRecChar==22))// && (szBuff[14]==0x01))
		return true;
#endif
	if (nRecChar == 22 && szBuff[14] == 0x01)
	{
		return true;
	}
	return false;
}
LONG	CMTSWDlg::OnRcvData(WPARAM ch, LPARAM port)
{

	return 1;
}
void	CMTSWDlg::WriteDataToPort(BYTE* pByte,int nLen)
{
	if (m_pPort->m_hComm!=NULL)
	{
		m_pPort->WriteToPort((char*)pByte,nLen);
	}
}
void	CMTSWDlg::WriteEffectRequest()
{
	BYTE	szBuff[0x100];
	memcpy(szBuff,&g_SpMtEffect,sizeof(SpMtEffectReq));

	m_pSerial->WriteCommData(szBuff,22,100);
// 	m_pPort->WriteToPort((char*)szBuff,22);

	Sleep(10);
}
/************************************************************************/
/* 数据读取按钮
*/
/************************************************************************/
void	 CMTSWDlg::OnBnClickedRead()
{
	// TODO: 在此添加控件通知处理程序代码
	BYTE	szBuff[0x2000];
	int		nRecChar;
	if (IsConnectRadio())
	{
		memcpy(szBuff,&g_SpMtRead,sizeof(SpMtReadReq));
 		m_pSerial->WriteCommData(szBuff,22,1);
// 		m_pPort->WriteToPort((char*)szBuff,22);
//		m_pComm->Write(szBuff,22);
//  		read cnf
  		nRecChar = m_pSerial->ReadCommData(szBuff,sizeof(SpMtReadCnf_S)+6,500);
 
  		if (nRecChar == 0x11c)
  		{
  			memcpy(szBuff+0x1000,szBuff+18,264);
  		}
  		else
  		{
  			AfxMessageBox(IDS_STRING_READFAILED);
  			WriteEffectRequest();
  			return;
  		}
  		if (szBuff[14] == 0x01)
  		{
  			WriteEffectRequest();
  			//接下来接收effect cnf消息
   			nRecChar = m_pSerial->ReadCommData(szBuff,22,500);
  			if (nRecChar == 22 && szBuff[14] == 0x01)	//长度正确且参数生效success标志
  			{
  				memcpy(m_pComData,szBuff+0x1000,10);	//common data
  				memcpy(m_pCalData,szBuff+0x1000+10,396);//cal data
  				AfxMessageBox(IDS_STRING_READSUCCESS);
  				LoadData();
  			}
  			else
  				AfxMessageBox(IDS_STRING_READFAILED);
  // 			m_pSerial->CloseComm();
  		}
  		else
  		{
  			AfxMessageBox(IDS_STRING_READFAILED);
  		}
	}
	m_pSerial->CloseComm();
	return ;
}

void	 CMTSWDlg::OnBnClickedWrite()
{
	// TODO: 在此添加控件通知处理程序代码
	if(!m_pSerial->IsOpen())
		m_pSerial->OpenComm(m_nComNum,m_nPaud,m_nParity);
	if (ConnectRadio())
	{
		BYTE*	szBuff = new BYTE[0x1000];
	}
}

void	CMTSWDlg::OnBnClickedComset()
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

void	 CMTSWDlg::OnDestroy()
{
	CDialog::OnDestroy();

	m_pAnalogDlg->DestroyWindow();
	m_pCommonDlg->DestroyWindow();
	m_pDigitDlg->DestroyWindow();

// 	if(m_pSerial->IsOpen())
// 		m_pSerial->CloseComm();
// 	m_pSerial = NULL;
	m_pPort = NULL;
	// TODO: 在此处添加消息处理程序代码
}

void	CMTSWDlg::ViewResize(LPRECT	lpRect)
{
	CRect	rect ;
	m_TabCtrl.GetItemRect(0,&rect);
	int	nHeight = rect.Height();//标签页的高度
	int	nY = rect.bottom;
	int	nX = rect.left;
	((CWnd*)m_TabCtrl.GetCurPage())->GetWindowRect(&rect);
	ScreenToClient(&rect);
	CPoint OldTLPoint; //左上角
	CPoint OldBRPoint; //右下角
#if 0
	OldTLPoint.x = lpRect->left;
	OldTLPoint.y = lpRect->top - nHeight;//整个tabctrl控件-标签页高度
	OldBRPoint.x = lpRect->right;
	OldBRPoint.y = lpRect->bottom;
	rect.SetRect(OldTLPoint,OldBRPoint); 
	rect.OffsetRect(CSize(-lpRect->left,-lpRect->top/2));
#endif

	OldTLPoint.x = nX;
	OldTLPoint.y = nY;//lpRect->top - nHeight;//整个tabctrl控件-标签页高度
	OldBRPoint.x = lpRect->right;
	OldBRPoint.y = lpRect->bottom;
	rect.SetRect(OldTLPoint,OldBRPoint); 
  
 	((CWnd*)m_TabCtrl.GetCurPage())->MoveWindow(&rect,TRUE);
// 	m_pAnalogDlg->MoveWindow(&rect,TRUE);
// 	m_pDigitDlg->MoveWindow(&rect,TRUE);

  }
void	CMTSWDlg::Resize()
{ 
	float	fsp[2];
	CRect	rect;
	Point	NewPoint;

	//m_TabCtrl.
		GetWindowRect(&rect);
	NewPoint.X = rect.right - rect.left;
	NewPoint.Y = rect.bottom - rect.top;
	
	fsp[0] = (float)NewPoint.X/m_point.X;
	fsp[1] = (float)NewPoint.Y/m_point.Y;

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
		GetDlgItem(nID)->MoveWindow(rectChild,TRUE);
		if (nID == IDC_TAB)//when it's tabctrl,we need to resize the views inside the tabctrl
		{
			ViewResize(&rectChild);
		}
		hwndChild=::GetWindow(hwndChild, GW_HWNDNEXT);   
	}
	m_point = NewPoint;
}
void	 CMTSWDlg::OnSize(UINT nType, int cx, int cy)
{
	CDialog::OnSize(nType, cx, cy);

	// TODO: 在此处添加消息处理程序代码
 	if(m_TabCtrl)
	{
		Resize();
	}

}

void	CMTSWDlg::LoadData()
{
	GetFreqGrp();
	if (m_pCommonDlg!=NULL)
 		m_pCommonDlg->LoadData();
	if(m_pAnalogDlg!=NULL)
		m_pAnalogDlg->LoadData();
	if(m_pDigitDlg!=NULL)
		m_pDigitDlg->LoadData();
}
void	CMTSWDlg::GetFreqGrp()
{
	m_nFreqCnt = *(WORD*)m_pCalData;
	if(m_nFreqCnt>=2 && m_nFreqCnt<=5)
		m_FreqGrp = new FreqGroup[m_nFreqCnt];
	else
		return;
	int i=0;
	for (;i<m_nFreqCnt;i++)
	{
		m_FreqGrp[i].dwTxFreq = *(DWORD*)(m_pCalData+2+i*STRUCT_LEN_CALCTCCHAL);
		m_FreqGrp[i].dwRxFreq = *(DWORD*)(m_pCalData+2+4+i*STRUCT_LEN_CALCTCCHAL);
		m_Cal[i] = *(PlCal_S*)(m_pCalData+2+i*STRUCT_LEN_CALCTCCHAL);
	}
}
DWORD	CMTSWDlg::GetTxFreq()
{
	return m_Cal[3].CalTxFreq;
}
DWORD	CMTSWDlg::GetRxFreq()
{
	return m_Cal[3].CalRxFreq;
}
BYTE*	CMTSWDlg::GetCommonDataPtr()
{
	BYTE*	pData = m_pComData;
	return pData;
}
BYTE*	CMTSWDlg::GetDigitDataPtr()
{
	BYTE*	pData = m_pCalData;
	return pData;
}
BOOL	CMTSWDlg::IsCommOpen()
{
	return m_pSerial->IsOpen();
}
void	CMTSWDlg::OpenComm()
{
	m_pSerial->OpenComm(m_nComNum,m_nPaud,m_nParity);
}
DWORD	CMTSWDlg::WriteCommData(void* lpBuff,int nLen)
{
	if(nLen<=0)
		return -1;
	BYTE	szBuff[0x2000];
	ZeroMemory(szBuff,nLen);
	int		nRecChar;
	memcpy(szBuff,lpBuff,nLen);
	nRecChar = m_pSerial->Write(szBuff,nLen);
// 	memcpy(lpBuff,szBuff,nRecChar);
	return nRecChar;
}
DWORD	CMTSWDlg::ReadCommData(void* lpBuff,int nLen)
{
	if(nLen<=0)
		return -1;
	BYTE	szBuff[0x2000];
	int		nRecChar;
	memcpy(szBuff,lpBuff,nLen);
	ZeroMemory(szBuff,nLen);
	nRecChar = m_pSerial->Read(szBuff,nLen);
	memcpy(lpBuff,szBuff,nRecChar);
	return nRecChar;
}
BOOL	CMTSWDlg::ConnectRadio()
{
	if (!IsCommOpen())
	{
		OpenComm();
	} 
	BYTE*	szBuff = new BYTE[0x100];
	int	nRecChar;
	ZeroMemory(szBuff,0x100);

	memcpy(szBuff,&g_SpMtConn,sizeof(SpMtConnectReq));
	// mt_sp_connect_req
	nRecChar = WriteCommData(szBuff,sizeof(SpMtConnectReq));
	if (nRecChar == 22)
	{//sp_mt_connect_cnf
		nRecChar = ReadCommData(szBuff,22);
		if (nRecChar == 22 && szBuff[14]== 0x01)
		{
			return TRUE;
		}
		return FALSE;
	}
	return FALSE;
}
BOOL	CMTSWDlg::UnconnectRadio()
{
	//MT_SP_EFFECT_REQ
	BYTE*	szBuff = new BYTE[0x100];
	int	nRecChar;
	ZeroMemory(szBuff,0x100);
	memcpy(szBuff,&g_SpMtEffect,sizeof(SpMtConnectReq));
	nRecChar = WriteCommData(szBuff,sizeof(SpMtConnectReq));
	if (nRecChar == 22)
	{//sp_mt_connect_cnf
		nRecChar = ReadCommData(szBuff,22);
		if (nRecChar == 22 && szBuff[14]== 0x01)
		{
			m_pSerial->CloseComm();
			return TRUE;
		}
		m_pSerial->CloseComm();
		return FALSE;
	}
	m_pSerial->CloseComm();
	return FALSE;

}