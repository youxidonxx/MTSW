// CommonSetDlg.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "MTSW.h"
#include "CommonSetDlg.h"
#include "MTSWDlg.h"


// CCommonSetDlg �Ի���

MtSchAnaFreqCalReq		g_MtSchFreqCal;
MtSchAnaFreqCalStop		g_MtSchFreqStop;
MtSchCalStop			g_MtSchRxCalStop;

IMPLEMENT_DYNAMIC(CCommonSetDlg, CDialog)

CCommonSetDlg::CCommonSetDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CCommonSetDlg::IDD, pParent)
{
	m_pParentWnd = pParent;
	m_nCurrentRow = 0;
	m_bSend = FALSE;
	m_pData = new BYTE[0x100];
	ZeroMemory(m_pData,0x100);
}

CCommonSetDlg::~CCommonSetDlg()
{
}

void	CCommonSetDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	// 	DDX_Control(pDX, IDC_CUSTOM, m_Grid);
	DDX_GridControl(pDX,IDC_CUSTOM,m_Grid);
	DDX_Control(pDX, IDC_SCROLLBAR, m_ScrBar);
	DDX_Control(pDX, IDC_EDIT_VALUE, m_EditValue);
	DDX_Control(pDX, IDC_STATIC_TITLE, m_StaticTitle);
	DDX_Control(pDX, IDC_EDIT_GRID, m_editGrid);
}


BEGIN_MESSAGE_MAP(CCommonSetDlg, CDialog)
	ON_WM_SHOWWINDOW()
	ON_NOTIFY(UDN_DELTAPOS, IDC_SPIN_VALUE, OnDeltaposSpinEditValue)
	ON_NOTIFY(GVN_BEGINLABELEDIT, IDC_CUSTOM, OnClickGrid)
	ON_BN_CLICKED(IDOK, &CCommonSetDlg::OnBnClickedOk)
	ON_BN_CLICKED(IDCANCEL,&CCommonSetDlg::OnBnClickedStop)
	ON_BN_CLICKED(IDC_BUTTON_EDIT,&CCommonSetDlg::OnBnClickedEdit)
	ON_BN_CLICKED(IDC_BUTTON_SURE,&CCommonSetDlg::OnBnClickedSure)
	ON_BN_KILLFOCUS(IDC_EDIT_GRID, &CCommonSetDlg::OnEnKillfocusEditGrid)
	ON_WM_HSCROLL()
	ON_WM_TIMER()
END_MESSAGE_MAP()


