// MySerial.cpp : 实现文件
//

#include "stdafx.h"
#include "MTSW.h"
#include "MySerial.h"


// CMySerial

CMySerial::CMySerial()
{
	m_hComm = INVALID_HANDLE_VALUE;
	m_hWriteEvent = NULL;
	m_bWrite = false;
	m_bRead = false;
	ZeroMemory(&m_ov,sizeof(OVERLAPPED));
}

CMySerial::~CMySerial()
{
	CloseHandle(m_hComm);
	m_hComm = INVALID_HANDLE_VALUE;
}


// CMySerial 成员函数
bool	CMySerial::InitComm(int nComm,int nPaud,int nParity /* =  */,
							DWORD dwEvent /* = EV_RXCHAR */,UINT databits /* = 8 */,UINT stopbits /* = 1 */)
{
	VERIFY(nComm > 0 && nComm < 100);//增加串口号		assert计算表达式如果其值为假（即为0），会调用abort来终止程序运行
 
	if (m_ov.hEvent != NULL)
		ResetEvent(m_ov.hEvent);
	else
		m_ov.hEvent = CreateEvent(NULL,true,false,NULL);

	if(m_hWriteEvent!=NULL)
		ResetEvent(m_hWriteEvent);
	else
		m_hWriteEvent = CreateEvent(NULL,true,false,NULL);

	m_nPaud = nPaud;
	m_nPort = nComm;
	CString		strComm;
	if (nComm >=10)
	{
		strComm.Format(_T("\\\\.\\%d"),m_nPort);
	}
	else
		strComm.Format(_T("COM%d"),m_nPort);

	m_hComm = CreateFile(
		strComm,
		GENERIC_READ|GENERIC_WRITE,0,
		NULL,OPEN_EXISTING,
		0,NULL);
	if (m_hComm == INVALID_HANDLE_VALUE)
	{
		return false;
	}
	DCB		dcb;
	COMMTIMEOUTS	tous;
	DWORD	dwInQue,dwOutQue;
	dwInQue = dwOutQue = 2048;

	memset(&tous,0,sizeof(tous));
	tous.ReadIntervalTimeout = 1000;
	tous.ReadTotalTimeoutMultiplier = 1000;
	tous.ReadTotalTimeoutConstant = 1000;
	tous.WriteTotalTimeoutMultiplier = 1000;
	tous.WriteTotalTimeoutConstant = 1000;

	if (!SetCommTimeouts(m_hComm,&tous))
	{
		return false;
	}
	if (!SetCommMask(m_hComm,dwEvent))
	{
		return false;
	}

	memset(&dcb,0,sizeof(DCB));
	GetCommState(m_hComm,&dcb);
	dcb.BaudRate = nPaud;
	dcb.DCBlength = sizeof(DCB);
	dcb.Parity = nParity;
	dcb.StopBits = stopbits;
	dcb.ByteSize = databits;
	dcb.fDtrControl = DTR_CONTROL_ENABLE;
	dcb.fRtsControl = RTS_CONTROL_ENABLE;
	dcb.fDsrSensitivity = false;

	if (!SetCommState(m_hComm,&dcb))
	{
		return false;
	}
	SetupComm(m_hComm,dwInQue,dwOutQue);
	PurgeComm(m_hComm, PURGE_RXCLEAR | PURGE_TXCLEAR | PURGE_RXABORT | PURGE_TXABORT);

	return true;
}
bool	CMySerial::IsOpen()
{
	return m_hComm!=INVALID_HANDLE_VALUE;
}
bool	CMySerial::Read(void* lpBuf,DWORD dwCount)
{
	m_bRead = true;
	bool	bResult = true;
	DWORD	dwErr = 0;
	DWORD	dwByteToRead = 0;
	COMSTAT		comstat;
	for (;;)
	{
		bResult = ClearCommError(m_hComm,&dwErr,&comstat);
		if(comstat.cbInQue == 0)
			continue;
		if (m_bRead)
		{
			bResult = ReadFile(m_hComm,
				lpBuf,
				dwCount,
				&dwByteToRead,
				&m_ov);
			if (!bResult)
			{
				switch (dwErr = GetLastError())
				{
				case ERROR_IO_PENDING:
					{
						m_bRead = false;
						break;
					}
				default:
					break;
				}
			}
			else
				m_bRead = true;
		}
		if (!m_bRead)
		{
			m_bRead = true;
			bResult = GetOverlappedResult(m_hComm,
				&m_ov,
				&dwByteToRead,
				true);
		}
	}
}
bool	CMySerial::Write(const void* lpBuf,DWORD dwCount)
{
	bool	bWrite = true;
	bool	bResult = true;
	DWORD	dwByteToWrite = 0;

	ResetEvent(m_hWriteEvent);
	if (bWrite)
	{
		m_ov.Offset = 0;
		m_ov.OffsetHigh = 0;

		bResult = WriteFile(m_hComm,
			lpBuf,
			dwCount,
			&dwByteToWrite,
			&m_ov);
		if (!bResult)
		{
			DWORD	dwErr = GetLastError();
			switch(dwErr)
			{
			case ERROR_IO_PENDING:
				dwByteToWrite = 0;
				bWrite = false;
				break;
			default:
				break;
			}
		}
	}
	if (!bWrite)
	{
		bWrite = true;
		bResult = GetOverlappedResult(m_hComm,
			&m_ov,
			&dwByteToWrite,
			true);
	}
	if (dwByteToWrite == dwCount)
	{
		return true;
	}
	return false;
}