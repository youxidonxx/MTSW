// DigitSetDlg.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "MTSW.h"
#include "MTSWDlg.h"
#include "DigitSetDlg.h"


// CDigitSetDlg �Ի���

IMPLEMENT_DYNAMIC(CDigitSetDlg, CDialog)

CDigitSetDlg::CDigitSetDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CDigitSetDlg::IDD, pParent)
{
	m_pParentWnd = pParent;
	m_pData = new BYTE[0x200];
	m_nFreqCount = 2;
	m_bEditRowCnt = FALSE;
	m_bSend = FALSE;
	m_nTimerID = -1;
	m_nInterval = 1;
	m_nCTCSSSigNum = 1;
	m_nCDCSSSigNum = 50;
}

CDigitSetDlg::~CDigitSetDlg()
{
}

void CDigitSetDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_EDIT_FREQ_CNT, m_editFreqCnt);
	DDX_Control(pDX, IDC_EDIT_GRID, m_editGrid);
	DDX_Control(pDX, IDC_SCROLLBAR, m_ScrollBar);
	DDX_Control(pDX, IDC_EDIT_VALUE, m_editVal);
	DDX_GridControl(pDX, IDC_CUSTOM_GRID, m_Grid);
}


BEGIN_MESSAGE_MAP(CDigitSetDlg, CDialog)
	ON_WM_SHOWWINDOW()
	ON_NOTIFY(GVN_BEGINLABELEDIT, IDC_CUSTOM_GRID, OnClickGrid)
	ON_BN_CLICKED(IDC_BUTTON_EDIT_ROWCNT, &CDigitSetDlg::OnBnClickedButtonEditRowcnt)
	ON_BN_CLICKED(IDOK, &CDigitSetDlg::OnBnClickedOk)
	ON_BN_CLICKED(IDCANCEL, &CDigitSetDlg::OnBnClickedCancel)
	ON_EN_KILLFOCUS(IDC_EDIT_GRID,&CDigitSetDlg::OnKillFocusEditGrid)
	ON_EN_KILLFOCUS(IDC_EDIT_FREQ_CNT,&CDigitSetDlg::OnKillFocusEditFreqCnt)
	ON_WM_HSCROLL()
	ON_WM_TIMER()
END_MESSAGE_MAP()