// CCommonSetDlg ��Ϣ�������
void	CCommonSetDlg::LoadData()
{
	m_pOwner = GetOwner();
	m_pData = ((CMTSWDlg*)((CTabCtrl*)m_pOwner->GetOwner()))->GetCommonDataPtr();
	SetGridInfo();
	ShowValuesOnCtrls();
}
void	CCommonSetDlg::SetGlobalVal()
{
	g_MtSchFreqCal.HeaderFrame = 0x5f5f;
	g_MtSchFreqCal.MsgLen	= 0x12;
	g_MtSchFreqCal.ReceiverMID = (uint16)swapWord(SYS_MODULE_MTSW,4);
	g_MtSchFreqCal.SenderMID = (uint16)swapWord(SYS_MODULE_SCH,4);
	g_MtSchFreqCal.MsgTYPE = MAINTAINANCE;
	g_MtSchFreqCal.MsgID = MT_SCH_ANA_FREQCAL_REQ;
	g_MtSchFreqCal.MsgContLen = 6;
	g_MtSchFreqCal.txananum = 0x00000000;
	g_MtSchFreqCal.VCCN = 0;
	g_MtSchFreqCal.CRC = 0xffff;
	g_MtSchFreqCal.EndFrame = 0xaa55;

	g_MtSchFreqStop.HeaderFrame = 0x5f5f;
	g_MtSchFreqStop.MsgLen	= 0x0c;
	g_MtSchFreqStop.ReceiverMID = (uint16)swapWord(SYS_MODULE_MTSW,4);
	g_MtSchFreqStop.SenderMID = (uint16)swapWord(SYS_MODULE_SCH,4);
	g_MtSchFreqStop.MsgTYPE = MAINTAINANCE;
	g_MtSchFreqStop.MsgID = MT_SCH_ANA_FREQCAL_STOP;
	g_MtSchFreqStop.MsgContLen = 0;
	g_MtSchFreqStop.CRC = 0xffff;
	g_MtSchFreqStop.EndFrame = 0xaa55;

	g_MtSchRxCalStop.HeaderFrame = 0x5f5f;
	g_MtSchRxCalStop.MsgLen	= 0x0c;
	g_MtSchRxCalStop.ReceiverMID = (uint16)swapWord(SYS_MODULE_SCH,4);
	g_MtSchRxCalStop.SenderMID = (uint16)swapWord(SYS_MODULE_MTSW,4);
	g_MtSchRxCalStop.MsgTYPE = MAINTAINANCE;
	g_MtSchRxCalStop.MsgID = MT_SCH_ANA_FREQCAL_STOP;
	g_MtSchRxCalStop.MsgContLen = 0;
	g_MtSchRxCalStop.CRC = 0xffff;
	g_MtSchRxCalStop.EndFrame = 0xaa55;

}
void	CCommonSetDlg::SetGridInfo()
{
	GV_ITEM		item;
	m_Grid.SetRowCount(COMMON_SETTING_NUM);
	m_Grid.SetColumnCount(2);
	m_Grid.SetFixedColumnCount(1);
	m_Grid.SetFixedRowCount(1);

	m_Grid.EnableSelection(FALSE);
 	m_Grid.SetColumnWidth(0,120);
	m_Grid.SetColumnWidth(1,100);

	m_Grid.SetRedraw(FALSE);
	item.mask = GVIF_TEXT|GVIF_FORMAT;
	item.nFormat = DT_CENTER|DT_VCENTER|DT_SINGLELINE;
	item.col = 0;
	CString		str;
	for (item.row=1;item.row<COMMON_SETTING_NUM;item.row++)
	{
		m_Grid.SetRowHeight(item.row,40);
		str.LoadString(IDS_STRING_BATLOWTHD+item.row-1);
		item.szText = str;
		m_Grid.SetItem(&item);
	}
	item.col = 1;
	item.row = 0;
	m_Grid.SetRowHeight(item.row,30);
	str.LoadString(IDS_STRING_TITLE);
	item.szText = str;
	m_Grid.SetItem(&item);
	m_Grid.SetRedraw(TRUE,TRUE);//�����������

}
UINT	CCommonSetDlg::GetCommonInfo(int nFlag)
{
	UINT	nRet = m_pData[0x00+nFlag]+m_pData[0x00+nFlag+1]*0xff;
	return nRet;
}
void	CCommonSetDlg::SetCommonInfo(int nFlag,int nVal)
{
	m_pData[0x00+nFlag] = nVal%0xff;
	m_pData[0x00+nFlag+1] = nVal/0xff;
}
BYTE*	CCommonSetDlg::GetCommonByte()
{
	return m_pData;
}
BOOL	CCommonSetDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	
	// TODO:  �ڴ���Ӷ���ĳ�ʼ��
	m_Grid.SetEditable(FALSE);
	SetGlobalVal();
	LoadData();
	return TRUE;  // return TRUE unless you set the focus to a control
	// �쳣: OCX ����ҳӦ���� FALSE
}

