// MTSWDlg.h : 头文件
//

#pragma once
#include "afxcmn.h"
 #include "CSerial.h"
#include "MySerial.h"
#include "pcserialport.h"
#include "SerialPort.h"
#include "TabPage.h"
#include "CommonSetDlg.h"
#include "AnalogSetPage.h"
#include "DigitSetDlg.h"
#include "spstruct.h"

#define		_SerialPort_2


extern	long	swapWord(long data,int len);
extern	SpMtConnectReq		g_SpMtConn;
extern	SpMtMaxvolReq	g_SpMtMaxvol;
extern	SpMtAdReq		g_SpMtAd;
extern	SpMtEffectReq	g_SpMtEffect;
extern	MtSchAnaRXCalReq	g_SpSchRxCal;
extern	MtSchAnaTXCalReq	g_MtSchAnaTxCal;
extern	MtSchDitTXCalReq	g_MtSchDigTxCal;
extern	MtSchCalStop			g_MtSchRxCalStop;

typedef		struct
{
	DWORD	dwTxFreq;
	DWORD	dwRxFreq;
}FreqGroup;
// CMTSWDlg 对话框
class CMTSWDlg : public CDialog
{
// 构造
public:
	CMTSWDlg(CWnd* pParent = NULL);	// 标准构造函数

// 对话框数据
	enum { IDD = IDD_MTSW_DIALOG };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV 支持

public:
	CTabPage m_TabCtrl;

	PlCal_S		m_Cal[5];

	void	WriteEffectRequest();
	void	LoadData();
	void	OpenComm();
	BYTE*	GetCommonDataPtr();
	BYTE*	GetDigitDataPtr();
	BOOL	IsCommOpen();
	BOOL	IsConnectRadio();

	BOOL	ConnectRadio();	//连接串口，进入校准模式
	BOOL	UnconnectRadio();	//离开校准模式

	DWORD	WriteCommData(void*	lpBuff,int	nLen);
	DWORD	ReadCommData(void*	lpBuff,int	nLen);

	DWORD	GetTxFreq();
	DWORD	GetRxFreq();
// 实现
protected:
	char	m_nParity;	//'N'
	int		m_nComNum;//串口号
	LONG32	m_nPaud;	//波特率
	Point	m_point;	//for dlg
	Point	m_pointPage;	//for tabctrl view
	HICON m_hIcon;
	
	CMySerial*	m_pMyComm;
	CSerial*	m_pSerial;
	CSerialPortPC*	m_pPort;
	CSerialPort*	m_pComm;

	CCommonSetDlg*		m_pCommonDlg;
	CDigitSetDlg*		m_pDigitDlg;
	CAnalogSetPage*	m_pAnalogDlg;

	BYTE*	m_pComData;		//公共数据
	BYTE*	m_pCalData;	//包含信道信息的校准数据
	UINT	m_nFreqCnt;	//校准频点组数
	FreqGroup*		m_FreqGrp;

	void	ViewResize(LPRECT	lpRect)  ;
	void	Resize();
	bool	ConnectToRadio();
	void	GetFreqGrp();
	void	WriteDataToPort(BYTE*	pByte,int nLen);
	// 生成的消息映射函数
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()
public:
	afx_msg LONG OnRcvData(WPARAM ch, LPARAM port);
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
public:
	afx_msg void OnBnClickedRead();
public:
	afx_msg void OnBnClickedWrite();
public:
	afx_msg void OnBnClickedComset();
public:
	afx_msg void OnDestroy();
public:
	afx_msg void OnSize(UINT nType, int cx, int cy);
};
