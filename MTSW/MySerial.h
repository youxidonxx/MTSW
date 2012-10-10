#pragma once

// CMySerial ÃüÁîÄ¿±ê

class CMySerial : public CObject
{
public:
	CMySerial();
	virtual ~CMySerial();
	bool	InitComm(int	nComm,int	nPaud,int	nParity = 'N',
		DWORD	dwEvent = EV_RXCHAR,UINT	databits = 8,UINT	stopbits = 1);
	bool	IsOpen();
	void	CloseComm();

	bool	Read(void*	lpBuf,DWORD	dwCount);
	bool	Write(const	void*	lpBuf,DWORD	dwCount);

	int		m_nPort;
	int		m_nPaud;

private:
	bool	m_bRead;
	bool	m_bWrite;
	HANDLE	m_hComm;
	HANDLE	m_hWriteEvent;
	OVERLAPPED		m_ov;
};