void	CCommonSetDlg::OnShowWindow(BOOL bShow, UINT nStatus)
{
	CDialog::OnShowWindow(bShow, nStatus);
	LoadData();
	// TODO: �ڴ˴������Ϣ����������
}
void	CCommonSetDlg::OnClickGrid(NMHDR * pNMHDR, LRESULT* pResult)
{
	GV_DISPINFO* pInfo = (GV_DISPINFO*)pNMHDR;
	int	nRow = pInfo->item.row;
	m_nCurrentRow = nRow;
	ShowValuesOnCtrls();
	CRect	rect(pInfo->rect);
	ScreenToClient(&rect);
	if (m_Grid.IsEditable())
	{
		m_editGrid.Initialize(CGridEdit::Freq,4,pInfo->item.szText,&rect);
	}

}
//��ʾ�ؼ��ϵĸ���ֵ
void	CCommonSetDlg::ShowValuesOnCtrls()
{
	//ǰ4����Ϊ1023�����һ��127
	UINT	nValue = GetCommonInfo(COMMON_BATLOWTHRD+2*(m_nCurrentRow-1));
	UINT	nPos = 0;
	CString		str;
	if (m_nCurrentRow==m_Grid.GetRowCount()-1)
	{
		nPos = MIN_MAX_VALUE;
	}
	else
		nPos = MIDDLE_MAX_VALUE;
	m_ScrBar.SetScrollRange(0,nPos);

	m_ScrBar.SetScrollPos(nValue);
	m_EditValue.SetInteger(nValue);
	//����Ҫ��grid����ʾ
	int i=1;
	for (;i<m_Grid.GetRowCount();i++)
	{
		nValue = GetCommonInfo(COMMON_BATLOWTHRD+2*(i-1));
		str.Format("%d",nValue);
		m_Grid.SetItemText(i,1,str.GetBuffer());
		str.ReleaseBuffer();
	}
}
void	CCommonSetDlg::OnDeltaposSpinEditValue(NMHDR* pNMHDR, LRESULT* pResult)
{
	NM_UPDOWN* pNMUpDown = (NM_UPDOWN*)pNMHDR;
	int		nVal;
	if (m_nCurrentRow>0 && m_nCurrentRow<m_Grid.GetRowCount())
	{
		nVal = m_EditValue.DeltaInteger(-pNMUpDown->iDelta);
		m_ScrBar.SetScrollPos(nVal);
		SetCommonInfo(COMMON_BATLOWTHRD+(m_nCurrentRow-1)*2,nVal);
	}
	*pResult = 0;
}
//����ÿһ�εĵ���������ӣ�ͨ����Ͽ��Ķ���
void	CCommonSetDlg::OnBnClickedOk()//����Ӧ����������ʱ��
{
	//�����Ӵ��ڣ�����ͨ��
	if(!ConnectToRadio())
		return;
	m_nTimerID = SetTimer(1,100,NULL);
	m_bSend = TRUE;
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	((CButton*)GetDlgItem(IDOK))->EnableWindow(FALSE);
	((CButton*)GetDlgItem(IDCANCEL))->EnableWindow(TRUE);
}
void	CCommonSetDlg::OnBnClickedStop()
{
	//�Ͽ�����
	KillTimer(m_nTimerID);
	UnconnectToRadio();
	((CButton*)GetDlgItem(IDOK))->EnableWindow(TRUE);
	((CButton*)GetDlgItem(IDCANCEL))->EnableWindow(FALSE);
	SetCommonInfo(COMMON_BATLOWTHRD+(m_nCurrentRow-1)*2,m_nVal);
// 	ShowValuesOnCtrls();
}
void	CCommonSetDlg::OnBnClickedEdit()
{
	m_Grid.SetEditable(TRUE);
	((CButton*)GetDlgItem(IDC_BUTTON_EDIT))->EnableWindow(FALSE);
	((CButton*)GetDlgItem(IDC_BUTTON_SURE))->EnableWindow(TRUE);
}
void	CCommonSetDlg::OnBnClickedSure()
{
	m_Grid.SetEditable(FALSE);
	((CButton*)GetDlgItem(IDC_BUTTON_EDIT))->EnableWindow(TRUE);
	((CButton*)GetDlgItem(IDC_BUTTON_SURE))->EnableWindow(FALSE);
	ShowValuesOnCtrls();
}
void	CCommonSetDlg::OnEnKillfocusEditGrid()
{
	CString		str;
	m_editGrid.GetWindowText(str);
	int		nVal;
	if(str.IsEmpty())
		nVal = 0;
	else
	{
		nVal = _ttoi(str.GetBuffer());
		str.ReleaseBuffer();
	}
	SetCommonInfo(COMMON_BATLOWTHRD+2*(m_nCurrentRow-1),nVal);
	LoadData();
	m_editGrid.HideWindow();
}
void	CCommonSetDlg::SendDataToPort(void* lpBuff,int nLen,int	comVal)
{
	if(nLen<=0)
		return;
	int	nRecChar;
	int	nVal = 0;
	switch (comVal)
	{//���ݽӿ��ĵ�,У׼��ʽ�з��� 
	case COMMON_BATLOWTHRD://MT_ SP_ADCAL _REQ...SP_MT_ADCAL_IND--4556-4557(11CC-11CD)
	case COMMON_VOX1OPEN:
	case COMMON_VOX5OPEN:
		{
			BYTE*	szBuff = new BYTE[nLen];
			ZeroMemory(szBuff,nLen);
			memcpy(szBuff,lpBuff,nLen);
			nRecChar = ((CMTSWDlg*)((CTabCtrl*)m_pOwner->GetOwner()))->WriteCommData(szBuff,nLen);
			if (nRecChar == 22)
			{
				nRecChar = ((CMTSWDlg*)((CTabCtrl*)m_pOwner->GetOwner()))->ReadCommData(szBuff,22);
				if (nRecChar == 22)
				{
					if (szBuff[14] == 0x03 || szBuff[14] == 0x04)//vox|bat_low
					{
						nVal = szBuff[16]+szBuff[17]*256;
						SetEditScrollVal(nVal);
					}
				}
			}
				
		}
		break;
	case COMMON_VCCN://MT_SCH_ANA_FREQCAL_REQ    MT_SCH_ANA_FREQCAL_STOP 
		{
			BYTE*	szBuff = new BYTE[nLen];
			ZeroMemory(szBuff,nLen);
			memcpy(szBuff,lpBuff,nLen);
			nRecChar = ((CMTSWDlg*)((CTabCtrl*)m_pOwner->GetOwner()))->WriteCommData(szBuff,nLen);
			if (nRecChar ==24)
			{
				nVal = szBuff[18]+szBuff[19]*256;
				SetEditScrollVal(nVal);
				memcpy(szBuff,&g_MtSchFreqStop,sizeof(g_MtSchFreqStop));
				Sleep(100);
				nRecChar = ((CMTSWDlg*)((CTabCtrl*)m_pOwner->GetOwner()))->WriteCommData(szBuff,18);
			}
		}
		break;
	case COMMON_MAXVOL://MT_SCH_ANA _RXCAL_ REQ=>MT_SP _MAXVOL _REQ=>MT_ SCH_ANA_ RXCAL_STOP
		{
			BYTE*	szBuff = new BYTE[sizeof(g_SpSchRxCal)];
			ZeroMemory(szBuff,sizeof(g_SpSchRxCal));
 			memcpy(szBuff,&g_SpSchRxCal,sizeof(g_SpSchRxCal));
			((CMTSWDlg*)((CTabCtrl*)m_pOwner->GetOwner()))->WriteCommData(szBuff,sizeof(g_SpSchRxCal));
			Sleep(100);
			memcpy(szBuff,lpBuff,nLen);
			((CMTSWDlg*)((CTabCtrl*)m_pOwner->GetOwner()))->WriteCommData(szBuff,nLen);
			nVal = szBuff[18]+szBuff[19]*256;
			SetEditScrollVal(nVal);
			Sleep(100);
			memcpy(szBuff,&g_MtSchRxCalStop,sizeof(g_MtSchRxCalStop));
			((CMTSWDlg*)((CTabCtrl*)m_pOwner->GetOwner()))->WriteCommData(szBuff,sizeof(g_MtSchRxCalStop));
			Sleep(100);
		}
		break;
	}
}
void	CCommonSetDlg::SetEditScrollVal(int nVal)
{
	if (m_nCurrentRow>0&& m_nCurrentRow<m_Grid.GetRowCount()-1)
	{
		m_ScrBar.SetScrollRange(0,MIDDLE_MAX_VALUE);
		if (nVal>MIDDLE_MAX_VALUE)
		{
			return;
		}
	}
	else
	{
		m_ScrBar.SetScrollRange(0,MIN_MAX_VALUE);
		if(nVal>MIN_MAX_VALUE)
			return;
	}
	m_ScrBar.SetScrollPos(nVal);
	m_EditValue.SetInteger(nVal);
	m_nVal = nVal;
}
void	CCommonSetDlg::OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar)
{
	// TODO: �ڴ������Ϣ�����������/�����Ĭ��ֵ
	int	nPos1 = pScrollBar->GetScrollPos();
	int	nMin,nMax;
	pScrollBar->GetScrollRange(&nMin,&nMax);
	int	nTmp = 1;
	switch(nSBCode)
	{
	case SB_LINELEFT:
		if (nMax>=0 && nMin>=0)
		{
			if (nPos1 - nTmp >nMin)
			{
				nPos1 -= nTmp;
			}
			else
				nPos1 = nMin;
		}
		break;
	case SB_LINERIGHT:
		if (nMax>=0 && nMin>=0)
		{
			if (nPos1 + nTmp >nMax)
			{
				nPos1 = nMax;
			}
			else
				nPos1 += nTmp;
		}
		break;
	case SB_THUMBPOSITION:
		nPos1 = nPos;
		break;
	default:
		break;
	}
	pScrollBar->SetScrollPos(nPos1);
	m_EditValue.SetInteger(nPos1);
	SetCommonInfo(COMMON_BATLOWTHRD+(m_nCurrentRow-1)*2,nPos1);
}

