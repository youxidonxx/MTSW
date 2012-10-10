#include "stdafx.h"
#include "CSerial.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CSerial
CSerial::CSerial()
{
	m_hComm	= INVALID_HANDLE_VALUE;
}

CSerial::~CSerial()
{
	CloseComm();
}
BOOL	CSerial::OpenComm(int nPort,LONG32 nPaud,char parity /* =  */, 
						  DWORD dwCommEvents /* = EV_RXCHAR */,UINT databits /* = 8 */, UINT stopbits /* = 1 */)
{
	COMMTIMEOUTS	tous;
	DCB		dcb;
	CString		strComm;
	memset(&tous,0,sizeof(tous));
	tous.ReadIntervalTimeout = MAXWORD;
	tous.ReadTotalTimeoutMultiplier       =       1000       ;   

	tous.ReadTotalTimeoutConstant       =       1000       ;   

	tous.WriteTotalTimeoutMultiplier       =       1000;   

	tous.WriteTotalTimeoutConstant       =       1000       ;   

	if (nPort >=10)
	{
		strComm.Format(_T("\\\\.\\%d"),nPort);
	}
	else
		strComm.Format(_T("COM%d"),nPort);

	m_hComm = CreateFile(strComm,GENERIC_READ|GENERIC_WRITE,0,
		NULL,OPEN_EXISTING,0,NULL);
	if (m_hComm!=INVALID_HANDLE_VALUE)
	{
		GetCommState(m_hComm,&dcb);
		dcb.BaudRate = nPaud;
		dcb.Parity = parity;
		dcb.fOutxCtsFlow = dcb.fOutxDsrFlow = 0;
		dcb.fOutX = dcb.fInX = 0;
		dcb.fRtsControl = RTS_CONTROL_ENABLE;
		dcb.fDtrControl = DTR_CONTROL_ENABLE;

		dcb.ByteSize = databits;
		dcb.StopBits = stopbits;

		dcb.XonLim = 512;
		dcb.XoffLim = 1;

		SetCommState(m_hComm,&dcb);
		SetupComm(m_hComm,2048,2048);
		SetCommTimeouts(m_hComm,&tous);
		EscapeCommFunction(m_hComm,SETDTR);
		EscapeCommFunction(m_hComm,SETRTS);		

		PurgeComm(m_hComm, PURGE_RXCLEAR | PURGE_TXCLEAR | PURGE_RXABORT | PURGE_TXABORT);
		m_nPaud = nPaud;
		m_nPort = nPort;
		return TRUE;
	}
	return FALSE;
}
int CSerial::WriteComm(BYTE ch,int delay)
{
	return(WriteCommData(&ch,1,delay));
}
DWORD	CSerial::Write(const void* lpBuf,DWORD dwCount)
{
	if(!IsOpen())
		return -1;
	COMSTAT	stat;
	DWORD	dwError;
	ClearCommError(m_hComm,&dwError,&stat);

	DWORD dwBytesWritten = 0;
	if (!WriteFile(m_hComm, lpBuf, dwCount, &dwBytesWritten,NULL))
	{
		DWORD err = ::GetLastError();
		TRACE(_T("CSerialPort::Write, Failed in call to WriteFile,Error code:%d\n",err));
		return -1;
	}
	return	dwBytesWritten;
}
int CSerial::WriteCommData(BYTE* buff,int len,int delay /* = 1 */,int flag /* = 0 */)
{
	int i=0;
	unsigned long number = 0;
	COMSTAT	stat;
	DWORD	dwError;
	ClearCommError(m_hComm,&dwError,&stat);
 	if (flag == 0)
	{
		if(delay!=0)
			Sleep(delay);
		WriteFile(m_hComm,buff,len,&number,NULL);
		return ((int)number);
	}
	//Ϊ��һ���ֽ�һ���ֽڵ�д��
	else
	{
		for (i=0;i<len;i++)
		{
			if(delay!=0)
				Sleep(delay);
			WriteFile(m_hComm,buff+i,1,&number,NULL);
			//������д������ʱ���˳�ѭ��
			if(number<1)
				break;
		}
		return i;
	}
}
//���ݽӿڶ����Ѿ���֪��Ҫ��ȡ�����ݴ�С
int CSerial::ReadCommData(BYTE* buff,int len,int timeout)
{
	DWORD	start;
	COMSTAT	stat;
	DWORD	dwError;

	int i=0;
	unsigned long number = 0;
	start = GetTickCount();
 	do 
	{
		//���COMM��־
		int err = GetLastError();
		ClearCommError(m_hComm,&dwError,&stat);
		//����һ��readfile�����޷���ȫ��ȡ����Ҫ�����ݣ�
		//�ʶԸò����ĳ��Ƚ����޶����ж�
		ReadFile(m_hComm,buff+i,len-i,&number,NULL);
		//���Ǹô˶�ȡ���������ݴ�С��ʣ����Ҫ�Ĵ�С��ͬ����˵���Ѿ���ȡ���
		if(number == (unsigned long)(len-i))
			return len;
		//���ǻ����Զ�ȡ�����ݣ�˵�����У�����ȡ���������ݽ��е���
		else if(number>0)
			i+=number;
		else if(timeout>5)
			Sleep(1);
	} while (GetTickCount()-start <(unsigned int)timeout);
	return i;
}
int CSerial::ReadCommData(int timeout)
{
	BYTE	ch;
	DWORD	start;
	COMSTAT	stat;
	DWORD	dwError;
	unsigned long	number = 0;
// 	COMSTAT	stat;
// 	DWORD	dwError;
// 	ClearCommError(m_hComm,&dwError,&stat);
	start = GetTickCount();
 	do 
	{
		int err = GetLastError();

		ClearCommError(m_hComm,&dwError,&stat);
		//һ���ֽ�һ���ֽڽ��ж�ȡ
		ReadFile(m_hComm,&ch,1,&number,NULL);
		//˵���������Ǵ������ݵ�
		if(number>0)
			return (int)ch;
	} while (GetTickCount()-start <(unsigned int)timeout);
	return -1;
}
DWORD	CSerial::Read(void* lpBuf,DWORD dwCount)
{
	if(!IsOpen())
		return -1;
	DWORD dwBytesRead = 0;
	COMSTAT	stat;
	DWORD	dwError;
	ClearCommError(m_hComm,&dwError,&stat);
	if (!ReadFile(m_hComm, lpBuf, dwCount, &dwBytesRead, NULL))
	{
		TRACE(_T("CSerialPort::Read, Failed in call to ReadFile\n"));
		return -1;
	}
	return dwBytesRead;
}
void CSerial::CloseComm()
{
	if (m_hComm!=INVALID_HANDLE_VALUE)
	{
		Sleep(5);
		CloseHandle(m_hComm);
		m_hComm = INVALID_HANDLE_VALUE;
	}
}
BOOL	CSerial::IsOpen()
{
	return (m_hComm!=INVALID_HANDLE_VALUE);
}
