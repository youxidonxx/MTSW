// MTSWDlg.h : ͷ�ļ�
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
// CMTSWDlg �Ի���
class CMTSWDlg : public CDialog
{
// ����
public:
	CMTSWDlg(CWnd* pParent = NULL);	// ��׼���캯��

// �Ի�������
	enum { IDD = IDD_MTSW_DIALOG };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV ֧��

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

	BOOL	ConnectRadio();	//���Ӵ��ڣ�����У׼ģʽ
	BOOL	UnconnectRadio();	//�뿪У׼ģʽ

	DWORD	WriteCommData(void*	lpBuff,int	nLen);
	DWORD	ReadCommData(void*	lpBuff,int	nLen);

	DWORD	GetTxFreq();
	DWORD	GetRxFreq();
// ʵ��
protected:
	char	m_nParity;	//'N'
	int		m_nComNum;//���ں�
	LONG32	m_nPaud;	//������
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

	BYTE*	m_pComData;		//��������
	BYTE*	m_pCalData;	//�����ŵ���Ϣ��У׼����
	UINT	m_nFreqCnt;	//У׼Ƶ������
	FreqGroup*		m_FreqGrp;

	void	ViewResize(LPRECT	lpRect)  ;
	void	Resize();
	bool	ConnectToRadio();
	void	GetFreqGrp();
	void	WriteDataToPort(BYTE*	pByte,int nLen);
	// ���ɵ���Ϣӳ�亯��
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
