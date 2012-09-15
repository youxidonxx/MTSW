// MTSWDlg.h : 头文件
//

#pragma once
#include "afxcmn.h"
 #include "CSerial.h"
#include "TabPage.h"
#include "CommonSetDlg.h"
#include "AnalogSetPage.h"
#include "DigitSetDlg.h"


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
	BOOL	IsConnectRadio();
// 实现
protected:
	HICON m_hIcon;
	char	m_nParity;	//'N'
	int		m_nComNum;//串口号
	LONG32	m_nPaud;	//波特率
	Point	m_point;	//for dlg
	Point	m_pointPage;	//for tabctrl view
	CSerial*	m_pSerial;

	CCommonSetDlg*		m_pCommonDlg;
	CDigitSetDlg*		m_pDigitDlg;
	CAnalogSetPage*	m_pAnalogDlg;

	BYTE*	m_ComData;		//公共数据
	BYTE*	m_DigitData;	//数字部分
	BYTE*	m_AnalogData;	//模拟校准数据

	void	ViewResize(LPRECT	lpRect)  ;
	void	Resize();
	// 生成的消息映射函数
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()
public:
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
