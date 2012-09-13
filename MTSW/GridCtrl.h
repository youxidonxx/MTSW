#if !defined(AFX_GRIDCTRL_H__519FA702_722C_11D1_ABBA_00A0243D1382__INCLUDED_)
#define AFX_GRIDCTRL_H__519FA702_722C_11D1_ABBA_00A0243D1382__INCLUDED_

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000

/////////////////////////////////////////////////////////////////////////////
// GridCtrl.h : header file
//
// MFC Grid Control header file
//
// Written by Chris Maunder (chrismaunder@codeguru.com)
// Copyright (c) 1998.
//
// The code contained in this file is based on the original
// WorldCom Grid control written by Joe Willcoxson,
//        E-mail:  chinajoe@aol.com
//        URL:  http://users.aol.com/chinajoe
//
// This code may be used in compiled form in any way you desire. This
// file may be redistributed unmodified by any means PROVIDING it is 
// not sold for profit without the authors written consent, and 
// providing that this notice and the authors name and all copyright 
// notices remains intact. If the source code in this file is used in 
// any  commercial application then a statement along the lines of 
// "Portions copyright (c) Chris Maunder, 1998" must be included in 
// the startup banner, "About" box or printed documentation. An email 
// letting me know that you are using it would be nice as well. That's 
// not much to ask considering the amount of work that went into this.
//
// This file is provided "as is" with no expressed or implied warranty.
// The author accepts no liability for any damage/loss of business that
// this product may cause.
//
// Expect bugs!
// 
// Please use and enjoy. Please let me know of any bugs/mods/improvements 
// that you have found/implemented and I will fix/incorporate them into this
// file. 
//
/////////////////////////////////////////////////////////////////////////////

#define GRIDCONTROL_USE_TITLETIPS       // Use titletips for cells with large data

#include "CellRange.h"
#include <afxtempl.h>

// Use this as the classname when inserting this control as a custom control
// in the MSVC++ dialog editor
#define GRIDCTRL_CLASSNAME _T("MFCGridCtrl")

#define IsSHIFTpressed() ( (GetKeyState(VK_SHIFT) & (1 << (sizeof(SHORT)*8-1))) != 0   )
#define IsCTRLpressed()  ( (GetKeyState(VK_CONTROL) & (1 << (sizeof(SHORT)*8-1))) != 0 )

// Used for Get/SetItem calls.
typedef struct _GV_ITEM { 
    int      row,col;     // Row and Column of item
    UINT     mask;        // Mask for use in getting/setting cell data
    UINT     state;       // cell state (focus/hilighted etc)
    UINT     nFormat;     // Format of cell
    CString  szText;      // Text in cell
    int      iImage;      // index of the list view item抯 icon
    COLORREF crBkClr;     // Background colour (or CLR_DEFAULT)
    COLORREF crFgClr;     // Forground colour (or CLR_DEFAULT)
    LPARAM   lParam;      // 32-bit value to associate with item 
    LOGFONT  lfFont;      // Cell font
} GV_ITEM; 

// Grid line selection
#define GVL_NONE                0
#define GVL_HORZ                1
#define GVL_VERT                2
#define GVL_BOTH                3

// Cell data mask
#define GVIF_TEXT               LVIF_TEXT
#define GVIF_IMAGE              LVIF_IMAGE
#define GVIF_PARAM              LVIF_PARAM
#define GVIF_STATE              LVIF_STATE
#define GVIF_BKCLR              (GVIF_STATE<<1)
#define GVIF_FGCLR              (GVIF_STATE<<2)
#define GVIF_FORMAT             (GVIF_STATE<<3)
#define GVIF_FONT               (GVIF_STATE<<4)

// Cell states
#define GVIS_FOCUSED            0x0001
#define GVIS_SELECTED           0x0002
#define GVIS_DROPHILITED        0x0004
#define GVIS_READONLY           0x0008

