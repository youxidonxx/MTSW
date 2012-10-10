/*******************************************************************************
*   File Name: pcserialport.cpp 

*   Description: Defines the class behaviors for the application.

*   Copyright:
      Copyright (c) 2010 LinkTop, Incorporated and its licensors. All Rights 
    Reserved. LinkTop Proprietary. Export of this technology or software is 
    regulated by the P.R.C Government. Diversion contrary to P.R.C law 
    prohibited.

*   Change History:
*   Date                 Author        Release       Description
  2011/04                Qiang jianlong      
*******************************************************************************/

/*******************************************************************************
                                Include Files
*******************************************************************************/
#include "stdafx.h"
#include "pcserialport.h"

#include <assert.h>




//
// Constructor
//
CSerialPortPC::CSerialPortPC()
{
	m_hComm = NULL;
	// initialize overlapped structure members to zero
	m_ov.Offset = 0;
	m_ov.OffsetHigh = 0;

	// create events
	m_ov.hEvent = NULL;
	m_hWriteEvent = NULL;
	m_hShutdownEvent = NULL;

	m_szWriteBuffer = NULL;
	m_nWriteSize=1;

	m_bThreadAlive = FALSE;
}

//
// Delete dynamic memory
//
CSerialPortPC::~CSerialPortPC()
{
	do
	{
		SetEvent(m_hShutdownEvent);
	} while (m_bThreadAlive);

	
	// if the port is still opened: close it 
	if (m_hComm != NULL)
	{
		CloseHandle(m_hComm);//���ڹرն��󣬺���ԭ��ΪBOOL CloseHandle(HANDLE hObject);�������ִ�гɹ�����TRUE
		m_hComm = NULL;
	}
	// Close Handles  
	if(m_hShutdownEvent!=NULL)
		CloseHandle( m_hShutdownEvent);//���ڹرն��󣬺���ԭ��ΪBOOL CloseHandle(HANDLE hObject);�������ִ�гɹ�����TRUE 
	if(m_ov.hEvent!=NULL)
		CloseHandle( m_ov.hEvent ); //���ڹرն��󣬺���ԭ��ΪBOOL CloseHandle(HANDLE hObject);�������ִ�гɹ�����TRUE
	if(m_hWriteEvent!=NULL)
		CloseHandle( m_hWriteEvent ); //���ڹرն��󣬺���ԭ��ΪBOOL CloseHandle(HANDLE hObject);�������ִ�гɹ�����TRUE

	TRACE("Thread ended\n");
	delete [] m_szWriteBuffer;
}

