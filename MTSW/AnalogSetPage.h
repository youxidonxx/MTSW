#pragma once


// CAnalogSetPage �Ի���

class CAnalogSetPage : public CDialog
{
	DECLARE_DYNAMIC(CAnalogSetPage)

public:
	CAnalogSetPage(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~CAnalogSetPage();

// �Ի�������
	enum { IDD = IDD_DLG_ANALOGSET };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

	DECLARE_MESSAGE_MAP()
};