// Cell Searching options
#define GVNI_FOCUSED            0x0001
#define GVNI_SELECTED           0x0002
#define GVNI_DROPHILITED        0x0004
#define GVNI_READONLY           0x0008

#define GVNI_ABOVE              LVNI_ABOVE
#define GVNI_BELOW              LVNI_BELOW
#define GVNI_TOLEFT             LVNI_TOLEFT
#define GVNI_TORIGHT            LVNI_TORIGHT
#define GVNI_ALL                (LVNI_BELOW|LVNI_TORIGHT)

// Hit test values (not yet implemented)
#define GVHT_DATA               0x0000
#define GVHT_TOPLEFT            0x0001
#define GVHT_COLHDR             0x0002
#define GVHT_ROWHDR             0x0004
#define GVHT_COLSIZER           0x0008
#define GVHT_ROWSIZER           0x0010
#define GVHT_LEFT               0x0020
#define GVHT_RIGHT              0x0040
#define GVHT_ABOVE              0x0080
#define GVHT_BELOW              0x0100

typedef struct tagGV_DISPINFO { 
    NMHDR   hdr; 
    GV_ITEM item;
	RECT	rect;
} GV_DISPINFO;

// Messages sent to the grid's parent (More will be added in future)
#define	GVN_BEGINDRAG           LVN_BEGINDRAG        // LVN_FIRST-9
#define	GVN_BEGINLABELEDIT      LVN_BEGINLABELEDIT   // LVN_FIRST-5
#define	GVN_BEGINRDRAG          LVN_BEGINRDRAG
#define GVN_COLUMNCLICK         LVN_COLUMNCLICK
#define	GVN_DELETEITEM          LVN_DELETEITEM
#define GVN_ENDLABELEDIT        LVN_ENDLABELEDIT     // LVN_FIRST-6
#define GVN_SELCHANGING         LVN_BEGINLABELEDIT+1 // LVN_FIRST-4
#define GVN_SELCHANGED          LVN_BEGINLABELEDIT+2 // LVN_FIRST-3



// Each cell contains one of these. Fields "row" and "column" are not stored since we
// will usually have acces to them in other ways, and they are an extra 8 bytes per
// cell that is probably unnecessary.

class CGridCell : public CObject
{
public:
    CGridCell() 
    {
        state = 0;
        nFormat = 0;
        szText.Empty();
        iImage = -1;
        lParam = 0;
		crBkClr = CLR_DEFAULT;
		crFgClr = CLR_DEFAULT;
    }

    UINT     state;       // Cell state (selected/focus etc)
    UINT     nFormat;     // Cell format
    CString  szText;      // Cell text (or binary data if you wish...)
    int      iImage;      // Index of the list view item抯 icon 
    COLORREF crBkClr;     // Background colour (or CLR_DEFAULT)
    COLORREF crFgClr;     // Forground colour (or CLR_DEFAULT)
    LPARAM   lParam;      // 32-bit value to associate with item
    LOGFONT  lfFont;      // Cell font
}; 


// storage typedef for each row in the grid
typedef CTypedPtrArray<CObArray, CGridCell*> GRID_ROW;

// DDX_GridControl is used where a DDX_Control call is needed. In some strange
// situations the usual DDX_Control does not result in CGridCtrl::SubclassWindow
// or CGridCtrl::PreSubclassWindow being called. Using this version calls 
// CGridCtrl::SubclassWindow directly - ensuring that cell metrics are set properly

class CGridCtrl;
void AFXAPI DDX_GridControl(CDataExchange* pDX, int nIDC, CGridCtrl& rControl);

/////////////////////////////////////////////////////////////////////////////
// CGridCtrl window

class CGridCtrl : public CWnd
{
// Construction
public:
    CGridCtrl(int nRows = 0, int nCols = 0, int nFixedRows = 0, int nFixedCols = 0);
    DECLARE_DYNCREATE(CGridCtrl)