//
// Initialize the port. This can be port 1 to 20.
//��ʼ������
BOOL CSerialPortPC::InitPort(CWnd* pPortOwner,	// the owner (CWnd) of the port (receives message)
						   UINT  portnr,		// portnumber (1..99)
						   UINT  baud,			// baudrate
						   char  parity,		// parity 
						   UINT  databits,		// databits 
						   UINT  stopbits,		// stopbits 
						   DWORD dwCommEvents,	// EV_RXCHAR, EV_CTS etc
						   UINT  writebuffersize)	// size to the writebuffer
{
	assert(portnr > 0 && portnr < 100);//���Ӵ��ں�		assert������ʽ�����ֵΪ�٣���Ϊ0���������abort����ֹ��������
	assert(pPortOwner != NULL);
	VERIFY(portnr > 0 && portnr < 100);//���Ӵ��ں�		assert������ʽ�����ֵΪ�٣���Ϊ0���������abort����ֹ��������
	VERIFY(pPortOwner != NULL);

	// if the thread is alive: Kill�߳��ڵĻ��ر���
	
	if (m_bThreadAlive)
	{
		do
		{
			SetEvent(m_hShutdownEvent);	

		} while (m_bThreadAlive);

		TRACE("Thread ended\n");
	}


	// create events�����¼�
	if (m_ov.hEvent != NULL)
		ResetEvent(m_ov.hEvent);
	else
		m_ov.hEvent = CreateEvent(NULL, TRUE, FALSE, NULL);
//HANDLE CreateEvent(LPSECURITY_ATTRIBUTES lpEventAttributes��ȫ���ԣ�һ��ָ��SECURITY_ATTRIBUTES�ṹ��ָ�룬ȷ�����صľ���Ƿ�ɱ��ӽ��̼̳������NULL�˾�����ܱ��̳�
				   //BOOL bManualReset��λ��ʽ,����TRUE������ResetEvent�������ֹ����¼���ԭ�����ź�״̬��ΪFALSE���¼���һ���ȴ��߳��ͷź�ϵͳ���Զ����¼�״̬��ԭΪ���ź�״̬
				   //BOOL bInitialState��ʼ״̬,ָ���¼�����ĳ�ʼ״̬�����ΪTRUE,��ʼ״̬Ϊ���ź�״̬,����Ϊ���ź�״̬
				   //LPCTSTR lpName��ΪNULL,����һ���������¼����󡣶�������,ָ���¼��Ķ��������,��һ����0�������ַ���ָ�����Ƶ��ַ���ʽ�޶���MAX_PATH֮��,�����ǶԴ�Сд���е�)
	if (m_hWriteEvent != NULL)
		ResetEvent(m_hWriteEvent);
	else
		m_hWriteEvent = CreateEvent(NULL, TRUE, FALSE, NULL);
	
	if (m_hShutdownEvent != NULL)
		ResetEvent(m_hShutdownEvent);
	else
		m_hShutdownEvent = CreateEvent(NULL, TRUE, FALSE, NULL);

	// initialize the event objects�¼������ʼ���趨���ȼ���
	m_hEventArray[0] = m_hShutdownEvent;	// highest priority
	m_hEventArray[1] = m_ov.hEvent;
	m_hEventArray[2] = m_hWriteEvent;

	// initialize critical section��ʼ��һ���ٽ���Դ����
	InitializeCriticalSection(&m_csCommunicationSync);
	
	// set buffersize for writing and save the owner
	m_pOwner = pPortOwner;

	if (m_szWriteBuffer != NULL)
		delete [] m_szWriteBuffer;
	m_szWriteBuffer = new TCHAR[writebuffersize];
	
	m_nPortNr = portnr;
	m_nPortNum = m_nPortNr;

	m_nWriteBufferSize = writebuffersize;
	m_dwCommEvents = dwCommEvents;

	BOOL bResult = FALSE;
	char *szPort = new char[50];
	char *szBaud = new char[50];

	// now it critical!
	//����̲߳�����ͬ������ʱ��һ������Ҫ��˳����ʵģ�������������ݴ��ң��޷��������ݣ�������������Ϊ���������⣬����Ҫ���뻥���������ÿ���߳�
	//����˳��ط��ʱ�����Ϊ���������⣬����Ҫ���뻥���������ÿ���̶߳���˳��ط��ʱ�������������Ҫʹ��EnterCriticalSection��LeaveCriticalSection����
	//�����ٽ�����
	//Sleep(1);
	EnterCriticalSection(&m_csCommunicationSync);

	// if the port is already opened: close it
	//�����Ѿ��򿪵ľ͹ر���
	if (m_hComm != NULL)
	{
		CloseHandle(m_hComm);//���ڹرն��󣬺���ԭ��ΪBOOL CloseHandle(HANDLE hObject);�������ִ�гɹ�����TRUE
		m_hComm = NULL;
	}

	// prepare port strings
	//���ڲ���
	sprintf_s(szPort,50, ("COM%d"), portnr);//��portnr��ӡ��һ���ַ���������szPort��
	sprintf_s(szBaud,50, "baud=%d parity=%c data=%d stop=%d", baud, parity, databits, stopbits);//��baud parity�ȱ�����szBaud����һ�����Ҫ�����˺ܾ�

	for(int i =0;i<10;i++)
	{
		Sleep(1);
		m_hComm = CreateFile(szPort,GENERIC_READ | GENERIC_WRITE,0,NULL,OPEN_EXISTING,FILE_FLAG_OVERLAPPED,	0);
		if(m_hComm!=INVALID_HANDLE_VALUE)
		{
			break;
		}
	}
		
/*
	// get a handle to the port
	//ͨ�ų�����CreateFile��ָ�������豸����صĲ������ԣ��ٷ���һ��������þ�������ں�����ͨ�Ų��������ᴩ����ͨ�Ź��̡����ڴ򿪺������Ա�����ΪĬ��ֵ,
	//���ݾ�����Ҫ��ͨ������GetCommState(hComm��&&dcb)��ȡ��ǰ�����豸���ƿ�DCB���ã��޸ĺ�ͨ��SetCommState(hComm��&&dcb)����д�롣����ReadFile()��WriteFile()
	//������API����ʵ�ִ��ڶ�д��������Ϊ�첽ͨ�ŷ�ʽ�������������һ������Ϊָ��OVERLAPPED�ṹ�ķǿ�ָ�룬�ڶ�д��������ֵΪFALSE������£�����GetLastError()
	//����������ֵΪERROR_IO_PENDING,����I/O�������ң�������ת���̨����ִ�С���ʱ��������WaitForSingleObject()���ȴ������źŲ�������ȴ�ʱ�䡣
	m_hComm = CreateFile(szPort,						// communication port string (COMX)���ں�
					     GENERIC_READ | GENERIC_WRITE,	// read/write types���Զ�д
					     0,								// comm devices must be opened with exclusive access��ռ��ʽ�򿪴���
					     NULL,							// no security attributes�ް�ȫ����
					     OPEN_EXISTING,					// comm devices must use OPEN_EXISTING
					     FILE_FLAG_OVERLAPPED,			// Async I/O�첽IO
					     0);							// template must be 0 for comm devices
	//����������ɹ���������			
*/
	
	if (m_hComm == INVALID_HANDLE_VALUE)
	{
		// port not found
		delete [] szPort;
		delete [] szBaud;

		return FALSE;
	}

	// set the timeout values
	//���ó�ʱ����(�첽��IO)
	m_CommTimeouts.ReadIntervalTimeout = 1000;
	m_CommTimeouts.ReadTotalTimeoutMultiplier = 1000;
	m_CommTimeouts.ReadTotalTimeoutConstant = 1000;
	m_CommTimeouts.WriteTotalTimeoutMultiplier = 1000;
	m_CommTimeouts.WriteTotalTimeoutConstant = 1000;

	// configure
	//�ֱ����Windows API���ô��ڲ���
		/******************************************************* 
        ���Զ˿����ݵ���Ӧʱ��Ҫ����ϸ񣬿ɲ����¼�������ʽ�� 
        �¼�������ʽͨ�������¼�֪ͨ������ϣ�����¼�����ʱ��Windows 
        �������¼��ѷ�����֪ͨ������DOS�����µ��жϷ�ʽ�����ơ�Windows 
        ������9�ִ���ͨ���¼����ϳ��õ����������֣�  
 
        EV_RXCHAR:���յ�һ���ֽڣ����������뻺������  
 
        EV_TXEMPTY:����������е����һ���ַ������ͳ�ȥ��  
 
        EV_RXFLAG:���յ��¼��ַ�(DCB�ṹ��EvtChar��Ա)���������뻺���� 
 
        ����SetCommMask()ָ�������õ��¼���Ӧ�ó���ɵ���WaitCommEvent()���ȴ��� 
        ���ķ�����SetCommMask(hComm,0)��ʹWaitCommEvent()��ֹ�� 
        **************************************************************/  
	if (SetCommTimeouts(m_hComm, &m_CommTimeouts))//���ó�ʱ
	{						   
		if (SetCommMask(m_hComm, dwCommEvents))//����ͨ���¼�
		{
			if (GetCommState(m_hComm, &m_dcb))//��ȡ��ǰDCB����
			{
				m_dcb.EvtChar = 'q';//���������ַ�
				m_dcb.fRtsControl = RTS_CONTROL_ENABLE;		// set RTS bit high!
				if (BuildCommDCB(szBaud, &m_dcb))//��дDCB�ṹ
				{
					if (SetCommState(m_hComm, &m_dcb))//����DCB
						; // normal operation... continue
					else
						ProcessErrorMessage("SetCommState()");
				}
				else
					ProcessErrorMessage("BuildCommDCB()");
			}
			else
				ProcessErrorMessage("GetCommState()");
		}
		else
			ProcessErrorMessage("SetCommMask()");
	}
	else
		ProcessErrorMessage("SetCommTimeouts()");

	delete [] szPort;
	delete [] szBaud;

	// flush the port
	//��ֹ��д����ս��պͷ���
	PurgeComm(m_hComm, PURGE_RXCLEAR | PURGE_TXCLEAR | PURGE_RXABORT | PURGE_TXABORT);
        //PURGE_RXCLEAR��ս��ջ����� ��շ��ͻ����� ��ʹ����(������)û�����Ҳ������ֹ���е��ص����ղ������������� ...����(д)...����...
	// release critical section
	//���ٽ���
	LeaveCriticalSection(&m_csCommunicationSync);
	//Sleep(1);

	TRACE("Initialisation for communicationport %d completed.\nUse Startmonitor to communicate.\n", portnr);

	return TRUE;
}