void	CCommonSetDlg::OnTimer(UINT nIDEvent)
{
	if (m_bSend)
	{
		int	nVal = m_ScrBar.GetScrollPos();
		if (m_nCurrentRow==m_Grid.GetRowCount()-1)//��������
		{	
			g_SpMtMaxvol.SpMtMaxvol.volgain = nVal;
			SendDataToPort((void*)&g_SpMtMaxvol,sizeof(g_SpMtMaxvol),COMMON_MAXVOL);
		}
		else if (m_nCurrentRow == IDS_STRING_VOX1OPEN-IDS_STRING_BATLOWTHD+1 ||
			m_nCurrentRow == IDS_STRING_VOX5OPEN-IDS_STRING_BATLOWTHD+1)//VOX
		{
			g_SpMtAd.SpMtAd.checkitem = 0x04;	//VOX
			SendDataToPort((void*)&g_SpMtAd,sizeof(g_SpMtAd),COMMON_VOX1OPEN);
		}
		else if (m_nCurrentRow == IDS_STRING_BATLOWTHD - IDS_STRING_BATLOWTHD+1)
		{
			g_SpMtAd.SpMtAd.checkitem = 0x03;	//bat_low_thrd
			SendDataToPort((void*)&g_SpMtAd,sizeof(g_SpMtAd),COMMON_BATLOWTHRD);
		}
		else			//VCCN 
		{
			g_MtSchFreqCal.VCCN = nVal;
			g_MtSchFreqCal.txananum = ((CMTSWDlg*)((CTabCtrl*)m_pOwner->GetOwner()))->GetTxFreq();
			SendDataToPort((void*)&g_MtSchFreqCal,sizeof(g_MtSchFreqCal),COMMON_VCCN);
		}
	}
}
//����У׼ģʽ��ÿ�ε��ú��Ҫ�˳�������һ��
BOOL	CCommonSetDlg::ConnectToRadio()
{
	if (!((CMTSWDlg*)((CTabCtrl*)m_pOwner->GetOwner()))->IsCommOpen())
	{
		((CMTSWDlg*)((CTabCtrl*)m_pOwner->GetOwner()))->OpenComm();
	} 
	return ((CMTSWDlg*)((CTabCtrl*)m_pOwner->GetOwner()))->ConnectRadio();

// 	BYTE*	szBuff = new BYTE[0x100];
// 	int	nRecChar;
// 	ZeroMemory(szBuff,0x100);
// 
// 	memcpy(szBuff,&g_SpMtConn,sizeof(SpMtConnectReq));
// 	// mt_sp_connect_req
// 	nRecChar = ((CMTSWDlg*)((CTabCtrl*)m_pOwner->GetOwner()))->WriteCommData(szBuff,sizeof(SpMtConnectReq));
// 	if (nRecChar == 22)
// 	{//sp_mt_connect_cnf
// 		nRecChar = ((CMTSWDlg*)((CTabCtrl*)m_pOwner->GetOwner()))->ReadCommData(szBuff,22);
// 		if (nRecChar == 22 && szBuff[14]== 0x01)
// 		{
// 			return TRUE;
// 		}
// 		return FALSE;
// 	}
// 	return FALSE;
}
BOOL	CCommonSetDlg::UnconnectToRadio()
{
// 	//MT_SP_EFFECT_REQ
// 	BYTE*	szBuff = new BYTE[0x100];
// 	int	nRecChar;
// 	ZeroMemory(szBuff,0x100);
// 	memcpy(szBuff,&g_SpMtEffect,sizeof(SpMtConnectReq));
// 	nRecChar = ((CMTSWDlg*)((CTabCtrl*)m_pOwner->GetOwner()))->WriteCommData(szBuff,sizeof(SpMtConnectReq));
// 	if (nRecChar == 22)
// 	{//sp_mt_connect_cnf
// 		nRecChar = ((CMTSWDlg*)((CTabCtrl*)m_pOwner->GetOwner()))->ReadCommData(szBuff,22);
// 		if (nRecChar == 22 && szBuff[14]== 0x01)
// 		{
// 			return TRUE;
// 		}
// 		return FALSE;
// 	}
// 	return FALSE;
	return ((CMTSWDlg*)((CTabCtrl*)m_pOwner->GetOwner()))->UnconnectRadio();

}