// CDigitSetDlg ��Ϣ�������
void	CDigitSetDlg::LoadData()
{
	m_pOwner = GetOwner();
	m_pData = ((CMTSWDlg*)((CTabCtrl*)m_pOwner->GetOwner()))->GetDigitDataPtr();
	SetFreqNum();
	SetGridInfo();
	SetCtrlVals();
}
//�������2��Ƶ������ƫ�ƴ�С
void	CDigitSetDlg::SetDigitInfo(int nFlag,int nVal)
{
	m_pData[0x00+2+nFlag] = nVal%256;
	m_pData[0x00+2+nFlag+1] = nVal/256;
}
WORD	CDigitSetDlg::GetDigitInfo(int nFlag)
{
	WORD	nRet = m_pData[0x00+2+nFlag]+m_pData[0x00+2+nFlag+1]*256;
	return nRet;
}
//����10����������д���ڴ���
void	CDigitSetDlg::SetDigitFreq(int nFlag,DWORD nFreq)
{
	CString		str;
	char	ch=0,data;
	BYTE*	ptr;
	str.Format("%x",nFreq);
	int	nLen = str.GetLength();
	ptr = (m_pData+2+nFlag);
	ZeroMemory(ptr,FREQ_NUM_LEN);
	if (nLen>0)
	{	//�ӵ�λ����λ��ֵ�ڴ�
		int i=0;
		for (i=0;i<nLen;i++)
		{
			ch = str.GetAt(nLen - i-1);
			if(ch>='0'&& ch<='9')
				data = ch-'0';
			else if(ch>='a' && ch<='f')
				data = ch - 'a'+10;
			else
				data = 0;
			if (i%2 == 0)
			{
				ptr[i/2] = (ptr[i/2]&0xf0)|(data);
			}
			else
				ptr[i/2] = (ptr[i/2]&0x0f)|(data<<4);
		}
	}
}
//�Ȼ�ȡ16���ƣ�Ȼ����ת��10����
DWORD	CDigitSetDlg::GetDigitFreq(int nFlag)
{
	BYTE*	ptr  = &m_pData[0x00+2+nFlag];
	CString		str;
	str.Format("%02X%02X%02X%02X",*(ptr+3),*(ptr+2),*(ptr+1),*ptr);
	DWORD	dwFreq;
	sscanf_s(str.GetBuffer(),"%x",&dwFreq);
	str.ReleaseBuffer();
	return dwFreq;
}
BOOL	CDigitSetDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	// TODO:  �ڴ���Ӷ���ĳ�ʼ��
	m_Grid.GetClientRect(&m_RectOri);
	m_Grid.SetEditable(TRUE);
	m_editFreqCnt.EnableWindow(FALSE);
	LoadData();
 	return TRUE;  // return TRUE unless you set the focus to a control
	// �쳣: OCX ����ҳӦ���� FALSE
}
void	CDigitSetDlg::SetFreqNum()
{
	if(!m_pData)
		return;
	m_nFreqCount = m_pData[0x00]+m_pData[0x01]*0xff;
	CString		str;
	str.Format("%d",m_nFreqCount);
	m_editFreqCnt.SetWindowText(str);
}
int		CDigitSetDlg::GetFreqNum()
{
	CString		str;
	m_editFreqCnt.GetWindowText(str);
	int	nVal;
	if(!str.IsEmpty())
		nVal = _ttoi(str.GetBuffer());
	else
		nVal = 2;
	str.ReleaseBuffer();
	return nVal;
}
void	CDigitSetDlg::OnShowWindow(BOOL bShow, UINT nStatus)
{
	CDialog::OnShowWindow(bShow, nStatus);
	LoadData();
	// TODO: �ڴ˴������Ϣ����������
}
void	CDigitSetDlg::SetGridInfo()
{
	GV_ITEM		item;
	m_Grid.SetRowCount(DIGIT_ANA_SETTING_NUM+1);
	m_Grid.SetColumnCount(m_nFreqCount+1);
	m_Grid.SetFixedColumnCount(1);
	m_Grid.SetFixedRowCount(1);

	CString		str;
	m_Grid.EnableSelection(FALSE);
	m_Grid.SetRedraw(FALSE);
	item.mask = GVIF_TEXT|GVIF_FORMAT;
	item.nFormat = DT_CENTER|DT_VCENTER|DT_SINGLELINE;

	item.row = 0;
	for(item.col=1;item.col<m_nFreqCount+1;item.col++)
	{
		m_Grid.SetColumnWidth(item.col,GRID_COLUMN_WIDTH/2);
		str.Format("Ƶ��%d",item.col);
		item.szText = str;
		m_Grid.SetItem(&item);
	}
 
	item.col = 0;
	for (item.row=1;item.row<DIGIT_ANA_SETTING_NUM+1;item.row++)
	{
// 		m_Grid.SetRowHeight(item.row,80);
// 		str.LoadString(IDS_STRING_BATLOWTHD+item.row-1);
		str.LoadString(IDS_STRING_CALTXFREQ+item.row-1);
		item.szText = str;
		m_Grid.SetItem(&item);
	}
	item.col = 0;
	item.row = 0;
	m_Grid.SetRowHeight(item.row,30);
	m_Grid.SetColumnWidth(item.col,GRID_COLUMN_WIDTH);//*7/4);
	item.szText = _T("");
	m_Grid.SetItem(&item);
// 	m_Grid.ExpandRowsToFit();
	m_Grid.ExpandColumnsToFit();
	m_Grid.SetRedraw(TRUE,TRUE);//�����������
}
void	CDigitSetDlg::SetCtrlVals()
{
	//��ȷ�ϵ�ǰ���ڽ����grid���ֵ
	CString		str;
	int		nVal;
	int i=0,j=0;
	for (i=0;i<m_nFreqCount;i++)
	{
		for (j=1;j<3;j++)
		{
			nVal = GetDigitFreq(DIGIT_CAL_TXFREQ+(j-1)*4+i*STRUCT_LEN_CALCTCCHAL);
			str.Format("%d",nVal);
			m_Grid.SetItemText(j,i+1,str.GetBuffer());
			str.ReleaseBuffer();
		}
		for (j=3;j<DIGIT_ANA_SETTING_NUM+1;j++)
		{
			nVal = GetDigitInfo(DIGIT_CAL_TXHIGHPC+2*(j-3)+(i)*STRUCT_LEN_CALCTCCHAL);
			str.Format("%d",nVal);
			m_Grid.SetItemText(j,i+1,str.GetBuffer());
			str.ReleaseBuffer();
			OutputDebugInfo(j,str);
		}
	}
	OutputDebugString("=============================\n");

	if(!m_nFocusCell.IsValid())
		return;
	//��GRID�Ͻ�����ʾ
	int	nRow = m_nFocusCell.row;
	int	nCol = m_nFocusCell.col;
	if(nRow>2)
	{
		nVal= GetDigitInfo((nRow+1)*2+(nCol-1)*STRUCT_LEN_CALCTCCHAL);
		SetScrollEditVals(nVal);
		SetCStaticText(nRow);
	}

}
/************************************************************************/
/* ������ʾ������ʱ�޶���ʾ������Ӧ������ʾ
*/
/************************************************************************/
void	CDigitSetDlg::SetCStaticText(int nRow)
{
	CString		strText;
	strText.LoadString(IDS_STRING_CALTXFREQ+nRow-1);

	((CStatic*)GetDlgItem(IDC_STATIC_TITLE))->SetWindowText(strText);
}
void	CDigitSetDlg::SetScrollEditVals(int nVal)
{

	UINT	nPos = 0;
	//��ȡ��ǰ�Ľ���ֵ
	int	nRow = m_nFocusCell.row;
 	switch (nRow-1)//���ݲ�ͬ�����ݣ���ʾ��ͬ�ķ�Χ
	{
	case 2:
	case 3:	//�ߵ͹���
	case 17://������
	case 18:
	case 19:
	case 20:
	case 21://����ȼ�����
	case 22:
	case 23://RSSI����
		nPos = MIDDLE_MAX_VALUE;
		break;
	default:
		nPos = MAX_MAX_VALUE;
	}
	m_ScrollBar.SetScrollRange(0,nPos);
	m_ScrollBar.SetScrollPos(nVal);
	m_editVal.SetInteger(nVal);
}
void	CDigitSetDlg::OnClickGrid(NMHDR * pNMHDR, LRESULT* pResult)
{
	GV_DISPINFO* pInfo = (GV_DISPINFO*)pNMHDR;
	m_nFocusCell = m_Grid.GetFocusCell(); 
	SetCtrlVals();
	CRect	rect(pInfo->rect);
	ScreenToClient(&rect);
	int	nLimit = 5;
	if(m_nFocusCell.row == 1 ||m_nFocusCell.row==2)
		nLimit = 10;
	if (m_Grid.IsEditable())
	{
		m_editGrid.Initialize(CGridEdit::Freq,nLimit,pInfo->item.szText,&rect);
	}
}
/*****************************************************
���༭���֮�󣬽�������ֵ�������ڴ��У��Ա���д��ʱʹ�ã�
��Ҫ����Ƶ��������������ֶε���������
********************************************************/
void	CDigitSetDlg::OnKillFocusEditGrid()
{
	int	nRow = m_nFocusCell.row;
	int	nCol = m_nFocusCell.col;
	CString		str;
	m_editGrid.GetWindowText(str);
	switch (nRow)
	{
	case 0:
	case 1://Ƶ��
		StoreFreq(nRow,nCol,str);
		break;
	default:
		SetCommonDigitVal(nRow,nCol,str);
		break;
	}
	m_editGrid.HideWindow();
	LoadData();
}
/************************************************************************/
/* ��Ƶ������������޸�
*/
/************************************************************************/
void	CDigitSetDlg::OnKillFocusEditFreqCnt()
{
	int	nVal = GetFreqNum();
	if (nVal>=MIN_FREQ_NUM && nVal<=MAX_FREQ_NUM)
	{
		m_nFreqCount = nVal;		
	}
	else
	{
		AfxMessageBox("Ƶ��������ΧΪ2-5����ȷ��������ķ�Χ");
		m_editFreqCnt.SetFocus();
	}

}