//������һ���߳�֮ǰ������Ϊ�̱߳�дһ��ȫ�ֵ��̺߳���������̺߳�������һ��32λ��LPVOID��Ϊ����������һ��UINT���̺߳����ĽṹΪ
//UINT ThreadFunction(LPVOID)
//{
//	�̴߳������
//  return 0��
//}
//���̴߳�����벿��ͨ������һ����ѭ������ѭ�����ȵȴ�ĳ�¼��ķ������ٴ�����صĹ���
//while(1)
//{
//	WaitForSingleObject(..,..)��WaitForMultipleObject��..��
//  Do something
//}
//  The CommThread Function.
//���Ӵ��ں����̺߳���							C++���Ա����������Ϊ�̺߳�����һ��Ҫ���������̱߳�̣����䣩8ҳ
UINT CSerialPortPC::CommThread(LPVOID pParam)
{
	// Cast the void pointer passed to the thread back to
	// a pointer of CSerialPort class
	CSerialPortPC *port = (CSerialPortPC*)pParam;
	
	// Set the status variable in the dialog class to
	// TRUE to indicate the thread is running.
	port->m_bThreadAlive = TRUE;	
		
	// Misc. variables
	DWORD BytesTransfered = 0; 
	DWORD Event = 0;
	DWORD CommEvent = 0;
	DWORD dwError = 0;
	COMSTAT comstat;
	BOOL  bResult = TRUE;
	
	// Clear comm buffers at startup
	//��鴮���Ƿ��
	if (port->m_hComm)		// check if the port is opened
		PurgeComm(port->m_hComm, PURGE_RXCLEAR | PURGE_TXCLEAR | PURGE_RXABORT | PURGE_TXABORT);

	// begin forever loop.  This loop will run as long as the thread is alive.
	//���϶�ȡ����
	for (;;) 
	{ 

		// Make a call to WaitCommEvent().  This call will return immediatly
		// because our port was created as an async(�첽��) port (FILE_FLAG_OVERLAPPED
		// and an m_OverlappedStructerlapped structure specified).  This call will cause the 
		// m_OverlappedStructerlapped element m_OverlappedStruct.hEvent, which is part of the m_hEventArray to 
		// be placed in a non-signeled state if there are no bytes available to be read,
		// or to a signeled state if there are bytes available.  If this event handle 
		// is set to the non-signeled state, it will be set to signeled when a 
		// character arrives at the port.

		// we do this for each port!
        	/******************************************************************** 
        WaitCommEvent������3������1pOverlapped������һ��OVERLAPPED�ṹ�ı���ָ�� 
        ��Ҳ������NULL������NULLʱ����ʾ�ú�����ͬ���ģ������ʾ�ú������첽�ġ� 
        ����WaitCommEventʱ������첽��������������ɣ�����������FALSE��ϵͳ�� 
        WaitCommEvent����ǰ��OVERLAPPED�ṹ��ԱhEvent��Ϊ���ź�״̬���ȵ�����ͨ�� 
        �¼�ʱ��ϵͳ���������ź� 
        ***********************************************************************/ 
		bResult = WaitCommEvent(port->m_hComm, &Event, &port->m_ov);
		if (!bResult)  
		{ 
			//���WaitCommEvent����ErrorΪNULL�����ѯ������Ϣ
			// If WaitCommEvent() returns FALSE, process the last error to determin
			// the reason..
			switch (dwError = GetLastError()) 
			{ 
			case ERROR_IO_PENDING: 	//���������û���ַ��ɶ�  
				{ 
					// This is a normal return value if there are no bytes
					// to read at the port.
					// Do nothing and continue
					break;
				}
			case 87://ϵͳ���� 
				{
					// Under Windows NT, this value is returned for some reason.
					// I have not investigated why, but it is also a valid reply
					// Also do nothing and continue.
					break;
				}
			default://������������
				{
					// All other error codes indicate a serious error has
					// occured.  Process this error.
					port->ProcessErrorMessage("WaitCommEvent()");
					break;
				}
			}
		}
		else//WaitCommEvent()����ȷ���� 
		{
			// If WaitCommEvent() returns TRUE, check to be sure there are
			// actually bytes in the buffer to read.  
			//
			// If you are reading more than one byte at a time from the buffer 
			// (which this program does not do) you will have the situation occur 
			// where the first byte to arrive will cause the WaitForMultipleObjects() 
			// function to stop waiting.  The WaitForMultipleObjects() function 
			// resets the event handle in m_OverlappedStruct.hEvent to the non-signelead state
			// as it returns.  
			//
			// If in the time between the reset of this event and the call to 
			// ReadFile() more bytes arrive, the m_OverlappedStruct.hEvent handle will be set again
			// to the signeled state. When the call to ReadFile() occurs, it will 
			// read all of the bytes from the buffer, and the program will
			// loop back around to WaitCommEvent().
			// 
			// At this point you will be in the situation where m_OverlappedStruct.hEvent is set,
			// but there are no bytes available to read.  If you proceed and call
			// ReadFile(), it will return immediatly due to the async port setup, but
			// GetOverlappedResults() will not return until the next character arrives.
			//
			// It is not desirable for the GetOverlappedResults() function to be in 
			// this state.  The thread shutdown event (event 0) and the WriteFile()
			// event (Event2) will not work if the thread is blocked by GetOverlappedResults().
			//
			// The solution to this is to check the buffer with a call to ClearCommError().
			// This call will reset the event handle, and if there are no bytes to read
			// we can loop back through WaitCommEvent() again, then proceed.
			// If there are really bytes to read, do nothing and proceed.
	
			bResult = ClearCommError(port->m_hComm, &dwError, &comstat);

			if (comstat.cbInQue == 0)
				continue;
		}	// end if bResult
		//���ȴ��������������߳� 
		// Main wait function.  This function will normally block the thread
		// until one of nine events occur that require action.
		Event = WaitForMultipleObjects(3, port->m_hEventArray, FALSE, INFINITE);//�ȴ�3���¼����ض�/��/д����һ���¼������ͷ��� 
		
		//�¼���������е��¼���3���¼�,�¼�����,��һ���¼������ͷ�����TRUE�������¼�����ʱ�ŷ���,��ʱʱ��
		switch (Event)
		{
		case 0:
			{
				// Shutdown event.  This is event zero so it will be
				// the higest priority and be serviced first.
				//�ض��¼����رմ���  
				CloseHandle(port->m_hComm);//���ڹرն��󣬺���ԭ��ΪBOOL CloseHandle(HANDLE hObject);�������ִ�гɹ�����TRUE
				port->m_hComm=NULL;
				port->m_bThreadAlive = FALSE;
				
				// Kill this thread.  break is not needed, but makes me feel better._endthread���������߳��ڵ�������ֹ������
				//ר������ͨ�Ŵ�����߳����޷���ȡ��ͨ�Ŷ˿ڵĿ��ƣ�����˳���
				
				AfxEndThread(100);//һ���߳��Լ������������Լ�
				port->m_Thread->m_bAutoDelete = TRUE;
				AfxMessageBox(_T("�̹߳ر�"));
				break;
			}
		case 1:	// read event
			{
				//���� 
				GetCommMask(port->m_hComm, &CommEvent);
				if (CommEvent & EV_RXCHAR)
					// Receive character event from port.
					ReceiveChar(port, comstat);//�Ӵ��ڶ���
				if (CommEvent & EV_CTS)
					::SendMessage(port->m_pOwner->m_hWnd, WM_COMM_CTS_DETECTED, (WPARAM) 0, (LPARAM) port->m_nPortNr);
				if (CommEvent & EV_BREAK)
					::SendMessage(port->m_pOwner->m_hWnd, WM_COMM_BREAK_DETECTED, (WPARAM) 0, (LPARAM) port->m_nPortNr);
				if (CommEvent & EV_ERR)
					::SendMessage(port->m_pOwner->m_hWnd, WM_COMM_ERR_DETECTED, (WPARAM) 0, (LPARAM) port->m_nPortNr);
				if (CommEvent & EV_RING)
					::SendMessage(port->m_pOwner->m_hWnd, WM_COMM_RING_DETECTED, (WPARAM) 0, (LPARAM) port->m_nPortNr);
				
				if (CommEvent & EV_RXFLAG)
					::SendMessage(port->m_pOwner->m_hWnd, WM_COMM_RXFLAG_DETECTED, (WPARAM) 0, (LPARAM) port->m_nPortNr);
					
				break;
			}  
		case 2: // write event��������
			{
				// Write character event from port
				WriteChar(port);
				break;
			}

		} // end switch

	} // close forever loop

	return 0;
}