    BOOL Create(const RECT& rect, CWnd* parent, UINT nID,
                DWORD dwStyle = WS_CHILD | WS_BORDER | WS_TABSTOP | WS_VISIBLE);
    BOOL SubclassWindow(HWND hWnd);

// Attributes
public:
	int		GetRowCount() const						{ return m_nRows; }
	int		GetColumnCount() const					{ return m_nCols; }
	int		GetFixedRowCount() const				{ return m_nFixedRows; }
	int		GetFixedColumnCount() const				{ return m_nFixedCols; }
	BOOL	SetRowCount(int nRows = 10);
	BOOL	SetColumnCount(int nCols = 10);
	BOOL	SetFixedRowCount(int nFixedRows = 1);
	BOOL	SetFixedColumnCount(int nFixedCols = 1);

	int		GetRowHeight(int nRow) const;
	BOOL	SetRowHeight(int row, int height);
	int		GetColumnWidth(int nCol) const;
	BOOL	SetColumnWidth(int col, int width);

	BOOL	GetCellOrigin(int nRow, int nCol, LPPOINT p) const;
	BOOL	GetCellOrigin(const CCellID& cell, LPPOINT p) const;
	BOOL	GetCellRect(int nRow, int nCol, LPRECT pRect) const;
	BOOL	GetCellRect(const CCellID& cell, LPRECT pRect) const;

	int		GetFixedRowHeight() const;
	int		GetFixedColumnWidth() const;
	long	GetVirtualWidth() const;
	long	GetVirtualHeight() const;

	int		GetSelectedCount() const				{ return m_SelectedCellMap.GetCount(); }

	CCellID	GetFocusCell() const					{ return m_idCurrentCell;           }

	void	SetImageList(CImageList* pList)			{ m_pImageList = pList;             }
	void	SetGridLines(int nWhichLines = GVL_BOTH)	{ m_nGridLines = nWhichLines; 
                                                    if (::IsWindow(GetSafeHwnd())) Invalidate(); }
	void	SetEditable(BOOL bEditable = TRUE)		{ m_bEditable = bEditable;			}
	BOOL	IsEditable() const						{ return m_bEditable;				}
	void	SetEditState(BOOL bEdit)				{ m_bEdit = bEdit;					}
	void	EnableSelection(BOOL bEnable = TRUE)		{ ResetSelectedRange(); m_bEnableSelection = bEnable; ResetSelectedRange(); }
    BOOL	IsSelectable() const                     { return m_bEnableSelection;        }
    void	SetHeaderSort(BOOL bSortOnClick = TRUE)  { m_bSortOnClick = bSortOnClick;    }
    BOOL	GetHeaderSort() const                    { return m_bSortOnClick;            }
    void	SetHandleTabKey(BOOL bHandleTab = TRUE)  { m_bHandleTabKey = bHandleTab;     }
    BOOL	GetHandleTabKey() const                  { return m_bHandleTabKey;           }
    void	SetDoubleBuffering(BOOL bBuffer = TRUE)  { m_bDoubleBuffer = bBuffer;        }
    BOOL	GetDoubleBuffering() const               { return m_bDoubleBuffer;           }
    void	EnableTitleTips(BOOL bEnable = TRUE)     { m_bTitleTips = bEnable;           }
    BOOL	GetTitleTips()                           { return m_bTitleTips;              }