/************************************************************************/
/* ���޸����Ƶ������֮�����б䶯�����GRID��ʾ����б䶯��������Ӧ�޸ģ���
д���ڴ���
*/
/************************************************************************/
void	CDigitSetDlg::OnBnClickedButtonEditRowcnt()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	m_bEditRowCnt = !m_bEditRowCnt;
	CString		str;
	if(!m_bEditRowCnt)
	{
		str.LoadString(IDS_STRING_EDIT);
		m_editFreqCnt.EnableWindow(FALSE);
		((CButton*)GetDlgItem(IDC_BUTTON_EDIT_ROWCNT))->SetWindowText(str);
		if (m_nFreqCount!=m_Grid.GetColumnCount()-1)
		{
			//��ʱӦ����movewindow�����ڴ�С���б仯
			ResizeGridWnd(FALSE);
			SetGridInfo();
		}
	}
	else
	{
		str.LoadString(IDS_STRING_SURE);
		m_editFreqCnt.EnableWindow(TRUE);
		((CButton*)GetDlgItem(IDC_BUTTON_EDIT_ROWCNT))->SetWindowText(str);
	}
}



/************************************************************************/
/* ����Ҫ����GRID���޸�ʱ���Դ��ڴ�С���б仯��
ͬʱҲ��Ӧ׼���ڽ��д�������ʱ�Կؼ���С���е�������δʵ��
*/
/************************************************************************/
void	CDigitSetDlg::ResizeGridWnd(BOOL bOnSize/*= FALSE*/)
{
	//�� ��¼��ԭʼ���ڴ�С
	CRect	rectOld(m_RectOri);
 	//��¼�޸ĺ��Ƶ�����뵱ǰ��Ƶ���б��ֵ
// 	int		nSurplus = m_nFreqCount - (m_Grid.GetColumnCount()-1);
	if (bOnSize)//����Ϊ���촰��ʱ�Ķ���
	{
	}
	else
	{
		m_Grid.MoveWindow(&rectOld);
	}
}
/************************************************************************/
/* ���������ʱ������֮ǰ��������꣬�������ò�������Ӧ������
*/
/************************************************************************/
void	CDigitSetDlg::OnBnClickedOk()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	if (ConnectToRadio())
	{
		int	nVal = m_ScrollBar.GetScrollPos();
		int	nRow = m_nFocusCell.row;
		int	nCol = m_nFocusCell.col;
		if (m_nFocusCell.IsValid())
		{
			switch (nRow)
			{
			case 2:
			case 3:	//�ߵ͹���
				g_MtSchAnaTxCal.txananum = GetDigitFreq((nCol-1)*STRUCT_LEN_CALCTCCHAL+DIGIT_CAL_TXFREQ);
				g_MtSchAnaTxCal.PC = nVal;
				g_MtSchAnaTxCal.Interval = m_nInterval;
				g_MtSchAnaTxCal.SigType = 0;
				g_MtSchAnaTxCal.SigNum = 0;
				g_MtSchAnaTxCal.MOD1 = 0;
				g_MtSchAnaTxCal.MOD2 = 0;
				WriteCommData(&g_MtSchAnaTxCal,sizeof(g_MtSchAnaTxCal));
				break;
			case 4:
			case 5://����Ƶƫ��ƽ��
				g_MtSchDigTxCal.txananum = GetDigitFreq((nCol-1)*STRUCT_LEN_CALCTCCHAL+DIGIT_CAL_TXFREQ);
				g_MtSchDigTxCal.PC = GetDigitInfo((nCol-1)*STRUCT_LEN_CALCTCCHAL+DIGIT_TXMOD1_FACTOR+2*(nRow-4));//��ȡ����ֵ
				g_MtSchDigTxCal.Interval = m_nInterval;
				g_MtSchDigTxCal.MOD1 = GetDigitInfo((nCol-1)*STRUCT_LEN_CALCTCCHAL+DIGIT_TXMOD1_FACTOR);
				g_MtSchDigTxCal.MOD2 = GetDigitInfo((nCol-1)*STRUCT_LEN_CALCTCCHAL+DIGIT_TXMOD2_FACTOR);
				WriteCommData(&g_MtSchDigTxCal,sizeof(g_MtSchDigTxCal));
				break;
			case 6:
			case 7://��Ƶ����
 			case 8:
			case 9:
			case 10:
			case 11:
			case 12:
			case 13://CTCSS����
				if(nRow-1==6 ||nRow-1 == 7)
				{
					m_nCTCSSSigNum = 0;		//��ƵʱΪ0
					g_MtSchAnaTxCal.SigType = 0;
				}
				else
				{
					m_nCTCSSSigNum = 1;
					g_MtSchAnaTxCal.SigType = 1;
				}
				g_MtSchAnaTxCal.txananum = GetDigitFreq((nCol-1)*STRUCT_LEN_CALCTCCHAL+DIGIT_CAL_TXFREQ);
				g_MtSchAnaTxCal.PC = GetDigitInfo((nCol-1)*STRUCT_LEN_CALCTCCHAL+DIGIT_CAL_TXLOWPC);
				g_MtSchAnaTxCal.Interval = m_nInterval;
				g_MtSchAnaTxCal.SigNum = m_nCTCSSSigNum;
				if ((nRow)%2 == 0)	//Ƶƫ
				{
					g_MtSchAnaTxCal.MOD1 = nVal;
					g_MtSchAnaTxCal.MOD2 = GetDigitInfo((nCol-1)*STRUCT_LEN_CALCTCCHAL+ANALOG_TXMOD1_FACTOR_AF+2*(nRow-6));
				}
				else
				{
					g_MtSchAnaTxCal.MOD1 = GetDigitInfo((nCol-1)*STRUCT_LEN_CALCTCCHAL+ANALOG_TXMOD1_FACTOR_AF+2*(nRow-6));
					g_MtSchAnaTxCal.MOD2 = nVal;
				}
				WriteCommData(&g_MtSchAnaTxCal,sizeof(g_MtSchAnaTxCal));
				break;
			case 14:
			case 15://CDCSS
				g_MtSchAnaTxCal.txananum = GetDigitFreq((nCol-1)*STRUCT_LEN_CALCTCCHAL+DIGIT_CAL_TXFREQ);
				g_MtSchAnaTxCal.PC = GetDigitInfo((nCol-1)*STRUCT_LEN_CALCTCCHAL+DIGIT_CAL_TXLOWPC);
				g_MtSchAnaTxCal.Interval = m_nInterval;
				g_MtSchAnaTxCal.SigType = 2;
				g_MtSchAnaTxCal.SigNum = m_nCDCSSSigNum;
				if ((nRow)%2 == 0)	//Ƶƫ
				{
					g_MtSchAnaTxCal.MOD1 = nVal;
					g_MtSchAnaTxCal.MOD2 = GetDigitInfo((nCol-1)*STRUCT_LEN_CALCTCCHAL+ANALOG_TXMOD1_FACTOR_CTC0+2*(nRow-8));
				}
				else
				{
					g_MtSchAnaTxCal.MOD1 = GetDigitInfo((nCol-1)*STRUCT_LEN_CALCTCCHAL+ANALOG_TXMOD1_FACTOR_CTC0+2*(nRow-8));
					g_MtSchAnaTxCal.MOD2 = nVal;
				}
				WriteCommData(&g_MtSchAnaTxCal,sizeof(g_MtSchAnaTxCal));
				break;
			case 16://������
				g_SpSchRxCal.TV = nVal;
				g_SpSchRxCal.txananum = GetDigitFreq((nCol-1)*STRUCT_LEN_CALCTCCHAL+DIGIT_CAL_RXFREQ);
				WriteCommData(&g_SpSchRxCal,sizeof(g_SpSchRxCal));
				break;
			case 17:
			case 18:
			case 19:
			case 20://����ȼ�����
				g_SpSchRxCal.TV = nVal;
				g_SpSchRxCal.txananum = GetDigitFreq((nCol-1)*STRUCT_LEN_CALCTCCHAL+DIGIT_CAL_RXFREQ);
				WriteCommData(&g_SpSchRxCal,sizeof(g_SpSchRxCal));
				m_nTimerID = SetTimer(1,100,NULL);
				break;
			case 21:
			case 22://RSSI����
				g_SpSchRxCal.TV = nVal;
				g_SpSchRxCal.txananum = GetDigitFreq((nCol-1)*STRUCT_LEN_CALCTCCHAL+DIGIT_CAL_RXFREQ);
				WriteCommData(&g_SpSchRxCal,sizeof(g_SpSchRxCal));
				m_nTimerID = SetTimer(1,100,NULL);
				break;
			default:
				break;
			}
		}
	}
}
/************************************************************************/
/* ������Ϻ󣬶�ʱ��ֹͣ��������ǰֵд���ڴ��У�׼����һ��д���նˣ�
�Թ�������EDIT���е���ֵΪ��׼����д��
*/
/************************************************************************/
void CDigitSetDlg::OnBnClickedCancel()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	if(m_nTimerID!=-1)
		KillTimer(m_nTimerID);
	int	nRow = m_nFocusCell.row;
	int	nCol = m_nFocusCell.col;
	int	nMsgID = 0;
	switch (nRow-1)
	{
	case 4:
	case 5://����Ƶƫ��ƽ��
		nMsgID = MT_SCH_DIG_TXCAL_STOP;
		break;
	case 2:
	case 3:	//�ߵ͹���
 	case 6:
	case 7://��Ƶ����
	case 8:
	case 9:
	case 10:
	case 11:
	case 12:
	case 13://CTCSS����
	case 14:
	case 15://CDCSS
		nMsgID = MT_SCH_ANA_TXCAL_STOP;
 		break;
	case 16://������
		nMsgID = MT_SCH_ANA_RXCAL_STOP;
		break;
	default:
		nMsgID = -1;
		break;
	}
	if (nMsgID!=-1)
	{
		g_MtSchRxCalStop.MsgID = nMsgID;
		WriteCommData(&g_MtSchRxCalStop,sizeof(g_MtSchRxCalStop));
	}
	UnconnectToRadio();
 }