//
// start comm watching
//���ƴ��ڼ����߳�:�������Ӵ���
BOOL CSerialPortPC::StartMonitoring()
{
	if (!(m_Thread = AfxBeginThread(CommThread,this)))
		return FALSE;
	TRACE("Thread started\n");
	return TRUE;	
}

//
// Restart the comm thread
//���ƴ��ڼ����߳�:���������߳�(��������)
BOOL CSerialPortPC::RestartMonitoring()
{
	TRACE("Thread resumed\n");
	m_Thread->ResumeThread();//�������̵߳�ʱ��������䴫��CREATE_SUSPENDED��־������̴߳����󱻹�������ResumeThread�ָ���
	//DWORD ResumeThread(HANDLE hThread);���ResumeThread���гɹ����������̵߳�ǰһ����ͣ���������򷵻�0xFFFFFFFF��
	return TRUE;	
}


//
// Suspend the comm thread
//���ƴ��ڼ����߳�:ֹͣ  ��������߳�
BOOL CSerialPortPC::StopMonitoring()
{
	TRACE("Thread suspended\n");
	m_Thread->SuspendThread(); //ǿ�й����߳�DWORD SuspendThread(HANDLE hThread);��Ҫһ���߳̿��Ա������Ρ��߳̿���������ͣ���У�
							   //���������лָ����С����һ���̱߳�����n�Σ�����߳�Ҳ���뱻�ָ�n�βſ��ܵ���ִ�С�
	return TRUE;	
}

