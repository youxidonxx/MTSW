/*******************************************************************************
*   File Name: pcserialport.h 

*   Description: Defines the class behaviors for the application.

*   Copyright:
      Copyright (c) 2010 LinkTop, Incorporated and its licensors. All Rights 
    Reserved. LinkTop Proprietary. Export of this technology or software is 
    regulated by the P.R.C Government. Diversion contrary to P.R.C law 
    prohibited.

*   Change History:
*   Date                 Author        Release       Description
*******************************************************************************/

/*******************************************************************************
                                Include Files
*******************************************************************************/
#ifndef __PCSERIALPORT_H__
#define __PCSERIALPORT_H__

#define WM_COMM_BREAK_DETECTED		WM_USER+1	// A break was detected on input.
#define WM_COMM_CTS_DETECTED		WM_USER+2	// The CTS (clear-to-send) signal changed state. 
#define WM_COMM_DSR_DETECTED		WM_USER+3	// The DSR (data-set-ready) signal changed state. 
#define WM_COMM_ERR_DETECTED		WM_USER+4	// A line-status error occurred. Line-status errors are CE_FRAME, CE_OVERRUN, and CE_RXPARITY. 
#define WM_COMM_RING_DETECTED		WM_USER+5	// A ring indicator was detected. 
#define WM_COMM_RLSD_DETECTED		WM_USER+6	// The RLSD (receive-line-signal-detect) signal changed state. 
#define WM_COMM_RXCHAR				WM_USER+7	// A character was received and placed in the input buffer. 
#define WM_COMM_RXFLAG_DETECTED		WM_USER+8	// The event character was received and placed in the input buffer.  
#define WM_COMM_TXEMPTY_DETECTED	WM_USER+9	// The last character in the output buffer was sent.  


class CSerialPortPC
{														 
public:
	int m_nWriteSize; //要写入串口的数据大小
	void ClosePort();
	// contruction and destruction
	CSerialPortPC();
	virtual		~CSerialPortPC();

	// port initialisation											
	BOOL		InitPort(CWnd* pPortOwner, UINT portnr = 1, UINT baud = 115200, 
		char parity = 'N', UINT databits = 8, UINT stopbits = 1, DWORD dwCommEvents = EV_RXCHAR, UINT writebuffersize = 1024);
	HANDLE				m_hComm;
	int			m_nPortNum;
	// start/stop comm watching
	//控制串口监视线程
	BOOL		StartMonitoring();//开启
	BOOL		RestartMonitoring();//复位
	BOOL		StopMonitoring();//停止

	DWORD		GetWriteBufferSize();//获取写缓冲大小
	DWORD		GetCommEvents();//获取事件
	DCB			GetDCB();//获取DCB
	//写数据到串口
	void		WriteToPort(char* string);
	void		WriteToPort(char* string,int n);
	void		WriteToPort(LPCTSTR string);
	void		WriteToPort(LPCTSTR string,int n);
	void		WriteToPort1(LPCTSTR string);
	void		SetWriteBuffer(unsigned long size);
	int			GetPortNum();
	void		ShowPortState();
	//void		DeleteWriteBuffer(viod);
protected:
	// protected memberfunctions
	void		ProcessErrorMessage(char* ErrorText);
	static UINT	CommThread(LPVOID pParam);//线程函数
	static void	ReceiveChar(CSerialPortPC* port, COMSTAT comstat);//接收字符
	static void	WriteChar(CSerialPortPC* port);//写字符

	// thread监视线程
	CWinThread*			m_Thread;

	// synchronisation objects临界资源
	CRITICAL_SECTION	m_csCommunicationSync;//define temporary critical section word定义临时句柄
	BOOL				m_bThreadAlive;//监视线程运行标志

	// handles事件句柄
	HANDLE				m_hWriteEvent;
	HANDLE				m_hShutdownEvent;

	// Event array. 
	// One element is used for each event. There are two event handles for each port.
	// A Write event and a receive character event which is located in the overlapped structure (m_ov.hEvent).
	// There is a general shutdown when the port is closed. 
	//事件数组，包括写一个事件，接受事件，关闭事件
	HANDLE				m_hEventArray[3];

	// structures
	OVERLAPPED			m_ov;//设置异步IO操作
	COMMTIMEOUTS		m_CommTimeouts;//超时设置
	DCB					m_dcb;//设备控制块

	// owner window
	CWnd*				m_pOwner;

	// misc
	UINT				m_nPortNr;
	TCHAR*				m_szWriteBuffer;//写缓冲区
	DWORD				m_dwCommEvents;
	DWORD				m_nWriteBufferSize;//写缓冲大小
};

#endif __SERIALPORT_H__


