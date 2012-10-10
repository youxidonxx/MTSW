// DigitSetDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "MTSW.h"
#include "MTSWDlg.h"
#include "DigitSetDlg.h"


// CDigitSetDlg 对话框

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


// CDigitSetDlg 消息处理程序
void	CDigitSetDlg::LoadData()
{
	m_pOwner = GetOwner();
	m_pData = ((CMTSWDlg*)((CTabCtrl*)m_pOwner->GetOwner()))->GetDigitDataPtr();
	SetFreqNum();
	SetGridInfo();
	SetCtrlVals();
}
//多出来的2是频率组数偏移大小
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
//传入10进制数，再写入内存中
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
	{	//从低位往高位赋值内存
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
//先获取16进制，然后再转成10进制
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

	// TODO:  在此添加额外的初始化
	m_Grid.GetClientRect(&m_RectOri);
	m_Grid.SetEditable(TRUE);
	m_editFreqCnt.EnableWindow(FALSE);
	LoadData();
 	return TRUE;  // return TRUE unless you set the focus to a control
	// 异常: OCX 属性页应返回 FALSE
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
	// TODO: 在此处添加消息处理程序代码
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
		str.Format("频点%d",item.col);
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
	m_Grid.SetRedraw(TRUE,TRUE);//令滚动条出现
}
void	CDigitSetDlg::SetCtrlVals()
{
	//先确认当前所在焦点的grid框的值
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
	//在GRID上进行显示
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
/* 标题显示，但暂时无对提示的自适应长度显示
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
	//获取当前的焦点值
	int	nRow = m_nFocusCell.row;
 	switch (nRow-1)//根据不同的内容，显示不同的范围
	{
	case 2:
	case 3:	//高低功率
	case 17://灵敏度
	case 18:
	case 19:
	case 20:
	case 21://静噪等级门限
	case 22:
	case 23://RSSI门限
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
当编辑完毕之后，将输入数值保存入内存中，以备在写入时使用，
主要区分频点的输入与其他字段的内容输入
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
	case 1://频率
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
/* 对频点的组数进行修改
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
		AfxMessageBox("频率组数范围为2-5，请确认所输入的范围");
		m_editFreqCnt.SetFocus();
	}

}


/************************************************************************/
/* 在修改完毕频率组数之后，若有变动，则对GRID显示框进行变动，做出相应修改，并
写入内存中
*/
/************************************************************************/
void	CDigitSetDlg::OnBnClickedButtonEditRowcnt()
{
	// TODO: 在此添加控件通知处理程序代码
	m_bEditRowCnt = !m_bEditRowCnt;
	CString		str;
	if(!m_bEditRowCnt)
	{
		str.LoadString(IDS_STRING_EDIT);
		m_editFreqCnt.EnableWindow(FALSE);
		((CButton*)GetDlgItem(IDC_BUTTON_EDIT_ROWCNT))->SetWindowText(str);
		if (m_nFreqCount!=m_Grid.GetColumnCount()-1)
		{
			//此时应该先movewindow将窗口大小进行变化
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
/* 当需要作出GRID框修改时，对窗口大小进行变化，
同时也相应准备在进行窗口拉伸时对控件大小进行调整——未实现
*/
/************************************************************************/
void	CDigitSetDlg::ResizeGridWnd(BOOL bOnSize/*= FALSE*/)
{
	//先 记录下原始窗口大小
	CRect	rectOld(m_RectOri);
 	//记录修改后的频点数与当前的频点列表差值
// 	int		nSurplus = m_nFreqCount - (m_Grid.GetColumnCount()-1);
	if (bOnSize)//若是为拉伸窗口时的动作
	{
	}
	else
	{
		m_Grid.MoveWindow(&rectOld);
	}
}
/************************************************************************/
/* 当点击发送时，根据之前鼠标点击坐标，进行配置并进入相应的流程
*/
/************************************************************************/
void	CDigitSetDlg::OnBnClickedOk()
{
	// TODO: 在此添加控件通知处理程序代码
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
			case 3:	//高低功率
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
			case 5://数字频偏、平衡
				g_MtSchDigTxCal.txananum = GetDigitFreq((nCol-1)*STRUCT_LEN_CALCTCCHAL+DIGIT_CAL_TXFREQ);
				g_MtSchDigTxCal.PC = GetDigitInfo((nCol-1)*STRUCT_LEN_CALCTCCHAL+DIGIT_TXMOD1_FACTOR+2*(nRow-4));//获取功率值
				g_MtSchDigTxCal.Interval = m_nInterval;
				g_MtSchDigTxCal.MOD1 = GetDigitInfo((nCol-1)*STRUCT_LEN_CALCTCCHAL+DIGIT_TXMOD1_FACTOR);
				g_MtSchDigTxCal.MOD2 = GetDigitInfo((nCol-1)*STRUCT_LEN_CALCTCCHAL+DIGIT_TXMOD2_FACTOR);
				WriteCommData(&g_MtSchDigTxCal,sizeof(g_MtSchDigTxCal));
				break;
			case 6:
			case 7://音频设置
 			case 8:
			case 9:
			case 10:
			case 11:
			case 12:
			case 13://CTCSS设置
				if(nRow-1==6 ||nRow-1 == 7)
				{
					m_nCTCSSSigNum = 0;		//音频时为0
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
				if ((nRow)%2 == 0)	//频偏
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
				if ((nRow)%2 == 0)	//频偏
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
			case 16://灵敏度
				g_SpSchRxCal.TV = nVal;
				g_SpSchRxCal.txananum = GetDigitFreq((nCol-1)*STRUCT_LEN_CALCTCCHAL+DIGIT_CAL_RXFREQ);
				WriteCommData(&g_SpSchRxCal,sizeof(g_SpSchRxCal));
				break;
			case 17:
			case 18:
			case 19:
			case 20://静噪等级门限
				g_SpSchRxCal.TV = nVal;
				g_SpSchRxCal.txananum = GetDigitFreq((nCol-1)*STRUCT_LEN_CALCTCCHAL+DIGIT_CAL_RXFREQ);
				WriteCommData(&g_SpSchRxCal,sizeof(g_SpSchRxCal));
				m_nTimerID = SetTimer(1,100,NULL);
				break;
			case 21:
			case 22://RSSI门限
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
/* 发送完毕后，定时器停止，并将当前值写入内存中，准备下一步写入终端；
以滚动条与EDIT框中的数值为标准进行写入
*/
/************************************************************************/
void CDigitSetDlg::OnBnClickedCancel()
{
	// TODO: 在此添加控件通知处理程序代码
	if(m_nTimerID!=-1)
		KillTimer(m_nTimerID);
	int	nRow = m_nFocusCell.row;
	int	nCol = m_nFocusCell.col;
	int	nMsgID = 0;
	switch (nRow-1)
	{
	case 4:
	case 5://数字频偏、平衡
		nMsgID = MT_SCH_DIG_TXCAL_STOP;
		break;
	case 2:
	case 3:	//高低功率
 	case 6:
	case 7://音频设置
	case 8:
	case 9:
	case 10:
	case 11:
	case 12:
	case 13://CTCSS设置
	case 14:
	case 15://CDCSS
		nMsgID = MT_SCH_ANA_TXCAL_STOP;
 		break;
	case 16://灵敏度
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
/* 滚动框调整，变化时，相应的EDIT框也进行数值显示
*/
/************************************************************************/
void CDigitSetDlg::OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar)
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值
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
	// TODO: 在此添加消息处理程序代码和/或调用默认值
	if (m_bSend)
	{
		int	nVal = m_ScrollBar.GetScrollPos();
		if (m_nFocusCell.IsValid())
		{
			switch (m_nFocusCell.col)
			{
			case 2:
			case 3:	//高低功率
				break;
			case 17://灵敏度
				break;
			case 18:
			case 19:
			case 20:
			case 21://静噪等级门限
				g_SpMtAd.SpMtAd.checkitem = 0x01;
				SendDataToPort((void*)&g_SpMtAd,sizeof(g_SpMtAd),RX_NOISE_1_OPEN+m_nFocusCell.col-17);
				break;
			case 22:
			case 23://RSSI门限
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
			if (szBuff[16] == 0x02 && szBuff[17]==0x00)	//checkitem == 2——rssi
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
			if (szBuff[16] == 0x01 && szBuff[17]==0x00)	//checkitem == 2——rssi
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
	//若是为空，则恢复原始的频点
	if (strFreq.IsEmpty())
		return;
	strFreq.Trim();
	if(!JudgeFreqStr(strFreq))
	{
		m_Grid.SetFocusCell(nRow,nCol);
		return;
	}
	//获取频点字符并保存
	DWORD	dwFreq = _ttol(strFreq.GetBuffer());
	strFreq.ReleaseBuffer();

	SetDigitFreq(DIGIT_CAL_TXFREQ+(nRow-1)*FREQ_NUM_LEN+(nCol-1)*STRUCT_LEN_CALCTCCHAL,dwFreq);
}
BOOL	CDigitSetDlg::JudgeFreqStr(CString& strFreq)
{
	//先判断是否包含小数点，若有则删除
	TCHAR	chDot = '.';
	int	nPos = strFreq.Find(chDot);
	if (nPos>=0)
	{
		strFreq.Delete(nPos);
	}
	int	i=0;
	int	nLen = strFreq.GetLength();
	//还得判断是否都是数字，若不是，进行提示并返回
	for (;i<nLen;i++)
	{
		chDot = strFreq.GetAt(i);
		if (!isdigit(chDot))
		{
			AfxMessageBox(IDS_STRING_INVALID_NUM);
			return FALSE;
		}
	}
	//然后判断长度是否满足
	if (nLen>MAX_FREQ_NUM_LEN)
	{//长度过长时，截取前面9位
		strFreq = strFreq.Left(MAX_FREQ_NUM_LEN);
	}
	else
	{//否则，为其进行补零
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