//���״̬
void	CSerialPortPC::ShowPortState()
{
 	m_Thread->Delete();
	return;
}
//
// If there is a error, give the right message
//
void CSerialPortPC::ProcessErrorMessage(char* ErrorText)
{
	TCHAR *Temp = new TCHAR[200];
	
	LPVOID lpMsgBuf;

	FormatMessage( 
		FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM,
		NULL,
		GetLastError(),
		MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT), // Default language
		(LPTSTR) &lpMsgBuf,
		0,
		NULL 
	);

	sprintf_s((char*)Temp,200, "WARNING:  %s Failed with the following error: \n%s\nPort: %d\n", (char*)ErrorText, lpMsgBuf, m_nPortNr); 
	MessageBox(NULL, Temp, _T("Application Error"), MB_ICONSTOP);

	LocalFree(lpMsgBuf);
	delete [] Temp;
}

//
// Write a character.
//�̵߳��õĺ���WriteChar���ѻ����������д�������У��ڼ����WriteFile
void CSerialPortPC::WriteChar(CSerialPortPC* port)
{
	BOOL bWrite = TRUE;
	BOOL bResult = TRUE;
	DWORD BytesSent = 0;
	ResetEvent(port->m_hWriteEvent);//��λд�¼����
	
	// Gain ownership of the critical section
	EnterCriticalSection(&port->m_csCommunicationSync);
	if (bWrite)
	{
		// Initailize variables
		port->m_ov.Offset = 0;
		port->m_ov.OffsetHigh = 0;

		// Clear buffer
		PurgeComm(port->m_hComm, PURGE_RXCLEAR | PURGE_TXCLEAR | PURGE_RXABORT | PURGE_TXABORT);
		//����д��  
		bResult = WriteFile(port->m_hComm,							// Handle to COMM Port
							port->m_szWriteBuffer,					// Pointer to message buffer in calling finction
//							strlen((char*)port->m_szWriteBuffer),	// Length of message to send
							port->m_nWriteSize,	// Length of message to send
							&BytesSent,								// Where to store the number of bytes sent
							&port->m_ov);							// Overlapped structure

		// deal with any error codes
		if (!bResult)  
		{
			DWORD dwError = GetLastError();
			switch (dwError)
			{
				case ERROR_IO_PENDING:
					{
						// continue to GetOverlappedResults()
						BytesSent = 0;
						bWrite = FALSE;
						break;
					}
				default:
					{
						// all other error codes
						port->ProcessErrorMessage("WriteFile()");
					}
			}
		} 
		else
		{
			LeaveCriticalSection(&port->m_csCommunicationSync);
		}
	} // end if(bWrite)

	if (!bWrite)  
	{
		bWrite = TRUE;
	
		bResult = GetOverlappedResult(port->m_hComm,	// Handle to COMM port 
									  &port->m_ov,		// Overlapped structure
									  &BytesSent,		// Stores number of bytes sent
									  TRUE); 			// Wait flag

		LeaveCriticalSection(&port->m_csCommunicationSync);

		// deal with the error code 
//		if (!bResult)  
		{
//			port->ProcessErrorMessage("GetOverlappedResults() in WriteFile()");
		}	
	} // end if (!bWrite)

//	::SendMessage((port->m_pOwner)->m_hWnd, WM_COMM_TXEMPTY_DETECTED,0,(LPARAM) port->m_nPortNr);//����Ƿ������^_^
}

