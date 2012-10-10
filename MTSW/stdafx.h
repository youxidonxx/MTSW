// stdafx.h : 标准系统包含文件的包含文件，
// 或是经常使用但不常更改的
// 特定于项目的包含文件

#pragma once

#ifndef _SECURE_ATL
#define _SECURE_ATL 1
#endif

#ifndef VC_EXTRALEAN
#define VC_EXTRALEAN		// 从 Windows 头中排除极少使用的资料
#endif

// 如果您必须使用下列所指定的平台之前的平台，则修改下面的定义。
// 有关不同平台的相应值的最新信息，请参考 MSDN。
#ifndef WINVER				// 允许使用特定于 Windows XP 或更高版本的功能。
#define WINVER 0x0501		// 将此值更改为相应的值，以适用于 Windows 的其他版本。
#endif

#ifndef _WIN32_WINNT		// 允许使用特定于 Windows XP 或更高版本的功能。
#define _WIN32_WINNT 0x0501	// 将此值更改为相应的值，以适用于 Windows 的其他版本。
#endif						

#ifndef _WIN32_WINDOWS		// 允许使用特定于 Windows 98 或更高版本的功能。
#define _WIN32_WINDOWS 0x0410 // 将它更改为适合 Windows Me 或更高版本的相应值。
#endif

#ifndef _WIN32_IE			// 允许使用特定于 IE 6.0 或更高版本的功能。
#define _WIN32_IE 0x0600	// 将此值更改为相应的值，以适用于 IE 的其他版本。值。
#endif

#define _ATL_CSTRING_EXPLICIT_CONSTRUCTORS	// 某些 CString 构造函数将是显式的

// 关闭 MFC 对某些常见但经常可放心忽略的警告消息的隐藏
#define _AFX_ALL_WARNINGS

#include <afxwin.h>         // MFC 核心组件和标准组件
#include <afxext.h>         // MFC 扩展


#include <afxdisp.h>        // MFC 自动化类



#ifndef _AFX_NO_OLE_SUPPORT
#include <afxdtctl.h>		// MFC 对 Internet Explorer 4 公共控件的支持
#endif
#ifndef _AFX_NO_AFXCMN_SUPPORT
#include <afxcmn.h>			// MFC 对 Windows 公共控件的支持
#endif // _AFX_NO_AFXCMN_SUPPORT
// #include "GdiplusTypes.h" 

#pragma		comment(lib,"gdiplus.lib")
#include <GdiPlus.h>
using namespace Gdiplus;


 
#include "MTSW.h"
#include "MTSWDlg.h"
#include "AddBox.h"
#include "cellrange.h"
// #include "CSerial.h"
#include "MySerial.h"
#include "SerialPort.h"
#include "pcserialport.h"
#include "CommonSetDlg.h"
#include "AnalogSetPage.h"
#include "DigitSetDlg.h"
#include "ComSetDlg.h"
#include "GridCtrl.h"
 #include "SpanEdit.h"
#include "spmsgid.h"
#include "spstruct.h"
#include "sysmsgdef.h"
#include "typedef.h"
#include <afx.h>

extern	void	OutputDebugInfo(int	nInfo,CString	strInfo);
enum	CommonValues
{
	COMMON_BATLOWTHRD = 0X00,						//0-1023
	COMMON_VCCN		= COMMON_BATLOWTHRD+2,
	COMMON_VOX1OPEN	= COMMON_VCCN+2,
	COMMON_VOX5OPEN	= COMMON_VOX1OPEN+2,
	COMMON_MAXVOL		= COMMON_VOX5OPEN+2,		//0-127
	COMMON_CAL_FREQNUM	=	COMMON_MAXVOL+2	//2-5
};
#define		GRID_COLUMN_WIDTH			220

#define		STRUCT_LEN_CALCTCCHAL		50
#define		DIGIT_ANA_SETTING_NUM		23
#define		COMMON_SETTING_NUM		6
#define		FREQ_NUM_LEN				4

#define		MAX_FREQ_NUM_LEN			9

#define		MIN_FREQ_NUM				2
#define		MAX_FREQ_NUM				5

