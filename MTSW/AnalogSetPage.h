#pragma once


// CAnalogSetPage �Ի���

class CAnalogSetPage : public CDialog
{
	DECLARE_DYNAMIC(CAnalogSetPage)

protected:
	BYTE*	m_pData;
	CWnd*	m_pOwner;
public:
	CAnalogSetPage(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~CAnalogSetPage();


	void	LoadData();
// �Ի�������
	enum { IDD = IDD_DLG_ANALOGSET };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

	DECLARE_MESSAGE_MAP()
};