//
// Character received. Inform the owner
//�����ַ�
//��ȡ������һ���첽�������������ݷ���ʱ���ᴥ�����¼�m_ov.hEvent�������̲߳�׽���¼��󲢻�֪�Ƕ��¼���������ض�����������ú���ReceiveChar
//��ReceiveChar�е���ReadFile�������������ݶ���Buffer�����У���ش������£�
void CSerialPortPC::ReceiveChar(CSerialPortPC* port, COMSTAT comstat)
{
	BOOL  bRead = TRUE; 
	BOOL  bResult = TRUE;
	DWORD dwError = 0;
	DWORD BytesRead = 0;
	unsigned char RXBuff;

	for (;;) 
	{ 
		// Gain ownership of the comm port critical section.
		// This process guarantees no other part of this program 
		// is using the port object. 
		
		EnterCriticalSection(&port->m_csCommunicationSync);

		// ClearCommError() will update the COMSTAT structure and
		// clear any other errors.
		//����COMSTAT  
		bResult = ClearCommError(port->m_hComm, &dwError, &comstat);

		LeaveCriticalSection(&port->m_csCommunicationSync);

		// start forever loop.  I use this type of loop because I
		// do not know at runtime how many loops this will have to
		// run. My solution is to start a forever loop and to
		// break out of it when I have processed all of the
		// data available.  Be careful with this approach and
		// be sure your loop will exit.
		// My reasons for this are not as clear in this sample 
		// as it is in my production code, but I have found this 
		// solutiion to be the most efficient way to do this.
		//�����ַ������������ж�ѭ��
		if (comstat.cbInQue == 0)
		{
			// break out when all bytes have been read
			break;
		}
						
		EnterCriticalSection(&port->m_csCommunicationSync);

		if (bRead)
		{
			//���ڶ������������������ֽ�  
			bResult = ReadFile(port->m_hComm,		// Handle to COMM port ָ���ʶ�ľ��
							   &RXBuff,				// RX Buffer Pointerָ��һ��������
							   1,					// Read one byte��ȡ���ֽ���
							   &BytesRead,			// Stores number of bytes readָ����ú����������ֽ���
							   &port->m_ov);		// pointer to the m_ov structureһ��OVERLAPPED�ṹ
			// deal with the error code�����ش��󣬴���֮
			if (!bResult)  
			{ 
				switch (dwError = GetLastError()) 
				{ 
					case ERROR_IO_PENDING: 	
						{ 
							// asynchronous i/o is still in progress�첽IO���ڽ���  
							// Proceed on to GetOverlappedResults();
							bRead = FALSE;
							break;
						}
					default:
						{	//���������� 
							// Another error has occured.  Process this error.
						//	port->ProcessErrorMessage("ReadFile()");
							break;
						} 
				}
			}
			else
			{	//ReadFile����TRUE 
				// ReadFile() returned complete. It is not necessary to call GetOverlappedResults()
				bRead = TRUE;
			}
		}  // close if (bRead)
		//�첽IO�������ڽ��У���Ҫ����GetOverlappedResult��ѯ
		if (!bRead)
		{
			bRead = TRUE;
			bResult = GetOverlappedResult(port->m_hComm,	// Handle to COMM port 
										  &port->m_ov,		// Overlapped structure
										  &BytesRead,		// Stores number of bytes read
										  TRUE); 			// Wait flag

			// deal with the error code 
			if (!bResult)  
			{
				port->ProcessErrorMessage("GetOverlappedResults() in ReadFile()");
			}	
		}  // close if (!bRead)
				
		LeaveCriticalSection(&port->m_csCommunicationSync);

		// notify parent that a byte was received
		::SendMessage((port->m_pOwner)->m_hWnd, WM_COMM_RXCHAR, (WPARAM) RXBuff, (LPARAM) port->m_nPortNr);
	} // end forever loop

}

