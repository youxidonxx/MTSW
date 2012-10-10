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
		CloseHandle(m_hComm);//用于关闭对象，函数原型为BOOL CloseHandle(HANDLE hObject);如果函数执行成功返回TRUE
		m_hComm = NULL;
	}
	// Close Handles  
	if(m_hShutdownEvent!=NULL)
		CloseHandle( m_hShutdownEvent);//用于关闭对象，函数原型为BOOL CloseHandle(HANDLE hObject);如果函数执行成功返回TRUE 
	if(m_ov.hEvent!=NULL)
		CloseHandle( m_ov.hEvent ); //用于关闭对象，函数原型为BOOL CloseHandle(HANDLE hObject);如果函数执行成功返回TRUE
	if(m_hWriteEvent!=NULL)
		CloseHandle( m_hWriteEvent ); //用于关闭对象，函数原型为BOOL CloseHandle(HANDLE hObject);如果函数执行成功返回TRUE

	TRACE("Thread ended\n");
	delete [] m_szWriteBuffer;
}

//
// Initialize the port. This can be port 1 to 20.
//初始化串口
BOOL CSerialPortPC::InitPort(CWnd* pPortOwner,	// the owner (CWnd) of the port (receives message)
						   UINT  portnr,		// portnumber (1..99)
						   UINT  baud,			// baudrate
						   char  parity,		// parity 
						   UINT  databits,		// databits 
						   UINT  stopbits,		// stopbits 
						   DWORD dwCommEvents,	// EV_RXCHAR, EV_CTS etc
						   UINT  writebuffersize)	// size to the writebuffer
{
	assert(portnr > 0 && portnr < 100);//增加串口号		assert计算表达式如果其值为假（即为0），会调用abort来终止程序运行
	assert(pPortOwner != NULL);
	VERIFY(portnr > 0 && portnr < 100);//增加串口号		assert计算表达式如果其值为假（即为0），会调用abort来终止程序运行
	VERIFY(pPortOwner != NULL);

	// if the thread is alive: Kill线程在的话关闭它
	
	if (m_bThreadAlive)
	{
		do
		{
			SetEvent(m_hShutdownEvent);	

		} while (m_bThreadAlive);

		TRACE("Thread ended\n");
	}


	// create events创建事件
	if (m_ov.hEvent != NULL)
		ResetEvent(m_ov.hEvent);
	else
		m_ov.hEvent = CreateEvent(NULL, TRUE, FALSE, NULL);
//HANDLE CreateEvent(LPSECURITY_ATTRIBUTES lpEventAttributes安全属性，一个指向SECURITY_ATTRIBUTES结构的指针，确定返回的句柄是否可被子进程继承如果是NULL此句柄不能被继承
				   //BOOL bManualReset复位方式,若是TRUE必须用ResetEvent函数来手工将事件复原到无信号状态若为FALSE当事件被一个等待线程释放后系统将自动将事件状态复原为无信号状态
				   //BOOL bInitialState初始状态,指定事件对象的初始状态。如果为TRUE,初始状态为有信号状态,否则为无信号状态
				   //LPCTSTR lpName若为NULL,创建一个无名的事件对象。对象名称,指定事件的对象的名称,是一个以0结束的字符串指针名称的字符格式限定在MAX_PATH之内,名字是对大小写敏感的)
	if (m_hWriteEvent != NULL)
		ResetEvent(m_hWriteEvent);
	else
		m_hWriteEvent = CreateEvent(NULL, TRUE, FALSE, NULL);
	
	if (m_hShutdownEvent != NULL)
		ResetEvent(m_hShutdownEvent);
	else
		m_hShutdownEvent = CreateEvent(NULL, TRUE, FALSE, NULL);

	// initialize the event objects事件数组初始化设定优先级别
	m_hEventArray[0] = m_hShutdownEvent;	// highest priority
	m_hEventArray[1] = m_ov.hEvent;
	m_hEventArray[2] = m_hWriteEvent;

	// initialize critical section初始化一个临界资源对象
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
	//多个线程操作相同的数据时，一般是需要按顺序访问的，否则会引导数据错乱，无法控制数据，变成随机变量。为解决这个问题，就需要引入互斥变量，让每个线程
	//都按顺序地访问变量。为解决这个问题，就需要引入互斥变量，让每个线程都按顺序地访问变量。这样就需要使用EnterCriticalSection和LeaveCriticalSection函数
	//进入临界区。
	//Sleep(1);
	EnterCriticalSection(&m_csCommunicationSync);

	// if the port is already opened: close it
	//串口已经打开的就关闭它
	if (m_hComm != NULL)
	{
		CloseHandle(m_hComm);//用于关闭对象，函数原型为BOOL CloseHandle(HANDLE hObject);如果函数执行成功返回TRUE
		m_hComm = NULL;
	}

	// prepare port strings
	//串口参数
	sprintf_s(szPort,50, ("COM%d"), portnr);//把portnr打印成一个字符串保存在szPort中
	sprintf_s(szBaud,50, "baud=%d parity=%c data=%d stop=%d", baud, parity, databits, stopbits);//把baud parity等保存在szBaud中这一句很重要郁闷了很久

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
	//通信程序在CreateFile处指定串口设备及相关的操作属性，再返回一个句柄，该句柄被用于后续的通信操作，并贯穿整个通信过程。串口打开后，其属性被设置为默认值,
	//根据具体需要，通过调用GetCommState(hComm，&&dcb)读取当前串口设备控制块DCB设置，修改后通过SetCommState(hComm，&&dcb)将其写入。运用ReadFile()与WriteFile()
	//这两个API函数实现串口读写操作，若为异步通信方式，两函数中最后一个参数为指向OVERLAPPED结构的非空指针，在读写函数返回值为FALSE的情况下，调用GetLastError()
	//函数，返回值为ERROR_IO_PENDING,表明I/O操作悬挂，即操作转入后台继续执行。此时，可以用WaitForSingleObject()来等待结束信号并设置最长等待时间。
	m_hComm = CreateFile(szPort,						// communication port string (COMX)串口号
					     GENERIC_READ | GENERIC_WRITE,	// read/write types可以读写
					     0,								// comm devices must be opened with exclusive access独占方式打开串口
					     NULL,							// no security attributes无安全属性
					     OPEN_EXISTING,					// comm devices must use OPEN_EXISTING
					     FILE_FLAG_OVERLAPPED,			// Async I/O异步IO
					     0);							// template must be 0 for comm devices
	//如果创建不成功，错误处理			
*/
	
	if (m_hComm == INVALID_HANDLE_VALUE)
	{
		// port not found
		delete [] szPort;
		delete [] szBaud;

		return FALSE;
	}

	// set the timeout values
	//设置超时上限(异步邋IO)
	m_CommTimeouts.ReadIntervalTimeout = 1000;
	m_CommTimeouts.ReadTotalTimeoutMultiplier = 1000;
	m_CommTimeouts.ReadTotalTimeoutConstant = 1000;
	m_CommTimeouts.WriteTotalTimeoutMultiplier = 1000;
	m_CommTimeouts.WriteTotalTimeoutConstant = 1000;

	// configure
	//分别调用Windows API设置串口参数
		/******************************************************* 
        若对端口数据的响应时间要求较严格，可采用事件驱动方式。 
        事件驱动方式通过设置事件通知，当所希望的事件发生时，Windows 
        发出该事件已发生的通知，这与DOS环境下的中断方式很相似。Windows 
        定义了9种串口通信事件，较常用的有以下三种：  
 
        EV_RXCHAR:接收到一个字节，并放入输入缓冲区；  
 
        EV_TXEMPTY:输出缓冲区中的最后一个字符，发送出去；  
 
        EV_RXFLAG:接收到事件字符(DCB结构中EvtChar成员)，放入输入缓冲区 
 
        在用SetCommMask()指定了有用的事件后，应用程序可调用WaitCommEvent()来等待事 
        件的发生。SetCommMask(hComm,0)可使WaitCommEvent()中止。 
        **************************************************************/  
	if (SetCommTimeouts(m_hComm, &m_CommTimeouts))//设置超时
	{						   
		if (SetCommMask(m_hComm, dwCommEvents))//设置通信事件
		{
			if (GetCommState(m_hComm, &m_dcb))//获取当前DCB参数
			{
				m_dcb.EvtChar = 'q';//设置字体字符
				m_dcb.fRtsControl = RTS_CONTROL_ENABLE;		// set RTS bit high!
				if (BuildCommDCB(szBaud, &m_dcb))//填写DCB结构
				{
					if (SetCommState(m_hComm, &m_dcb))//配置DCB
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
	//终止读写并清空接收和发送
	PurgeComm(m_hComm, PURGE_RXCLEAR | PURGE_TXCLEAR | PURGE_RXABORT | PURGE_TXABORT);
        //PURGE_RXCLEAR清空接收缓冲区 清空发送缓冲区 即使接收(读操作)没有完成也立即终止所有的重叠接收操作，立即返回 ...发送(写)...发送...
	// release critical section
	//出临界区
	LeaveCriticalSection(&m_csCommunicationSync);
	//Sleep(1);

	TRACE("Initialisation for communicationport %d completed.\nUse Startmonitor to communicate.\n", portnr);

	return TRUE;
}

//在启动一个线程之前，必须为线程编写一个全局的线程函数，这个线程函数接受一个32位的LPVOID作为参数，返回一个UINT，线程函数的结构为
//UINT ThreadFunction(LPVOID)
//{
//	线程处理代码
//  return 0；
//}
//在线程处理代码部分通常包括一个死循环，该循环中先等待某事件的发生，再处理相关的工作
//while(1)
//{
//	WaitForSingleObject(..,..)或WaitForMultipleObject（..）
//  Do something
//}
//  The CommThread Function.
//监视串口函数线程函数							C++类成员函数不能作为线程函数若一定要方法见多线程编程（经典）8页
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
	//检查串口是否打开
	if (port->m_hComm)		// check if the port is opened
		PurgeComm(port->m_hComm, PURGE_RXCLEAR | PURGE_TXCLEAR | PURGE_RXABORT | PURGE_TXABORT);

	// begin forever loop.  This loop will run as long as the thread is alive.
	//不断读取数据
	for (;;) 
	{ 

		// Make a call to WaitCommEvent().  This call will return immediatly
		// because our port was created as an async(异步的) port (FILE_FLAG_OVERLAPPED
		// and an m_OverlappedStructerlapped structure specified).  This call will cause the 
		// m_OverlappedStructerlapped element m_OverlappedStruct.hEvent, which is part of the m_hEventArray to 
		// be placed in a non-signeled state if there are no bytes available to be read,
		// or to a signeled state if there are bytes available.  If this event handle 
		// is set to the non-signeled state, it will be set to signeled when a 
		// character arrives at the port.

		// we do this for each port!
        	/******************************************************************** 
        WaitCommEvent函数第3个参数1pOverlapped可以是一个OVERLAPPED结构的变量指针 
        ，也可以是NULL，当用NULL时，表示该函数是同步的，否则表示该函数是异步的。 
        调用WaitCommEvent时，如果异步操作不能立即完成，会立即返回FALSE，系统在 
        WaitCommEvent返回前将OVERLAPPED结构成员hEvent设为无信号状态，等到产生通信 
        事件时，系统将其置有信号 
        ***********************************************************************/ 
		bResult = WaitCommEvent(port->m_hComm, &Event, &port->m_ov);
		if (!bResult)  
		{ 
			//如果WaitCommEvent返回Error为NULL，则查询错误信息
			// If WaitCommEvent() returns FALSE, process the last error to determin
			// the reason..
			switch (dwError = GetLastError()) 
			{ 
			case ERROR_IO_PENDING: 	//正常情况，没有字符可读  
				{ 
					// This is a normal return value if there are no bytes
					// to read at the port.
					// Do nothing and continue
					break;
				}
			case 87://系统错误 
				{
					// Under Windows NT, this value is returned for some reason.
					// I have not investigated why, but it is also a valid reply
					// Also do nothing and continue.
					break;
				}
			default://发生其他错误
				{
					// All other error codes indicate a serious error has
					// occured.  Process this error.
					port->ProcessErrorMessage("WaitCommEvent()");
					break;
				}
			}
		}
		else//WaitCommEvent()能正确返回 
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
		//主等待函数，会阻塞线程 
		// Main wait function.  This function will normally block the thread
		// until one of nine events occur that require action.
		Event = WaitForMultipleObjects(3, port->m_hEventArray, FALSE, INFINITE);//等待3个事件：关断/读/写，有一个事件发生就返回 
		
		//事件句柄数组中的事件数3个事件,事件数组,有一个事件发生就返回若TRUE则所有事件发生时才返回,超时时间
		switch (Event)
		{
		case 0:
			{
				// Shutdown event.  This is event zero so it will be
				// the higest priority and be serviced first.
				//关断事件，关闭串口  
				CloseHandle(port->m_hComm);//用于关闭对象，函数原型为BOOL CloseHandle(HANDLE hObject);如果函数执行成功返回TRUE
				port->m_hComm=NULL;
				port->m_bThreadAlive = FALSE;
				
				// Kill this thread.  break is not needed, but makes me feel better._endthread函数对于线程内的条件终止很有用
				//专门用于通信处理的线程若无法获取对通信端口的控制，则会退出。
				
				AfxEndThread(100);//一个线程自己调用来结束自己
				port->m_Thread->m_bAutoDelete = TRUE;
				AfxMessageBox(_T("线程关闭"));
				break;
			}
		case 1:	// read event
			{
				//接收 
				GetCommMask(port->m_hComm, &CommEvent);
				if (CommEvent & EV_RXCHAR)
					// Receive character event from port.
					ReceiveChar(port, comstat);//从串口读数
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
		case 2: // write event发送数据
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
//控制串口监视线程:开启监视串口
BOOL CSerialPortPC::StartMonitoring()
{
	if (!(m_Thread = AfxBeginThread(CommThread,this)))
		return FALSE;
	TRACE("Thread started\n");
	return TRUE;	
}

//
// Restart the comm thread
//控制串口监视线程:重启监视线程(挂起重启)
BOOL CSerialPortPC::RestartMonitoring()
{
	TRACE("Thread resumed\n");
	m_Thread->ResumeThread();//当创建线程的时候，如果给其传入CREATE_SUSPENDED标志，则该线程创建后被挂起需用ResumeThread恢复它
	//DWORD ResumeThread(HANDLE hThread);如果ResumeThread运行成功它将返回线程的前一个暂停计数，否则返回0xFFFFFFFF。
	return TRUE;	
}


//
// Suspend the comm thread
//控制串口监视线程:停止  挂起监视线程
BOOL CSerialPortPC::StopMonitoring()
{
	TRACE("Thread suspended\n");
	m_Thread->SuspendThread(); //强行挂起线程DWORD SuspendThread(HANDLE hThread);主要一个线程可以被挂起多次。线程可以自行暂停运行，
							   //但不能自行恢复运行。如果一个线程被挂起n次，则该线程也必须被恢复n次才可能得以执行。
	return TRUE;	
}

//检测状态
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
//线程调用的函数WriteChar，把缓冲里的数据写到串口中，期间调用WriteFile
void CSerialPortPC::WriteChar(CSerialPortPC* port)
{
	BOOL bWrite = TRUE;
	BOOL bResult = TRUE;
	DWORD BytesSent = 0;
	ResetEvent(port->m_hWriteEvent);//复位写事件句柄
	
	// Gain ownership of the critical section
	EnterCriticalSection(&port->m_csCommunicationSync);
	if (bWrite)
	{
		// Initailize variables
		port->m_ov.Offset = 0;
		port->m_ov.OffsetHigh = 0;

		// Clear buffer
		PurgeComm(port->m_hComm, PURGE_RXCLEAR | PURGE_TXCLEAR | PURGE_RXABORT | PURGE_TXABORT);
		//串口写入  
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

//	::SendMessage((port->m_pOwner)->m_hWnd, WM_COMM_TXEMPTY_DETECTED,0,(LPARAM) port->m_nPortNr);//检测是否发送完毕^_^
}

//
// Character received. Inform the owner
//接收字符
//读取数据是一个异步操作，当有数据发来时，会触发读事件m_ov.hEvent，监视线程捕捉到事件后并获知是读事件，进入相关读处理，这里调用函数ReceiveChar
//，ReceiveChar中调用ReadFile函数将串口数据读到Buffer缓冲中，相关代码如下：
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
		//更新COMSTAT  
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
		//所有字符均被读出，中断循环
		if (comstat.cbInQue == 0)
		{
			// break out when all bytes have been read
			break;
		}
						
		EnterCriticalSection(&port->m_csCommunicationSync);

		if (bRead)
		{
			//串口读出，读出缓冲区中字节  
			bResult = ReadFile(port->m_hComm,		// Handle to COMM port 指向标识的句柄
							   &RXBuff,				// RX Buffer Pointer指向一个缓冲区
							   1,					// Read one byte读取的字节数
							   &BytesRead,			// Stores number of bytes read指向调用函数读出的字节数
							   &port->m_ov);		// pointer to the m_ov structure一个OVERLAPPED结构
			// deal with the error code若返回错误，处理之
			if (!bResult)  
			{ 
				switch (dwError = GetLastError()) 
				{ 
					case ERROR_IO_PENDING: 	
						{ 
							// asynchronous i/o is still in progress异步IO仍在进行  
							// Proceed on to GetOverlappedResults();
							bRead = FALSE;
							break;
						}
					default:
						{	//其他错误处理 
							// Another error has occured.  Process this error.
						//	port->ProcessErrorMessage("ReadFile()");
							break;
						} 
				}
			}
			else
			{	//ReadFile返回TRUE 
				// ReadFile() returned complete. It is not necessary to call GetOverlappedResults()
				bRead = TRUE;
			}
		}  // close if (bRead)
		//异步IO操作仍在进行，需要调用GetOverlappedResult查询
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
	SetEvent(m_hWriteEvent);//通知辅助线程有数据需要发送
}


void CSerialPortPC::WriteToPort(char* string,int n)
{		
	assert(m_hComm != 0);
	VERIFY(m_hComm != 0);

	memset(m_szWriteBuffer, 0, sizeof(m_szWriteBuffer));
	memcpy(m_szWriteBuffer, string, n);
	m_nWriteSize=n;

	// set event for write
	SetEvent(m_hWriteEvent);//通知辅助线程有数据需要发送
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
	m_nWriteSize=m_nWriteSize+1;//strlen计数字符串直到碰到第一个字符串结束符“\0”为止,+1是因为加了0x0d
	// set event for write
	SetEvent(m_hWriteEvent);//通知辅助线程有数据需要发送
}

void CSerialPortPC::WriteToPort(LPCTSTR string,int n)
{		
	assert(m_hComm != 0);//若发现没有串口则终止程序
	VERIFY(m_hComm != 0);
	memset(m_szWriteBuffer, 0, sizeof(m_szWriteBuffer));//清空缓冲区
	memcpy(m_szWriteBuffer, string, n);//从string中拷贝n个字节到m_szWriteBuffer
	m_nWriteSize=n;
	// set event for write
	SetEvent(m_hWriteEvent);//通知辅助线程有数据需要发送
}
int	CSerialPortPC::GetPortNum()
{
	return m_nPortNr;
}
//
// Return the device control block
//控制串口监视线程:获取DCB
DCB CSerialPortPC::GetDCB()
{
	return m_dcb;
}

//
// Return the communication event masks
//控制串口监视线程:获取事件
DWORD CSerialPortPC::GetCommEvents()
{
	return m_dwCommEvents;
}

//
// Return the output buffer size
//控制串口监视线程:获取写缓冲大小
DWORD CSerialPortPC::GetWriteBufferSize()
{
	return m_nWriteBufferSize;
}

//关闭串口
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
