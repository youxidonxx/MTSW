// MTSWDlg.h : ͷ�ļ�
//

#pragma once
#include "afxcmn.h"
 #include "CSerial.h"
#include "TabPage.h"
#include "CommonSetDlg.h"
#include "AnalogSetPage.h"
#include "DigitSetDlg.h"


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
	BOOL	IsConnectRadio();
// ʵ��
protected:
	HICON m_hIcon;
	char	m_nParity;	//'N'
	int		m_nComNum;//���ں�
	LONG32	m_nPaud;	//������
	Point	m_point;	//for dlg
	Point	m_pointPage;	//for tabctrl view
	CSerial*	m_pSerial;

	CCommonSetDlg*		m_pCommonDlg;
	CDigitSetDlg*		m_pDigitDlg;
	CAnalogSetPage*	m_pAnalogDlg;

	BYTE*	m_ComData;		//��������
	BYTE*	m_DigitData;	//���ֲ���
	BYTE*	m_AnalogData;	//ģ��У׼����

	void	ViewResize(LPRECT	lpRect)  ;
	void	Resize();
	// ���ɵ���Ϣӳ�亯��
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