//
// Write a string to the port
//
void CSerialPortPC::WriteToPort(char* string)
{		
	assert(m_hComm != 0);
	VERIFY(m_hComm != 0);
	
	memset(m_szWriteBuffer, 0, sizeof(m_szWriteBuffer));
	strcpy_s((char*)m_szWriteBuffer, sizeof(m_szWriteBuffer),string);
	m_nWriteSize=strlen(string);

	// set event for write
	SetEvent(m_hWriteEvent);//֪ͨ�����߳���������Ҫ����
}


void CSerialPortPC::WriteToPort(char* string,int n)
{		
	assert(m_hComm != 0);
	VERIFY(m_hComm != 0);

	memset(m_szWriteBuffer, 0, sizeof(m_szWriteBuffer));
	memcpy(m_szWriteBuffer, string, n);
	m_nWriteSize=n;

	// set event for write
	SetEvent(m_hWriteEvent);//֪ͨ�����߳���������Ҫ����
}

void CSerialPortPC::WriteToPort(LPCTSTR string)
{		
	assert(m_hComm != 0);
	VERIFY(m_hComm != 0);

	memset(m_szWriteBuffer, 0, sizeof(m_szWriteBuffer));
	_tcscpy(m_szWriteBuffer, string);
	m_nWriteSize=_tcslen(string);

	// set event for write
	SetEvent(m_hWriteEvent);
}