enum		ERR_LIST
{
		ERR_WRITE_FAILED	=	-1,
		ERR_READ_FAILED	=	-2,
		ERR_DATA_LEN_INVALID	=	-3
};
enum	MaxValues
{
	MIN_MAX_VALUE	=		127,
	MIDDLE_MAX_VALUE	=	1023,
	MAX_MAX_VALUE		=	32767
};
enum	DigitValues
{
	DIGIT_CAL_TXFREQ	= 0X00,
	DIGIT_CAL_RXFREQ	= DIGIT_CAL_TXFREQ+4,
	DIGIT_CAL_TXHIGHPC	=	DIGIT_CAL_RXFREQ+4,		//0-1023
	DIGIT_CAL_TXLOWPC		=	DIGIT_CAL_TXHIGHPC+2,
	DIGIT_TXMOD1_FACTOR	=	DIGIT_CAL_TXLOWPC+2,		//0-32767
	DIGIT_TXMOD2_FACTOR	=	DIGIT_TXMOD1_FACTOR+2,
	ANALOG_TXMOD1_FACTOR_AF	=	DIGIT_TXMOD2_FACTOR+2,
	ANALOG_TXMOD2_FACTOR_AF	=	ANALOG_TXMOD1_FACTOR_AF+2,
	ANALOG_TXMOD1_FACTOR_CTC0	=	ANALOG_TXMOD2_FACTOR_AF+2,
	ANALOG_TXMOD2_FACTOR_CTC0	=	ANALOG_TXMOD1_FACTOR_CTC0+2,
	ANALOG_TXMOD1_FACTOR_CTC1	=	ANALOG_TXMOD2_FACTOR_CTC0+2,
	ANALOG_TXMOD2_FACTOR_CTC1	=	ANALOG_TXMOD1_FACTOR_CTC1+2,
	ANALOG_TXMOD1_FACTOR_CTC2	=	ANALOG_TXMOD2_FACTOR_CTC1+2,
	ANALOG_TXMOD2_FACTOR_CTC2	=	ANALOG_TXMOD1_FACTOR_CTC2+2,

	ANALOG_TXMOD1_FACTOR_CDC		=	ANALOG_TXMOD2_FACTOR_CTC2+2,
	ANALOG_TXMOD2_FACTOR_CDC		=	ANALOG_TXMOD1_FACTOR_CDC+2,

	RX_CAL_TV	=	ANALOG_TXMOD2_FACTOR_CDC+2,			//0-1023
	RX_NOISE_1_OPEN	=	RX_CAL_TV+2,
	RX_NOISE_1_CLOSE	=	RX_NOISE_1_OPEN+2,
	RX_NOISE_9_OPEN	=	RX_NOISE_1_CLOSE+2,
	RX_NOISE_9_CLOSE	=	RX_NOISE_9_OPEN+2,
	RX_RSSI_1_THRD		=	RX_NOISE_9_CLOSE+2,
	RX_RSSI_5_THRD		=	RX_RSSI_1_THRD+2
};






#ifdef _UNICODE
#if defined _M_IX86
#pragma comment(linker,"/manifestdependency:\"type='win32' name='Microsoft.Windows.Common-Controls' version='6.0.0.0' processorArchitecture='x86' publicKeyToken='6595b64144ccf1df' language='*'\"")
#elif defined _M_IA64
#pragma comment(linker,"/manifestdependency:\"type='win32' name='Microsoft.Windows.Common-Controls' version='6.0.0.0' processorArchitecture='ia64' publicKeyToken='6595b64144ccf1df' language='*'\"")
#elif defined _M_X64
#pragma comment(linker,"/manifestdependency:\"type='win32' name='Microsoft.Windows.Common-Controls' version='6.0.0.0' processorArchitecture='amd64' publicKeyToken='6595b64144ccf1df' language='*'\"")
#else
#pragma comment(linker,"/manifestdependency:\"type='win32' name='Microsoft.Windows.Common-Controls' version='6.0.0.0' processorArchitecture='*' publicKeyToken='6595b64144ccf1df' language='*'\"")
#endif
#endif