    BOOL	SetItem(const GV_ITEM* pItem);
    BOOL	GetItem(GV_ITEM* pItem);
    BOOL	SetItemText(int nRow, int nCol, LPCTSTR str);
    virtual CString GetItemText(int nRow, int nCol);
    BOOL	SetItemData(int nRow, int nCol, LPARAM lParam);
    LPARAM	GetItemData(int nRow, int nCol) const;
    BOOL	SetItemImage(int nRow, int nCol, int iImage);
    int		GetItemImage(int nRow, int nCol) const;
    BOOL	SetItemState(int nRow, int nCol, UINT state);
    UINT	GetItemState(int nRow, int nCol) const;
    BOOL	SetItemFormat(int nRow, int nCol, UINT nFormat);
    UINT	GetItemFormat(int nRow, int nCol) const;
	BOOL	SetItemBkColour(int nRow, int nCol, COLORREF cr = CLR_DEFAULT);
	COLORREF GetItemBkColour(int nRow, int nCol) const;
	BOOL	SetItemFgColour(int nRow, int nCol, COLORREF cr = CLR_DEFAULT);
	COLORREF GetItemFgColour(int nRow, int nCol) const;
	BOOL	SetItemFont(int nRow, int nCol, LOGFONT* lf);
	LOGFONT* GetItemFont(int nRow, int nCol) const;

// Operations
public:
    int  InsertColumn(LPCTSTR strHeading, UINT nFormat = DT_CENTER|DT_VCENTER|DT_SINGLELINE,
                      int nColumn = -1);
    int  InsertRow(LPCTSTR strHeading, int nRow = -1);
    BOOL DeleteColumn(int nColumn);
    BOOL DeleteRow(int nRow);
    BOOL DeleteNonFixedRows();
    BOOL DeleteAllItems();

    BOOL AutoSizeRow(int nRow);
    BOOL AutoSizeColumn(int nCol);
    void AutoSizeRows();
    void AutoSizeColumns();
    void AutoSize();
    void ExpandColumnsToFit();
    void ExpandRowsToFit();
    void ExpandToFit();

    CSize GetTextExtent(LPCTSTR str, BOOL bUseSelectedFont = TRUE);
    void EnsureVisible(CCellID &cell)       { EnsureVisible(cell.row, cell.col); }
    void EnsureVisible(int nRow, int nCol);
    BOOL IsCellVisible(int nRow, int nCol) const;
    BOOL IsCellVisible(CCellID cell) const;
    BOOL IsCellEditable(int nRow, int nCol) const;
    BOOL IsCellEditable(CCellID &cell) const;

    // SetRedraw stops/starts redraws on things like changing the # rows/columns
    // and autosizing, but not for user-intervention such as resizes
    void SetRedraw(BOOL bAllowDraw, BOOL bResetScrollBars = FALSE);
    BOOL RedrawCell(int nRow, int nCol, CDC* pDC = NULL);
    BOOL RedrawCell(const CCellID& cell, CDC* pDC = NULL);
    BOOL RedrawRow(int row);
    BOOL RedrawColumn(int col);

    CCellRange GetCellRange() const;
    CCellRange GetSelectedCellRange() const;
    void SetSelectedRange(const CCellRange& Range, BOOL bForceRepaint = FALSE);
    void SetSelectedRange(int nMinRow, int nMinCol, int nMaxRow, int nMaxCol,
                          BOOL bForceRepaint = FALSE);
    BOOL IsValid(int nRow, int nCol) const;
    BOOL IsValid(const CCellID& cell) const;
    BOOL IsValid(const CCellRange& range) const;

    // Clipboard and cut n' paste operations



    CCellID GetNextItem(CCellID& cell, int nFlags) const;

    BOOL SortTextItems(int nCol, BOOL bAscending);
    BOOL SortItems(PFNLVCOMPARE pfnCompare, int nCol, BOOL bAscending, LPARAM data = 0);

// Overrides
    // ClassWizard generated virtual function overrides
    //{{AFX_VIRTUAL(CGridCtrl)
	public:
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	protected:
    virtual void PreSubclassWindow();
	//}}AFX_VIRTUAL
    public:
// Implementation
public:
	void PrintColumnHeadings(CDC* pDC,CPrintInfo* pInfo);
	void PrintRow(CDC* pDC,CPrintInfo* pInfo,int nRow);