void CSerialPortPC::WriteToPort1(LPCTSTR string)
{		
	assert(m_hComm != 0);
	VERIFY(m_hComm != 0);
	memset(m_szWriteBuffer, 0, sizeof(m_szWriteBuffer));
	_tcscpy(m_szWriteBuffer , string);
	m_nWriteSize=_tcslen(string);
	m_szWriteBuffer[m_nWriteSize]=0x0d;
	m_nWriteSize=m_nWriteSize+1;//strlen�����ַ���ֱ��������һ���ַ�����������\0��Ϊֹ,+1����Ϊ����0x0d
	// set event for write
	SetEvent(m_hWriteEvent);//֪ͨ�����߳���������Ҫ����
}

void CSerialPortPC::WriteToPort(LPCTSTR string,int n)
{		
	assert(m_hComm != 0);//������û�д�������ֹ����
	VERIFY(m_hComm != 0);
	memset(m_szWriteBuffer, 0, sizeof(m_szWriteBuffer));//��ջ�����
	memcpy(m_szWriteBuffer, string, n);//��string�п���n���ֽڵ�m_szWriteBuffer
	m_nWriteSize=n;
	// set event for write
	SetEvent(m_hWriteEvent);//֪ͨ�����߳���������Ҫ����
}
int	CSerialPortPC::GetPortNum()
{
	return m_nPortNr;
}
//
// Return the device control block
//���ƴ��ڼ����߳�:��ȡDCB
DCB CSerialPortPC::GetDCB()
{
	return m_dcb;
}

//
// Return the communication event masks
//���ƴ��ڼ����߳�:��ȡ�¼�
DWORD CSerialPortPC::GetCommEvents()
{
	return m_dwCommEvents;
}

//
// Return the output buffer size
//���ƴ��ڼ����߳�:��ȡд�����С
DWORD CSerialPortPC::GetWriteBufferSize()
{
	return m_nWriteBufferSize;
}

//�رմ���
void CSerialPortPC::ClosePort()
{
		SetEvent(m_hShutdownEvent);
}

void CSerialPortPC::SetWriteBuffer(unsigned long size)
{
	if (m_szWriteBuffer != NULL)
	{
		delete [] m_szWriteBuffer;
	}
	m_szWriteBuffer = new TCHAR[size];
	
	m_nWriteBufferSize = size;

}

/*
void CSerialPort::ClosePort()
{
	do
	{
		SetEvent(m_hShutdownEvent);
	} while (m_bThreadAlive);

	
	// if the port is still opened: close it 
	if (m_hComm != NULL)
	{
		CloseHandle(m_hComm);
		m_hComm = NULL;
	}
	// Close Handles  
	if(m_hShutdownEvent!=NULL)
		CloseHandle( m_hShutdownEvent); 
	if(m_ov.hEvent!=NULL)
		CloseHandle( m_ov.hEvent ); 
	if(m_hWriteEvent!=NULL)
		CloseHandle( m_hWriteEvent ); 

	TRACE("Thread ended\n");
	delete [] m_szWriteBuffer;
}
*/