/************************************************************************/
/* ������������仯ʱ����Ӧ��EDIT��Ҳ������ֵ��ʾ
*/
/************************************************************************/
void CDigitSetDlg::OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar)
{
	// TODO: �ڴ������Ϣ�����������/�����Ĭ��ֵ
	int	nCtrlID = pScrollBar->GetDlgCtrlID();
	if(nCtrlID == m_ScrollBar.GetDlgCtrlID())
	{
		int	nPos1 = pScrollBar->GetScrollPos();
		int	nMax,nMin;
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
		m_editVal.SetInteger(nPos1);
	}
	else
		CDialog::OnHScroll(nSBCode, nPos, pScrollBar);
}
/************************************************************************/
/*                                                                      */
/************************************************************************/
void CDigitSetDlg::OnTimer(UINT_PTR nIDEvent)
{
	// TODO: �ڴ������Ϣ�����������/�����Ĭ��ֵ
	if (m_bSend)
	{
		int	nVal = m_ScrollBar.GetScrollPos();
		if (m_nFocusCell.IsValid())
		{
			switch (m_nFocusCell.col)
			{
			case 2:
			case 3:	//�ߵ͹���
				break;
			case 17://������
				break;
			case 18:
			case 19:
			case 20:
			case 21://����ȼ�����
				g_SpMtAd.SpMtAd.checkitem = 0x01;
				SendDataToPort((void*)&g_SpMtAd,sizeof(g_SpMtAd),RX_NOISE_1_OPEN+m_nFocusCell.col-17);
				break;
			case 22:
			case 23://RSSI����
// 				g_SpSchRxCal.TV = nVal;
// 				g_SpSchRxCal.txananum = GetDigitFreq((m_nFocusCell.row-1)*STRUCT_LEN_CALCTCCHAL+DIGIT_CAL_RXFREQ);
				g_SpMtAd.SpMtAd.checkitem = 0x02;
				SendDataToPort((void*)&g_SpMtAd,sizeof(g_SpMtAd),RX_RSSI_1_THRD+m_nFocusCell.col-22);
				break;
			default:
				break;
			}
		}
	}
	CDialog::OnTimer(nIDEvent);
}
void	CDigitSetDlg::SendDataToPort(void* lpBuff,int nLen,int comVal)
{
	if(nLen<=0)
		return;
	int	nRecChar;
	int	nVal = 0;
	BYTE*	szBuff = new BYTE[nLen];
	ZeroMemory(szBuff,nLen);
	switch (comVal)
	{
	case RX_RSSI_1_THRD:
	case RX_RSSI_5_THRD:
		nRecChar = WriteCommData(lpBuff,nLen);
		if (nRecChar == 22)
		{
			nRecChar = ReadCommData(szBuff,22);	//sp_mt_adcal_ind
			if (szBuff[16] == 0x02 && szBuff[17]==0x00)	//checkitem == 2����rssi
			{
				nVal =  szBuff[18]+szBuff[19]*256;
				SetScrollEditVals(nVal);
			}
		}
		break;
	case RX_NOISE_1_OPEN:
	case RX_NOISE_1_CLOSE:
	case RX_NOISE_9_OPEN:
	case RX_NOISE_9_CLOSE:
		nRecChar = WriteCommData(lpBuff,nLen);
		if (nRecChar == 22)
		{
			nRecChar = ReadCommData(szBuff,22);	//sp_mt_adcal_ind
			if (szBuff[16] == 0x01 && szBuff[17]==0x00)	//checkitem == 2����rssi
			{
				nVal =  szBuff[18]+szBuff[19]*256;
				SetScrollEditVals(nVal);
			}
		}
		break;
	}
}
BOOL	CDigitSetDlg::ConnectToRadio()
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
BOOL	CDigitSetDlg::UnconnectToRadio()
{
	return ((CMTSWDlg*)((CTabCtrl*)m_pOwner->GetOwner()))->UnconnectRadio();
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
}
DWORD	CDigitSetDlg::WriteCommData(void* lpBuff,int nLen)
{
	if(nLen<=0)
		return ERR_DATA_LEN_INVALID;
	BYTE*	szBuff = new BYTE[nLen];
	ZeroMemory(szBuff,nLen);
	memcpy(szBuff,lpBuff,nLen);
	DWORD	nRecChar = ((CMTSWDlg*)((CTabCtrl*)m_pOwner->GetOwner()))->WriteCommData(szBuff,nLen);
	return nRecChar;
}
DWORD	CDigitSetDlg::ReadCommData(BYTE* lpBuff,int nLen)
{
	if(nLen<=0)
		return ERR_DATA_LEN_INVALID;
	ZeroMemory(lpBuff,nLen);
	int		nRecChar = ((CMTSWDlg*)((CTabCtrl*)m_pOwner->GetOwner()))->ReadCommData(lpBuff,nLen);
	return nRecChar;
}
void	CDigitSetDlg::StoreFreq(int nRow,int nCol,CString& strFreq)
{
	//����Ϊ�գ���ָ�ԭʼ��Ƶ��
	if (strFreq.IsEmpty())
		return;
	strFreq.Trim();
	if(!JudgeFreqStr(strFreq))
	{
		m_Grid.SetFocusCell(nRow,nCol);
		return;
	}
	//��ȡƵ���ַ�������
	DWORD	dwFreq = _ttol(strFreq.GetBuffer());
	strFreq.ReleaseBuffer();

	SetDigitFreq(DIGIT_CAL_TXFREQ+(nRow-1)*FREQ_NUM_LEN+(nCol-1)*STRUCT_LEN_CALCTCCHAL,dwFreq);
}
BOOL	CDigitSetDlg::JudgeFreqStr(CString& strFreq)
{
	//���ж��Ƿ����С���㣬������ɾ��
	TCHAR	chDot = '.';
	int	nPos = strFreq.Find(chDot);
	if (nPos>=0)
	{
		strFreq.Delete(nPos);
	}
	int	i=0;
	int	nLen = strFreq.GetLength();
	//�����ж��Ƿ������֣������ǣ�������ʾ������
	for (;i<nLen;i++)
	{
		chDot = strFreq.GetAt(i);
		if (!isdigit(chDot))
		{
			AfxMessageBox(IDS_STRING_INVALID_NUM);
			return FALSE;
		}
	}
	//Ȼ���жϳ����Ƿ�����
	if (nLen>MAX_FREQ_NUM_LEN)
	{//���ȹ���ʱ����ȡǰ��9λ
		strFreq = strFreq.Left(MAX_FREQ_NUM_LEN);
	}
	else
	{//����Ϊ����в���
		for (i=nLen;i<MAX_FREQ_NUM_LEN;i++)
		{
			strFreq.Insert(i,'0');
		}
	}
	return TRUE;
}
void	CDigitSetDlg::SetCommonDigitVal(int nRow,int nCol,CString strVal)
{
	int	nVal = _ttol(strVal.GetBuffer());
	strVal.ReleaseBuffer();

	SetDigitInfo((nRow+1)*2+(nCol-1)*STRUCT_LEN_CALCTCCHAL,nVal);
}