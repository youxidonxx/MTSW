#pragma once
#include "stdafx.h"
#include "GridEdit.h"
#include "gridctrl.h"
#include "SpanEdit.h"
#include "afxwin.h"


// CCommonSetDlg �Ի���

class CCommonSetDlg : public CDialog
{
	DECLARE_DYNAMIC(CCommonSetDlg)

public:
	CCommonSetDlg(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~CCommonSetDlg();
	BYTE*		GetCommonByte();

	void	LoadData();
// �Ի�������
	enum { IDD = IDD_DLG_COMMONSET };
protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��
	void	SetGlobalVal();
	void	SetGridInfo();
	void	SetEditScrollVal(int	nVal);
	void	ShowValuesOnCtrls();
	void	SetCommonInfo(int	nFlag,int	nVal);
	UINT	GetCommonInfo(int	nFlag);
	BOOL	ConnectToRadio();	//���Ӵ��ڣ�����У׼ģʽ
	BOOL	UnconnectToRadio();	//�뿪У׼ģʽ

	UINT	m_nCurrentRow;		//��ǰ��ѡ�����
	BOOL	m_bSend;
	int		m_nTimerID;
	UINT	m_nVal;
	BYTE*	m_pData;
	CWnd*	m_pOwner;
	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL OnInitDialog();
public:
	afx_msg void OnDeltaposSpinEditValue(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnClickGrid(NMHDR * pNMHDR, LRESULT* pResult);
	afx_msg void OnTimer(UINT nIDEvent);
	afx_msg void OnShowWindow(BOOL bShow, UINT nStatus);
public:
	CGridCtrl m_Grid;
public:
	CScrollBar m_ScrBar;
public:
	CSpanEdit m_EditValue;
	CGridEdit m_editGrid;
public:
	CStatic m_StaticTitle;
public:
	afx_msg void OnBnClickedOk();
	afx_msg void OnBnClickedStop();
	afx_msg void OnBnClickedEdit();
	afx_msg void OnBnClickedSure();
	afx_msg void OnEnKillfocusEditGrid();
	void	SendDataToPort(void*	lpBuff,int	nLen,int	comVal);
public:
	afx_msg void OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar);
};