	void SetGridResize(BOOL bResizeRow = TRUE,BOOL bResizeCol = TRUE);
    virtual ~CGridCtrl();
	CCellID SetFocusCell(int nRow, int nCol);
protected:
    BOOL RegisterWindowClass();
    LRESULT SendMessageToParent(int nRow, int nCol, int nMessage);

    BOOL InvalidateCellRect(const CCellID& cell);
    BOOL InvalidateCellRect(const CCellRange& cellRange);
    void EraseBkgnd(CDC* pDC);

    BOOL GetCellRangeRect(const CCellRange& cellRange, LPRECT lpRect) const;

    CGridCell* GetCell(int nRow, int nCol) const;
    BOOL SetCell(int nRow, int nCol, CGridCell* pCell);

    CCellID GetCellFromPt(CPoint point, BOOL bAllowFixedCellCheck = TRUE) const;
    CCellID GetTopleftNonFixedCell() const;
    CCellRange GetUnobstructedNonFixedCellRange() const;
    CCellRange GetVisibleNonFixedCellRange(LPRECT pRect = NULL) const;

    CCellID SetFocusCell(CCellID cell);
   

    void ResetSelectedRange();
    void ResetScrollBars();
    int  GetScrollPos32(int nBar, BOOL bGetTrackPos = FALSE);
    BOOL SetScrollPos32(int nBar, int nPos, BOOL bRedraw = TRUE);

    BOOL SortTextItems(int nCol, BOOL bAscending, int low, int high);
    BOOL SortItems(PFNLVCOMPARE pfnCompare, int nCol, BOOL bAscending, LPARAM data, 
                   int low, int high);

// Overrrides
protected:
    // Printing    

    // Drag n' drop

    // Mouse Clicks
    virtual void  OnFixedColumnClick(CCellID& cell);
    virtual void  OnFixedRowClick(CCellID& cell);

    // Editing
    virtual CSize GetCellExtent(int nRow, int nCol, CDC* pDC);
    virtual void  OnEditCell(int nRow, int nCol, UINT nChar);

    // Drawing
    virtual void  OnDraw(CDC* pDC);
    virtual BOOL  DrawFixedCell(CDC* pDC, int nRow, int nCol, CRect rect, BOOL bEraseBk=FALSE);
    virtual BOOL  DrawCell(CDC* pDC, int nRow, int nCol, CRect rect, BOOL bEraseBk=FALSE);

    // GridCell Creation and Cleanup
    virtual CGridCell* CreateCell(int nRow, int nCol);
    virtual void EmptyCell(CGridCell* pCell, int nRow, int nCol);

// Attributes
protected:
    // General attributes
	COLORREF	m_crTextColour;			//文本颜色
	COLORREF	m_crTextBkColour;		//文本背景颜色
	COLORREF	m_crBkColour;			//格子颜色

	COLORREF	m_crFixedTextColour;	//固定格子的文本颜色
	COLORREF	m_crFixedBkColour;		//固定格子的背景颜色
    COLORREF    m_crGridColour;			//格子线的颜色

    COLORREF    m_crWindowText, m_crWindowColour, m_cr3DFace, m_crShadow;	// System colours
                    
	int			m_nGridLines;
	BOOL		m_bEditable;
	BOOL		m_bAllowDraw;
	BOOL		m_bEnableSelection;
	BOOL		m_bSortOnClick;
	BOOL		m_bHandleTabKey;
	BOOL		m_bDoubleBuffer;
	BOOL		m_bTitleTips;

	BOOL		m_bAllowRowResize;		//允许调节行尺寸
	BOOL		m_bAllowColResize;		//允许调节栏尺寸

	BOOL		m_bEdit;				//正在编辑
	CRect		m_rectEdit;				//所编辑的位置
    // Cell size details
    int			m_nRows;				//格子行数
	int			m_nFixedRows;			//固定格式的行数
	int			m_nCols;				//格子栏数
	int			m_nFixedCols;			//固定格子的栏数

