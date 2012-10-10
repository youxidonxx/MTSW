#pragma once


// CDigitSetDlg 对话框
#include "MTSWDlg.h"
#include "gridctrl.h"
#include "GridEdit.h"
#include "SpanEdit.h"
#include "afxwin.h"



class CDigitSetDlg : public CDialog
{
	DECLARE_DYNAMIC(CDigitSetDlg)
protected:
	BYTE*	m_pData;
	CWnd*	m_pOwner;

	BOOL	m_bSend;
	int		m_nTimerID;
	int		m_nFreqCount;		//根据输入确认列表的行数
	BOOL	m_bEditRowCnt;		//根据是否改变了行数对grid进行重绘
	CCellID		m_nFocusCell;

	CRect	m_RectOri;			//grid原始大小
private:
	int		m_nInterval;
	int		m_nCTCSSSigNum;			//信令号码
	int		m_nCDCSSSigNum;			//信令号码
public:
	CDigitSetDlg(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CDigitSetDlg();


	void	LoadData();
// 对话框数据
	enum { IDD = IDD_DLG_DIGITSET };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	void	SetDigitInfo(int	nFlag,int	nVal);
	WORD	GetDigitInfo(int	nFlag);
	void	SetCommonDigitVal(int	nRow,int	nCol,CString	strVal);
	//存储频点
	void	StoreFreq(int	nRow,int	nCol,CString&	strFreq);
	//判断频点字符串是否包含小数点并对其进行修改
	BOOL	JudgeFreqStr(CString&	strFreq);
	//获取频率大小
	void	SetDigitFreq(int	nFlag,DWORD	nFreq);
	DWORD	GetDigitFreq(int	nFlag);
	//resize grid window , 看是否是拉伸窗口时的大小变化
	void	ResizeGridWnd(BOOL	bOnSize = FALSE);

	BOOL	ConnectToRadio();	//连接串口，进入校准模式
	BOOL	UnconnectToRadio();	//离开校准模式
	void	SendDataToPort(void*	lpBuff,int	nLen,int	comVal);

	DWORD	WriteCommData(void*	lpBuff,int	nLen);
	DWORD	ReadCommData(BYTE*	lpBuff,int	nLen);

	//设置GRID样式
	void	SetGridInfo();
	//设置GRID、滚动条、EDIT框的数值显示
	void	SetCtrlVals();
	//设置频点的组数，获取频点组数
	void	SetFreqNum();
	int		GetFreqNum();
	//设置滚动条和EDIT的显示值
	void	SetScrollEditVals(int	nVal);	//校准时的显示
	//设置STATIC的文字显示
	void	SetCStaticText(int	nRow);
	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL OnInitDialog();
public:
	afx_msg void OnClickGrid(NMHDR * pNMHDR, LRESULT* pResult);
	afx_msg void OnShowWindow(BOOL bShow, UINT nStatus);
	afx_msg		void	OnKillFocusEditGrid();
	afx_msg		void	OnKillFocusEditFreqCnt();
protected:
	CEdit m_editFreqCnt;
public:
	CGridCtrl m_Grid;
public:
	CGridEdit m_editGrid;
public:
	afx_msg void OnBnClickedButtonEditRowcnt();
public:
	afx_msg void OnBnClickedOk();
public:
	afx_msg void OnBnClickedCancel();
public:
	afx_msg void OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar);
public:
	CScrollBar m_ScrollBar;
public:
	CSpanEdit m_editVal;
public:
	afx_msg void OnTimer(UINT_PTR nIDEvent);
};
