/******************************************
由于只是在需要时打开串口读取/写入数据，故不采用
普遍的串口类，只用一个简单的串口类进行操作,
根据接口定义，不需要额外的数据，对每一组数据都有
固定的模式，不采用普遍的串口类，因为那些类对于未知长度
未知数据的类型更适合，省略去对串口数据的获取方式，专注
于对数据的赋值，减少工作量
******************************************/
#pragma once

class	CSerial:public CObject
{
public:
	CSerial();
	virtual	~CSerial();
	BOOL	OpenComm(int nPort,LONG32	nPaud,char parity = 'N',
		DWORD dwCommEvents = EV_RXCHAR,UINT databits = 8, UINT stopbits = 1);
	BOOL	IsOpen();
	void	CloseComm();
	int		WriteComm(BYTE ch,int delay);
	int		WriteCommData(BYTE*	buff,int	len,int delay = 1,int flag = 0);
	int		ReadCommData(BYTE*	buff,int len,int timeout);
	int		ReadCommData(int  timeout);

	int		m_nPort;
	int		m_nPaud;
private:
	//int		m_hComm;
	HANDLE	m_hComm;
};