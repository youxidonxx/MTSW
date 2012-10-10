#pragma once


// CDigitSetDlg �Ի���
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
	int		m_nFreqCount;		//��������ȷ���б������
	BOOL	m_bEditRowCnt;		//�����Ƿ�ı���������grid�����ػ�
	CCellID		m_nFocusCell;

	CRect	m_RectOri;			//gridԭʼ��С
private:
	int		m_nInterval;
	int		m_nCTCSSSigNum;			//�������
	int		m_nCDCSSSigNum;			//�������
public:
	CDigitSetDlg(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~CDigitSetDlg();


	void	LoadData();
// �Ի�������
	enum { IDD = IDD_DLG_DIGITSET };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

	void	SetDigitInfo(int	nFlag,int	nVal);
	WORD	GetDigitInfo(int	nFlag);
	void	SetCommonDigitVal(int	nRow,int	nCol,CString	strVal);
	//�洢Ƶ��
	void	StoreFreq(int	nRow,int	nCol,CString&	strFreq);
	//�ж�Ƶ���ַ����Ƿ����С���㲢��������޸�
	BOOL	JudgeFreqStr(CString&	strFreq);
	//��ȡƵ�ʴ�С
	void	SetDigitFreq(int	nFlag,DWORD	nFreq);
	DWORD	GetDigitFreq(int	nFlag);
	//resize grid window , ���Ƿ������촰��ʱ�Ĵ�С�仯
	void	ResizeGridWnd(BOOL	bOnSize = FALSE);

	BOOL	ConnectToRadio();	//���Ӵ��ڣ�����У׼ģʽ
	BOOL	UnconnectToRadio();	//�뿪У׼ģʽ
	void	SendDataToPort(void*	lpBuff,int	nLen,int	comVal);

	DWORD	WriteCommData(void*	lpBuff,int	nLen);
	DWORD	ReadCommData(BYTE*	lpBuff,int	nLen);

	//����GRID��ʽ
	void	SetGridInfo();
	//����GRID����������EDIT�����ֵ��ʾ
	void	SetCtrlVals();
	//����Ƶ�����������ȡƵ������
	void	SetFreqNum();
	int		GetFreqNum();
	//���ù�������EDIT����ʾֵ
	void	SetScrollEditVals(int	nVal);	//У׼ʱ����ʾ
	//����STATIC��������ʾ
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