    CUIntArray  m_arRowHeights, m_arColWidths;
    int         m_nMargin;
    int         m_nDefCellWidth, m_nDefCellHeight;
    int         m_nVScrollMax, m_nHScrollMax;

    // Fonts and images
    LOGFONT     m_Logfont;
    CFont		m_Font;         // for the grid
    CImageList* m_pImageList;

    // Cell data
    CTypedPtrArray<CObArray, GRID_ROW*> m_RowData;

    // Mouse operations such as cell selection
	int			m_MouseMode;	//鼠标状态
    CPoint      m_LeftClickDownPoint, m_LastMousePoint;
    CCellID     m_LeftClickDownCell, m_SelectionStartCell;
    CCellID     m_idCurrentCell;
    int         m_nResizeCaptureRange;
    int         m_nRowsPerWheelNotch;
    CMap<DWORD,DWORD, CCellID, CCellID&> m_SelectedCellMap, m_PrevSelectedCellMap;

    // sorting
    int         m_bAscending;
    int         m_SortColumn;

protected:
    void SelectAllCells();
    void SelectColumns(CCellID currentCell);
    void SelectRows(CCellID currentCell);
    void SelectCells(CCellID currentCell);
    void OnSelecting(const CCellID& currentCell);

    // Generated message map functions
    //{{AFX_MSG(CGridCtrl)
    afx_msg void OnPaint();
    afx_msg void OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar);
    afx_msg void OnVScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar);
    afx_msg void OnSize(UINT nType, int cx, int cy);
    afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
    afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
    afx_msg void OnMouseMove(UINT nFlags, CPoint point);
    afx_msg UINT OnGetDlgCode();
    afx_msg void OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags);
    afx_msg BOOL OnEraseBkgnd(CDC* pDC);
    afx_msg BOOL OnSetCursor(CWnd* pWnd, UINT nHitTest, UINT message);
    afx_msg void OnSysColorChange();
	afx_msg void OnRButtonUp(UINT nFlags, CPoint point);
	//}}AFX_MSG
#if _MFC_VER >= 0x0421
    afx_msg void OnSettingChange(UINT uFlags, LPCTSTR lpszSection);
    afx_msg BOOL OnMouseWheel(UINT nFlags, short zDelta, CPoint pt);
#endif
    afx_msg LRESULT OnSetFont(WPARAM hFont, LPARAM lParam);
    afx_msg LRESULT OnGetFont(WPARAM hFont, LPARAM lParam);
    DECLARE_MESSAGE_MAP()

    enum eMouseModes {
		MOUSE_NOTHING,
		MOUSE_SELECT_ALL,
		MOUSE_SELECT_COL,
		MOUSE_SELECT_ROW,
		MOUSE_SELECT_CELLS,
		MOUSE_OVER_ROW_DIVIDE,			//鼠标经过行分界线位置
		MOUSE_SIZING_ROW,				//改变行尺寸状态
		MOUSE_OVER_COL_DIVIDE,			//鼠标经过栏分界线位置
		MOUSE_SIZING_COL};				//改变列尺寸状态
};

inline CGridCell* CGridCtrl::GetCell(int nRow, int nCol) const
{
    if (nRow < 0 || nRow >= m_nRows || nCol < 0 || nCol >= m_nCols) return NULL;

    GRID_ROW* pRow = m_RowData[nRow];
    if (!pRow) return NULL;
    return pRow->GetAt(nCol);
}

inline BOOL CGridCtrl::SetCell(int nRow, int nCol, CGridCell* pCell)
{
    if (nRow < 0 || nRow >= m_nRows || nCol < 0 || nCol >= m_nCols) return FALSE;

    GRID_ROW* pRow = m_RowData[nRow];
    if (!pRow) return FALSE;

    pRow->SetAt(nCol, pCell);
    return TRUE;
}

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Developer Studio will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_GRIDCTRL_H__519FA702_722C_11D1_ABBA_00A0243D1382__INCLUDED_)
