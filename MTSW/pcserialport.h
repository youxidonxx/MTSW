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
	int m_nWriteSize; //Ҫд�봮�ڵ����ݴ�С
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
	//���ƴ��ڼ����߳�
	BOOL		StartMonitoring();//����
	BOOL		RestartMonitoring();//��λ
	BOOL		StopMonitoring();//ֹͣ

	DWORD		GetWriteBufferSize();//��ȡд�����С
	DWORD		GetCommEvents();//��ȡ�¼�
	DCB			GetDCB();//��ȡDCB
	//д���ݵ�����
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
	static UINT	CommThread(LPVOID pParam);//�̺߳���
	static void	ReceiveChar(CSerialPortPC* port, COMSTAT comstat);//�����ַ�
	static void	WriteChar(CSerialPortPC* port);//д�ַ�

	// thread�����߳�
	CWinThread*			m_Thread;

	// synchronisation objects�ٽ���Դ
	CRITICAL_SECTION	m_csCommunicationSync;//define temporary critical section word������ʱ���
	BOOL				m_bThreadAlive;//�����߳����б�־

	// handles�¼����
	HANDLE				m_hWriteEvent;
	HANDLE				m_hShutdownEvent;

	// Event array. 
	// One element is used for each event. There are two event handles for each port.
	// A Write event and a receive character event which is located in the overlapped structure (m_ov.hEvent).
	// There is a general shutdown when the port is closed. 
	//�¼����飬����дһ���¼��������¼����ر��¼�
	HANDLE				m_hEventArray[3];

	// structures
	OVERLAPPED			m_ov;//�����첽IO����
	COMMTIMEOUTS		m_CommTimeouts;//��ʱ����
	DCB					m_dcb;//�豸���ƿ�

	// owner window
	CWnd*				m_pOwner;

	// misc
	UINT				m_nPortNr;
	TCHAR*				m_szWriteBuffer;//д������
	DWORD				m_dwCommEvents;
	DWORD				m_nWriteBufferSize;//д�����С
};

#endif __SERIALPORT_H__


