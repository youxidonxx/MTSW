// GridCtrl.cpp : implementation file
//
// MFC Grid Control
//
// Written by Chris Maunder 
//        mailto:chrismaunder@codeguru.com
//
// Copyright (c) 1998.
//
// The code contained in this file is based on the original
// WorldCom Grid control written by Joe Willcoxson,
//        mailto:chinajoe@aol.com
//        http://users.aol.com/chinajoe
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
// Please use and enjoy, and let me know of any bugs/mods/improvements 
// that you have found/implemented and I will fix/incorporate them into 
// this file. 
//
//  History:
//  --------
//  This control is constantly evolving, sometimes due to new features that I
//  feel are necessary, and sometimes due to existing bugs. Where possible I 
//  have credited the changes to those who contributed code corrections or
//  enhancements (names in brackets) or code suggestions (suggested by...)
//   
//          1.0     20 Feb 1998   First release version.
//          1.01    24 Feb 1998   Memory leak fix (Jens Bohlmann)
//                                Fixec typo (my fault!) in CMemDC.h - Claus Arend-Schneider)
//                                Bug in GetSelectedCount (Lyn Newton)
//          1.02    4  Mar 1998   Scrolling a little neater (less dead area)
//                                Cell selection via OnTimer correctly updates Focus cell (Suggested by Lyn Newton)
//          1.03    17 Mar 1998   Clipboard functions added, Intellimouse support
//                                Using 32 bit scroll pos functions instead of 16 bit ("cronos")
//                                Added OLE drag and drop.
//          1.04     6 Apr 1998   Added Ctrl-A = Select All, fixed CGridDropTarget 
//                                problem, minor bug in CopyTextFromGrid (assert on
//                                empty string). Cleaned up reponse to m_bEditable
//                                (OnDrop and Ctrl-X disabled)
//          1.05    10 May 1998   Memory leak fixed. (Yuheng Zhao)
//                                Changed OLE initialisation (Carlo Comino)
//                                Added separate fore + background cell colours (Suggested by John Crane)
//                                ExpandToFit etc cleaned up - now decreases and
//                                increases cell sizes to fit client area.
//                                Added notification messages for the grid's parent (Suggested by 
//                                Added GVIS_READONLY state
//          1.06    20 May 1998   Added TAB key handling. (Daniela Rybarova)
//                                Intellimouse code correction for whole page scrolling (Paul Grant) 
//                                Fixed 16 bit thumb track problems (now 32 bit) (Paul Grant) 
//                                Fixed accelerator key problem in CInPlaceEdit (Matt Weagle)
//                                Fixed Stupid ClassWizard code parsing problem (Michael A. Barnhart)
//                                Double buffering now programmatically selectable
//                                Workaround for win95 drag and drop registration problem
//                                Corrected UNICODE implementation of clipboard stuff
//                                Dragging and dropping from a selection onto itself no 
//                                no longer causes the cells to be emptied
//          1.07    28 Jul 1998   Added EnsureVisible. (Roelf Werkman)
//                                Fixed delete key problem on read-only cells. (Serge Weinstock)
//                                Added CreateInPlaceEditControl to make it easier to 
//                                change the way cells are edited. (suggested by Chris Clark)
//                                Added Set/GetGridColor.
//                                CopyTextToClipboard and PasteTextToGrid problem with 
//                                blank cells fixed, and CopyTextToClipboard tweaked.
//                                Focus cell made visible when editing begins.
//                                Blank lines now treated correctly when pasting data.
//                                Removed ES_MULTILINE style from the default edit control.
//                                Added virtual CreateCell(row, col) function.
//                                Fonts now specified on a per-cell basis using Get/SetItemFont.
//          1.08     6 Aug 1998   Ctrl+arrows now allows cell navigation. Modified
//                                CreateInPlaceEditControl to accept ID of control.
//                                Added Titletips to grid cells. (Added EnableTitleTips / GetTitleTips)
//          1.09    12 Sep 1998   When printing, parent window title is printed in header - Gert Rijs
//                                GetNextItem search with GVNI_DROPHILITED now returns
//                                cells with GVIS_DROPHILITED set, instead of GVIS_FOCUSED (Franco Bez)
//                                (Also fixed minor bug in GetNextItem) (Franco Bez)
//                                Cell selection using Shift+arrows works - Franco Bez 
//                                Cell colours now dithered in 256 colour screens.
//                                Support for MSVC 4.2 (Graham Cheetham)
//          1.10    30 Nov 1998   Titletips now disappear on a scroll event. Compiler errors
//                                fixed. Grid lines drawing fixed (Graham Cheetham). 
//                                Cell focus fix on Isert Row/Col (Jochen Kauffmann) 
//                                Added DeleteNonFixedRows() (John Rackley)
//                                Message #define conflict fixed (Oskar Wieland)
//                                Titletips & cell insert/delete fix (Ramesh Dhar) 
//                                Titletips repeat-creation bug fixed.
//                                GVN_SELCHANGED message now sends current cell ID
//                                Font resource leak in GetTextExtent fixed (Gavin Jerman)
//                                More TAB fixes (Andreas Ruh)
//          1.11    1 Dec 1998    GetNextItem bug fix (suggested by Francis Fu)
//                                InsertColumn (-1) fix (Roy Hopkins)
//                                Was too liberal with the "IsEditable"'s. oops. (Michel Hete)
//                                          
// TODO:
//    - OnOutOfMemory function instead of exceptions
//    - Decrease timer interval over time to speed up selection over time
// 
// NOTE: Grid data is stored row-by-row, so all operations on large numbers
//       of cells should be done row-by-row as well.
// 
// KNOWN ISSUES TO BE ADDRESSED (Please don't send bug reports): 
// * Killfocus comes to late when a command is selected by the Menu. 
//   When you are editing a cell and choose a Menuitem that searches for all the
//   modified cells it is not found. When you chose the menu a second time it is
//   found. I assume that the Menu command is executed before the cell receives the
//   KillFocus event. Expect similar Problems with accelerators. (Franco Bez)
// * When you select a cell and move the mouse around (with the Left button down 
//   i.e continuing with your selection) - if the mouse is over the Fixed column 
//   or Row the drawing of the selected region is strange - in particular as you 
//   move up and down say the Left Fixed Column notice the behaviour of the Focus 
//   Cell - it is out of sync. (Vinay Desai)
//
/////////////////////////////////////////////////////////////////////////////
    
#include "stdafx.h"
#include "GridCtrl.h"

// OLE stuff for clipboard operations
#include <afxadv.h>            // For CSharedFile
#include <afxole.h>            // For COleDataSource
#include <afxconv.h>           // For LPTSTR -> LPSTR macros

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

#define HEADER_HEIGHT       2    // For printing
#define FOOTER_HEIGHT       2
#define LEFT_MARGIN         4
#define RIGHT_MARGIN        4
#define TOP_MARGIN          1
#define BOTTOM_MARGIN       1 
#define GAP                 1

#define SELECTED_CELL_FONT_WEIGHT 600    // wieght of text for selected items
#define IDC_INPLACE_EDIT            8    // ID of inplace edit controls

IMPLEMENT_DYNCREATE(CGridCtrl, CWnd)

void AFXAPI DDX_GridControl(CDataExchange* pDX, int nIDC, CGridCtrl& rControl)
{
    if (rControl.GetSafeHwnd() == NULL)    // not subclassed yet
    {
        ASSERT(!pDX->m_bSaveAndValidate);

        HWND hWndCtrl = pDX->PrepareCtrl(nIDC);

        if (!rControl.SubclassWindow(hWndCtrl))
        {
            ASSERT(FALSE);      // possibly trying to subclass twice?
            AfxThrowNotSupportedException();
        }
#ifndef _AFX_NO_OCC_SUPPORT
        else
        {
            // If the control has reparented itself (e.g., invisible control),
            // make sure that the CWnd gets properly wired to its control site.
            if (pDX->m_pDlgWnd->GetSafeHwnd() != ::GetParent(rControl.GetSafeHwnd()))
                rControl.AttachControlSite(pDX->m_pDlgWnd);
        }
#endif //!_AFX_NO_OCC_SUPPORT

    }
}

// Get the number of lines to scroll with each mouse wheel notch
// Why doesn't windows give us this function???
UINT GetMouseScrollLines()
{
    int nScrollLines = 3;            // reasonable default
    HKEY hKey;

    if (RegOpenKeyEx(HKEY_CURRENT_USER,  _T("Control Panel\\Desktop"),
                     0, KEY_QUERY_VALUE, &hKey) == ERROR_SUCCESS)
    {
        TCHAR szData[128];
        DWORD dwKeyDataType;
        DWORD dwDataBufSize = sizeof(szData);

        if (RegQueryValueEx(hKey, _T("WheelScrollLines"), NULL, &dwKeyDataType,
                           (LPBYTE) &szData, &dwDataBufSize) == ERROR_SUCCESS)
        {
            nScrollLines = _tcstoul(szData, NULL, 10);
        }
        RegCloseKey(hKey);
    }

    return nScrollLines;
}

/////////////////////////////////////////////////////////////////////////////
// CGridCtrl

CGridCtrl::CGridCtrl(int nRows, int nCols, int nFixedRows, int nFixedCols)
{
    RegisterWindowClass();

    // Store the system colours in case they change. The gridctrl uses
    // these colours, and in OnSysColorChange we can check to see if 
    // the gridctrl colours have been changed from the system colours.
    // If they have, then leave them, otherwise change them to reflect
    // the new system colours.
    m_crWindowText       = ::GetSysColor(COLOR_WINDOWTEXT);
    m_crWindowColour     = ::GetSysColor(COLOR_WINDOW);
    m_cr3DFace           = ::GetSysColor(COLOR_3DFACE);
    m_crShadow           = ::GetSysColor(COLOR_3DSHADOW);
    m_crGridColour       = ::GetSysColor(COLOR_3DSHADOW);	//RGB(0,0,0);

    m_nRows              = 0;
    m_nCols              = 0;
    m_nFixedRows         = 0;
    m_nFixedCols         = 0;

    m_nDefCellHeight     = 10;        // These will get changed to something meaningful
    m_nDefCellHeight     = 30;        //    when the window is created or subclassed

    m_nVScrollMax        = 0;         // Scroll position
    m_nHScrollMax        = 0;
    m_nMargin            = 0;         // cell padding
    m_nRowsPerWheelNotch = GetMouseScrollLines(); // Get the number of lines
                                                  // per mouse wheel notch to scroll

    m_MouseMode			= MOUSE_NOTHING;
    m_nGridLines		= GVL_BOTH;
    m_bEditable			= TRUE;
	m_bAllowDraw		= TRUE;			//ÔÊÐí¸üÐÂ
    m_bEnableSelection	= TRUE;
    m_bAllowRowResize	= TRUE;
    m_bAllowColResize	= TRUE;
    m_bSortOnClick		= TRUE;			// Sort on header row click if in list mode
    m_bHandleTabKey		= TRUE;
    m_bDoubleBuffer		= TRUE;			// Use double buffering to avoid flicker?
    m_bTitleTips		= TRUE;			// show cell title tips
	m_bEdit				= FALSE;

    m_bAscending		= TRUE;			// sorting stuff
    m_SortColumn		= -1;

    m_nResizeCaptureRange= 3;			// When resizing columns/row, the cursor has to be 
										// within +/-3 pixels of the dividing line for 
										// resizing to be possible
    m_pImageList		= NULL;      

    // Initially use the system message font for the GridCtrl font
    NONCLIENTMETRICS ncm;
    ncm.cbSize = sizeof(NONCLIENTMETRICS);
    VERIFY(SystemParametersInfo(SPI_GETNONCLIENTMETRICS, sizeof(NONCLIENTMETRICS), &ncm, 0));    
    memcpy(&m_Logfont, &(ncm.lfMessageFont), sizeof(LOGFONT));

    // Set up the initial grid size
    SetRowCount(nRows);
    SetColumnCount(nCols);
    SetFixedRowCount(nFixedRows);
    SetFixedColumnCount(nFixedCols);

    // Set the colours
    m_crTextColour		= m_crWindowText;
    m_crTextBkColour	= m_crWindowColour;
    m_crBkColour		= m_crShadow;
    m_crFixedTextColour	= m_crWindowText;
    m_crFixedBkColour	= m_cr3DFace;

    // set initial selection range (ie. none)
    m_SelectedCellMap.RemoveAll();
    m_PrevSelectedCellMap.RemoveAll();
}

CGridCtrl::~CGridCtrl()
{
    DeleteAllItems();
    DestroyWindow();
    m_Font.DeleteObject();
}

// Register the window class if it has not already been registered.
BOOL CGridCtrl::RegisterWindowClass()
{
    WNDCLASS wndcls;
    HINSTANCE hInst = AfxGetResourceHandle();

    if (!(::GetClassInfo(hInst, GRIDCTRL_CLASSNAME, &wndcls)))
    {
        // otherwise we need to register a new class
        wndcls.style            = CS_DBLCLKS | CS_HREDRAW | CS_VREDRAW;
        wndcls.lpfnWndProc      = ::DefWindowProc;
        wndcls.cbClsExtra       = wndcls.cbWndExtra = 0;
        wndcls.hInstance        = hInst;
        wndcls.hIcon            = NULL;
        wndcls.hCursor          = NULL;
        wndcls.hbrBackground    = (HBRUSH) (COLOR_3DFACE + 1);
        wndcls.lpszMenuName     = NULL;
        wndcls.lpszClassName    = GRIDCTRL_CLASSNAME;

        if (!AfxRegisterClass(&wndcls)) {
            AfxThrowResourceException();
            return FALSE;
        }
    }

    return TRUE;
}

BOOL CGridCtrl::Create(const RECT& rect, CWnd* pParentWnd, UINT nID, DWORD dwStyle)
{
    ASSERT(pParentWnd->GetSafeHwnd());

    if (!CWnd::Create(GRIDCTRL_CLASSNAME, NULL, dwStyle, rect, pParentWnd, nID)) 
        return FALSE;

    // The number of rows and columns will only be non-zero if the constructor
    // was called with non-zero initialising parameters. If this window was created
    // using a dialog template then the number of rows and columns will be 0 (which
    // means that the code below will not be needed - which is lucky 'cause it ain't
    // gonna get called in a dialog-template-type-situation.

    try {
        m_arRowHeights.SetSize(m_nRows);    // initialize row heights
        m_arColWidths.SetSize(m_nCols);     // initialize column widths
    }
    catch (CMemoryException *e) {
        e->ReportError();
        e->Delete();
        return FALSE;
    }
	int i = 0;
    for ( ;i < m_nRows; i++) m_arRowHeights[i] = m_nDefCellHeight;
    for (i = 0; i < m_nCols; i++)      m_arColWidths[i] = m_nDefCellWidth;

    ResetScrollBars();
    return TRUE;
}

void CGridCtrl::PreSubclassWindow() 
{    
    CWnd::PreSubclassWindow();

    HFONT hFont = ::CreateFontIndirect(&m_Logfont);
    OnSetFont((LPARAM)hFont, 0);
    DeleteObject(hFont);
    
    ResetScrollBars();   
}

BOOL CGridCtrl::SubclassWindow(HWND hWnd) 
{    
    if (!CWnd::SubclassWindow(hWnd)) return FALSE;

    return TRUE;
}

LRESULT CGridCtrl::SendMessageToParent(int nRow, int nCol, int nMessage)
{
    if (!IsWindow(m_hWnd))
        return 0;

    CWnd *pOwner = GetOwner();
    if (pOwner && IsWindow(pOwner->m_hWnd))
	{
		GV_DISPINFO info;
		info.item.row		= nRow;
		info.item.col		= nCol;
		info.item.mask		= 0xFFFFFFFF;
		GetItem(&info.item);

		GetCellRect(nRow,nCol,&info.rect);
		m_rectEdit=info.rect;

		ClientToScreen(&info.rect);

		info.hdr.hwndFrom	= m_hWnd;
		info.hdr.idFrom		= GetDlgCtrlID();
		info.hdr.code		= nMessage;

        return pOwner->SendMessage(WM_NOTIFY, (WPARAM)info.hdr.idFrom, (LPARAM)&info);
	}
    else
        return 0;
}

BEGIN_MESSAGE_MAP(CGridCtrl, CWnd)
    //{{AFX_MSG_MAP(CGridCtrl)
    ON_WM_PAINT()
    ON_WM_HSCROLL()
    ON_WM_VSCROLL()
    ON_WM_SIZE()
    ON_WM_LBUTTONUP()
    ON_WM_LBUTTONDOWN()
    ON_WM_MOUSEMOVE()
    ON_WM_GETDLGCODE()
    ON_WM_KEYDOWN()
    ON_WM_ERASEBKGND()
    ON_WM_SETCURSOR()
    ON_WM_SYSCOLORCHANGE()
	ON_WM_RBUTTONUP()
	//}}AFX_MSG_MAP
#if _MFC_VER >= 0x0421
    ON_WM_MOUSEWHEEL()
    ON_WM_SETTINGCHANGE()
#endif
    ON_MESSAGE(WM_SETFONT, OnSetFont)
    ON_MESSAGE(WM_GETFONT, OnGetFont)
END_MESSAGE_MAP()


/////////////////////////////////////////////////////////////////////////////
// CGridCtrl message handlers

void CGridCtrl::OnPaint() 
{
	CPaintDC dc(this);      // device context for painting

	if(m_bEdit)
		dc.ExcludeClipRect(m_rectEdit);

//	if (m_bDoubleBuffer)    // Use a memory DC to remove flicker
//	{
//		CMemDC MemDC(&dc);
//		OnDraw(&MemDC);
//	}
//	else                    // Draw raw - this helps in debugging vis problems.
		OnDraw(&dc);
}

BOOL CGridCtrl::OnEraseBkgnd(CDC* /*pDC*/) 
{
    return TRUE;    // Don't erase the background.
}

// Custom background erasure. This gets called from within the OnDraw function,
// since we will (most likely) be using a memory DC to stop flicker. If we just
// erase the background normally through OnEraseBkgnd, and didn't fill the memDC's
// selected bitmap with colour, then all sorts of vis problems would occur
void CGridCtrl::EraseBkgnd(CDC* pDC) 
{
    CRect  VisRect, ClipRect, rect;
    CBrush FixedBack(m_crFixedBkColour),
           TextBack(m_crTextBkColour),
           Back(m_crBkColour);

    if (pDC->GetClipBox(ClipRect) == ERROR) return;
    GetVisibleNonFixedCellRange(VisRect);

    // Draw Fixed columns background
    int nFixedColumnWidth = GetFixedColumnWidth();
    if (ClipRect.left < nFixedColumnWidth && ClipRect.top < VisRect.bottom)
        pDC->FillRect(CRect(ClipRect.left, ClipRect.top, nFixedColumnWidth, VisRect.bottom),
                      &FixedBack);
        
    // Draw Fixed rows background
    int nFixedRowHeight = GetFixedRowHeight();
    if (ClipRect.top < nFixedRowHeight && 
        ClipRect.right > nFixedColumnWidth && ClipRect.left < VisRect.right)
        pDC->FillRect(CRect(nFixedColumnWidth-1, ClipRect.top, VisRect.right, nFixedRowHeight),
                      &FixedBack);


    // Draw non-fixed cell background
    if (rect.IntersectRect(VisRect, ClipRect)) 
    {
        CRect CellRect(max(nFixedColumnWidth, rect.left), max(nFixedRowHeight, rect.top),
                       rect.right, rect.bottom);
        pDC->FillRect(CellRect, &TextBack);
    }

    // Draw right hand side of window outside grid
    if (VisRect.right < ClipRect.right) 
        pDC->FillRect(CRect(VisRect.right, ClipRect.top, ClipRect.right, ClipRect.bottom),
                      &Back);

    // Draw bottom of window below grid
    if (VisRect.bottom < ClipRect.bottom && ClipRect.left < VisRect.right) 
        pDC->FillRect(CRect(ClipRect.left, VisRect.bottom, VisRect.right, ClipRect.bottom),
                      &Back);
}

void CGridCtrl::OnSize(UINT nType, int cx, int cy) 
{
	SetFocus();		// Auto-destroy any InPlaceEdit's

	CWnd::OnSize(nType, cx, cy);
	ResetScrollBars();
}

UINT CGridCtrl::OnGetDlgCode() 
{
    UINT nCode = DLGC_WANTARROWS | DLGC_WANTCHARS;

    if (m_bHandleTabKey && !IsCTRLpressed()) 
        nCode |= DLGC_WANTTAB;

    return nCode;
}

// If system colours change, then redo colours
void CGridCtrl::OnSysColorChange() 
{
    CWnd::OnSysColorChange();
    
    if (m_crTextColour == m_crWindowText)						// Still using system colours
        m_crTextColour = ::GetSysColor(COLOR_WINDOWTEXT);      // set to new system colour
    if (m_crTextBkColour == m_crWindowColour)
        m_crTextBkColour = ::GetSysColor(COLOR_WINDOW);
    if (m_crBkColour == m_crShadow)
        m_crBkColour = ::GetSysColor(COLOR_3DSHADOW);
    if (m_crFixedTextColour == m_crWindowText)
        m_crFixedTextColour	= ::GetSysColor(COLOR_WINDOWTEXT);
    if (m_crFixedBkColour == m_cr3DFace)
        m_crFixedBkColour = ::GetSysColor(COLOR_3DFACE);

    m_crWindowText   = ::GetSysColor(COLOR_WINDOWTEXT);
    m_crWindowColour = ::GetSysColor(COLOR_WINDOW);
    m_cr3DFace       = ::GetSysColor(COLOR_3DFACE);
    m_crShadow       = ::GetSysColor(COLOR_3DSHADOW);
}


#if _MFC_VER >= 0x0421
// If system settings change, then redo colours
void CGridCtrl::OnSettingChange(UINT uFlags, LPCTSTR lpszSection) 
{
    CWnd::OnSettingChange(uFlags, lpszSection);
    
    if (m_crTextColour == m_crWindowText)					// Still using system colours
        m_crTextColour	= ::GetSysColor(COLOR_WINDOWTEXT);	// set to new system colour
    if (m_crTextBkColour == m_crWindowColour)
        m_crTextBkColour= ::GetSysColor(COLOR_WINDOW);
    if (m_crBkColour == m_crShadow)
        m_crBkColour = ::GetSysColor(COLOR_3DSHADOW);
    if (m_crFixedTextColour == m_crWindowText)
        m_crFixedTextColour	= ::GetSysColor(COLOR_WINDOWTEXT);
    if (m_crFixedBkColour == m_cr3DFace)
        m_crFixedBkColour = ::GetSysColor(COLOR_3DFACE);

    m_crWindowText   = ::GetSysColor(COLOR_WINDOWTEXT);
    m_crWindowColour = ::GetSysColor(COLOR_WINDOW);
    m_cr3DFace       = ::GetSysColor(COLOR_3DFACE);
    m_crShadow       = ::GetSysColor(COLOR_3DSHADOW);

    m_nRowsPerWheelNotch = GetMouseScrollLines(); // Get the number of lines
}
#endif

// move about with keyboard
void CGridCtrl::OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags) 
{
    if (!IsValid(m_idCurrentCell)) 
    {
        CWnd::OnKeyDown(nChar, nRepCnt, nFlags);
        return;
    }

    CCellID next = m_idCurrentCell;
    BOOL bChangeLine = FALSE;

    switch (nChar)
    {
//			case VK_DELETE: 
//				if (IsCellEditable(m_idCurrentCell.row, m_idCurrentCell.col))
//				{
//					SetItemText(m_idCurrentCell.row, m_idCurrentCell.col, _T(""));
//					RedrawCell(m_idCurrentCell);
//				}
//				break;
        case VK_TAB:    
            if (IsSHIFTpressed())
            {
                if (next.col > m_nFixedCols) 
                    next.col--;
                else if (next.col == m_nFixedCols && next.row > m_nFixedRows) 
                {
                    next.row--; 
                    next.col = GetColumnCount() - 1; 
                    bChangeLine = TRUE;
                }
                else
                    CWnd::OnKeyDown(nChar, nRepCnt, nFlags);
            }
            else
            {
                if (next.col < (GetColumnCount() - 1)) 
                    next.col++;
                else if (next.col == (GetColumnCount() - 1) && 
                         next.row < (GetRowCount() - 1) )
                {
                    next.row++; 
                    next.col = m_nFixedCols; 
                    bChangeLine = TRUE;
                }
                else
                    CWnd::OnKeyDown(nChar, nRepCnt, nFlags);
            } 
            break;

        case VK_DOWN:   
            if (next.row < (GetRowCount() - 1))
                next.row++; 
            break;            
            
        case VK_UP:     
            if (next.row > m_nFixedRows)           
                next.row--; 
            break;

        case VK_RIGHT:  
            if (next.col < (GetColumnCount() - 1)) 
                next.col++; 
            break;
            
        case VK_LEFT:   
            if (next.col > m_nFixedCols)           
                next.col--; 
            break;

        case VK_NEXT:   
            {
            CCellID idOldTopLeft = GetTopleftNonFixedCell();
            SendMessage(WM_VSCROLL, SB_PAGEDOWN, 0);
            CCellID idNewTopLeft = GetTopleftNonFixedCell();

            int increment = idNewTopLeft.row - idOldTopLeft.row;
            if (increment) {
                next.row += increment;
                if (next.row > (GetRowCount() - 1)) 
                    next.row = GetRowCount() - 1;
            }
            else
                next.row = GetRowCount() - 1;
                break;
            }
    
            case VK_PRIOR:  
            {
            CCellID idOldTopLeft = GetTopleftNonFixedCell();
            SendMessage(WM_VSCROLL, SB_PAGEUP, 0);
            CCellID idNewTopLeft = GetTopleftNonFixedCell();

            int increment = idNewTopLeft.row - idOldTopLeft.row;
            if (increment) 
            {
                next.row += increment;
                if (next.row < m_nFixedRows) 
                    next.row = m_nFixedRows;
            } else
                next.row = m_nFixedRows;
                break;
            }
    
        case VK_HOME:   
            SendMessage(WM_VSCROLL, SB_TOP, 0);
            next.row = m_nFixedRows;
            break;
        
        case VK_END:    
            SendMessage(WM_VSCROLL, SB_BOTTOM, 0);
            next.row = GetRowCount() - 1;
            break;
                
        default:
            CWnd::OnKeyDown(nChar, nRepCnt, nFlags);
    }
  
    if (next != m_idCurrentCell) 
    {
        // While moving with the Cursorkeys the current ROW/CELL will get selected
        // OR Selection will get expanded when SHIFT is pressed
        // Cut n paste from OnLButtonDown - Franco Bez 
        // Added check for NULL mouse mode - Chris Maunder.
        if (m_MouseMode == MOUSE_NOTHING)
        {
            m_PrevSelectedCellMap.RemoveAll();
            m_MouseMode = MOUSE_SELECT_CELLS;
            if (!IsSHIFTpressed() || nChar == VK_TAB)
                m_SelectionStartCell = next;
            OnSelecting(next);
            m_MouseMode = MOUSE_NOTHING;
        }

        SetFocusCell(next);

        if (!IsCellVisible(next))
        {   
            EnsureVisible(next); // Make sure cell is visible

            switch (nChar) {
                case VK_RIGHT:  
                    SendMessage(WM_HSCROLL, SB_LINERIGHT, 0); 
                    break;

                case VK_LEFT:   
                    SendMessage(WM_HSCROLL, SB_LINELEFT, 0);  
                    break;

                case VK_DOWN:   
                    SendMessage(WM_VSCROLL, SB_LINEDOWN, 0);  
                    break;
                
                case VK_UP:     
                    SendMessage(WM_VSCROLL, SB_LINEUP, 0);    
                    break;                
                
                case VK_TAB:    
                    if (IsSHIFTpressed())
                    {
                        if (bChangeLine) 
                        {
                            SendMessage(WM_VSCROLL, SB_LINEUP, 0);
                            SetScrollPos32(SB_HORZ, m_nHScrollMax);
                            break;
                        }
                        else 
                            SendMessage(WM_HSCROLL, SB_LINELEFT, 0);
                    }
                    else
                    {
                        if (bChangeLine) 
                        {
                            SendMessage(WM_VSCROLL, SB_LINEDOWN, 0);
                            SetScrollPos32(SB_HORZ, 0);
                            break;
                        }
                        else 
                            SendMessage(WM_HSCROLL, SB_LINERIGHT, 0);
                    }
                    break;

            }
            Invalidate();
        }
    }
}

// Handle horz scrollbar notifications
void CGridCtrl::OnHScroll(UINT nSBCode, UINT /*nPos*/, CScrollBar* /*pScrollBar*/)
{
    if (GetFocus()->GetSafeHwnd() != GetSafeHwnd()) 
        SetFocus();  // Auto-destroy any InPlaceEdit's

    int scrollPos = GetScrollPos32(SB_HORZ);

    CCellID idTopLeft = GetTopleftNonFixedCell();

    CRect rect;
    GetClientRect(rect);

    switch (nSBCode)
    {
        case SB_LINERIGHT:
            if (scrollPos < m_nHScrollMax)
            {
                int xScroll = GetColumnWidth(idTopLeft.col);
                SetScrollPos32(SB_HORZ, scrollPos + xScroll);
                if (GetScrollPos32(SB_HORZ) == scrollPos) break;          // didn't work

                rect.left = GetFixedColumnWidth() + xScroll;
                ScrollWindow(-xScroll, 0, rect);
                rect.left = rect.right - xScroll;
                InvalidateRect(rect);
            }
            break;

        case SB_LINELEFT:
            if (scrollPos > 0 && idTopLeft.col > GetFixedColumnCount())
            {
                int xScroll = GetColumnWidth(idTopLeft.col-1);
                SetScrollPos32(SB_HORZ, max(0,scrollPos - xScroll));
                rect.left = GetFixedColumnWidth();
                ScrollWindow(xScroll, 0, rect);
                rect.right = rect.left + xScroll;
                InvalidateRect(rect);
            }
            break;

        case SB_PAGERIGHT:
            if (scrollPos < m_nHScrollMax)
            {
                rect.left = GetFixedColumnWidth();
                int offset = rect.Width();
                int pos = min(m_nHScrollMax, scrollPos + offset);
                SetScrollPos32(SB_HORZ, pos);
                rect.left = GetFixedColumnWidth();
                InvalidateRect(rect);
            }
            break;

        case SB_PAGELEFT:
            if (scrollPos > 0)
            {
                rect.left = GetFixedColumnWidth();
                int offset = -rect.Width();
                int pos = max(0, scrollPos + offset);
                SetScrollPos32(SB_HORZ, pos);
                rect.left = GetFixedColumnWidth();
                InvalidateRect(rect);
            }
            break;

        case SB_THUMBPOSITION:
        case SB_THUMBTRACK:
            {
                SetScrollPos32(SB_HORZ, GetScrollPos32(SB_HORZ, TRUE));
                rect.left = GetFixedColumnWidth();
                InvalidateRect(rect);
            }
            break;

        case SB_LEFT:
            if (scrollPos > 0)
            {
                SetScrollPos32(SB_HORZ, 0);
                Invalidate();
            }
            break;

        case SB_RIGHT:
            if (scrollPos < m_nHScrollMax)
            {
                SetScrollPos32(SB_HORZ, m_nHScrollMax);
                Invalidate();
            }
            break;


        default: break;
    }
}

// Handle vert scrollbar notifications
void CGridCtrl::OnVScroll(UINT nSBCode, UINT /*nPos*/, CScrollBar* /*pScrollBar*/)
{
    if (GetFocus()->GetSafeHwnd() != GetSafeHwnd()) 
        SetFocus();        // Auto-destroy any InPlaceEdit's

    // Get the scroll position ourselves to ensure we get a 32 bit value
    int scrollPos = GetScrollPos32(SB_VERT);

    CCellID idTopLeft = GetTopleftNonFixedCell();

    CRect rect;
    GetClientRect(rect);

    switch (nSBCode)
    {
        case SB_LINEDOWN:
            if (scrollPos < m_nVScrollMax)
            {
                int yScroll = GetRowHeight(idTopLeft.row);
                SetScrollPos32(SB_VERT, scrollPos + yScroll);
                if (GetScrollPos32(SB_VERT) == scrollPos) break;          // didn't work

                rect.top = GetFixedRowHeight() + yScroll;
                ScrollWindow( 0, -yScroll, rect);
                rect.top = rect.bottom - yScroll;
                InvalidateRect(rect);
            }
            break;

        case SB_LINEUP:
            if (scrollPos > 0 && idTopLeft.row > GetFixedRowCount())
            {
                int yScroll = GetRowHeight(idTopLeft.row-1);
                SetScrollPos32(SB_VERT, max(0, scrollPos - yScroll));
                rect.top = GetFixedRowHeight();
                ScrollWindow(0, yScroll, rect);
                rect.bottom = rect.top + yScroll;
                InvalidateRect(rect);
            }
            break;

        case SB_PAGEDOWN:
            if (scrollPos < m_nVScrollMax)
            {
                rect.top = GetFixedRowHeight();
                scrollPos = min(m_nVScrollMax, scrollPos + rect.Height());
                SetScrollPos32(SB_VERT, scrollPos);
                rect.top = GetFixedRowHeight();
                InvalidateRect(rect);
            }
            break;

        case SB_PAGEUP:
            if (scrollPos > 0)
            {
                rect.top = GetFixedRowHeight();
                int offset = -rect.Height();
                int pos = max(0, scrollPos + offset);
                SetScrollPos32(SB_VERT, pos);
                rect.top = GetFixedRowHeight();
                InvalidateRect(rect);
            }
            break;

        case SB_THUMBPOSITION:
        case SB_THUMBTRACK:
            {
                SetScrollPos32(SB_VERT, GetScrollPos32(SB_VERT, TRUE));
                rect.top = GetFixedRowHeight();
                InvalidateRect(rect);
            }
            break;

        case SB_TOP:
            if (scrollPos > 0)
            {
                SetScrollPos32(SB_VERT, 0);
                Invalidate();
            }
            break;

        case SB_BOTTOM:
            if (scrollPos < m_nVScrollMax)
            {
                SetScrollPos32(SB_VERT, m_nVScrollMax);
                Invalidate();
            }

        default: break;
    }
}

/////////////////////////////////////////////////////////////////////////////
// CGridCtrl implementation functions

void CGridCtrl::OnDraw(CDC* pDC)
{
    CRect rect;
    int row,col;

    CRect clipRect;
    if (pDC->GetClipBox(&clipRect) == ERROR) return;

    EraseBkgnd(pDC);            // OnEraseBkgnd does nothing, so erase bkgnd here.
                                // This necessary since we may be using a Memory DC.

    int nFixedRowHeight = GetFixedRowHeight();
    int nFixedColWidth  = GetFixedColumnWidth();

    CCellID idTopLeft = GetTopleftNonFixedCell();
    int minVisibleRow = idTopLeft.row,
        minVisibleCol = idTopLeft.col;

    CRect VisRect;
    CCellRange VisCellRange = GetVisibleNonFixedCellRange(VisRect);
    int maxVisibleRow = VisCellRange.GetMaxRow(),
        maxVisibleCol = VisCellRange.GetMaxCol();

    // draw top-left cells 0..m_nFixedRows-1, 0..m_nFixedCols-1
    rect.bottom = -1;
    for (row = 0; row < m_nFixedRows; row++)
    {
        rect.top = rect.bottom+1;
        rect.bottom = rect.top + GetRowHeight(row)-1;
        rect.right = -1;
        for (col = 0; col < m_nFixedCols; col++)
        {
            rect.left = rect.right+1;
            rect.right = rect.left + GetColumnWidth(col)-1;
            DrawFixedCell(pDC, row, col, rect);
        }
    }
     
    // draw fixed column cells:  m_nFixedRows..n, 0..m_nFixedCols-1
    rect.bottom = nFixedRowHeight-1;
    for (row = minVisibleRow; row <= maxVisibleRow; row++)
    {
        rect.top = rect.bottom+1;
        rect.bottom = rect.top + GetRowHeight(row)-1;

        // rect.bottom = bottom pixel of previous row
        if (rect.top > clipRect.bottom) break;                // Gone past cliprect
        if (rect.bottom < clipRect.top) continue;             // Reached cliprect yet?

        rect.right = -1;
        for (col = 0; col < m_nFixedCols; col++)
        {
            rect.left = rect.right+1;
            rect.right = rect.left + GetColumnWidth(col)-1;

            if (rect.left > clipRect.right) break;            // gone past cliprect
            if (rect.right < clipRect.left) continue;         // Reached cliprect yet?

            DrawFixedCell(pDC, row, col, rect);
        }
    }
    
    // draw fixed row cells  0..m_nFixedRows, m_nFixedCols..n
    rect.bottom = -1;
    for (row = 0; row < m_nFixedRows; row++)
    {
        rect.top = rect.bottom+1;
        rect.bottom = rect.top + GetRowHeight(row)-1;

        // rect.bottom = bottom pixel of previous row
        if (rect.top > clipRect.bottom) break;                // Gone past cliprect
        if (rect.bottom < clipRect.top) continue;             // Reached cliprect yet?

        rect.right = nFixedColWidth-1;
        for (col = minVisibleCol; col <= maxVisibleCol; col++)
        {                                       
            rect.left = rect.right+1;
            rect.right = rect.left + GetColumnWidth(col)-1;

            if (rect.left > clipRect.right) break;        // gone past cliprect
            if (rect.right < clipRect.left) continue;     // Reached cliprect yet?

            DrawFixedCell(pDC, row, col, rect);
        }
    }
    
    // draw rest of non-fixed cells
    rect.bottom = nFixedRowHeight-1;
    for (row = minVisibleRow; row <= maxVisibleRow; row++)
    {
        rect.top = rect.bottom+1;
        rect.bottom = rect.top + GetRowHeight(row)-1;

        // rect.bottom = bottom pixel of previous row
        if (rect.top > clipRect.bottom) break;                // Gone past cliprect
        if (rect.bottom < clipRect.top) continue;             // Reached cliprect yet?

        rect.right = nFixedColWidth-1;
        for (col = minVisibleCol; col <= maxVisibleCol; col++)
        {
            rect.left = rect.right+1;
            rect.right = rect.left + GetColumnWidth(col)-1;

            if (rect.left > clipRect.right) break;        // gone past cliprect
            if (rect.right < clipRect.left) continue;     // Reached cliprect yet?

            DrawCell(pDC, row, col, rect);
        }
    }


    CPen pen;
    try {
        pen.CreatePen(PS_SOLID, 0, m_crGridColour);
    }
    catch (CResourceException *e)
    {
        e->Delete();
        return;
    }
    pDC->SelectObject(&pen);

    // draw vertical lines (drawn at ends of cells)
    if (m_nGridLines == GVL_BOTH || m_nGridLines == GVL_VERT) 
    {
        int x = nFixedColWidth;
        for (col = minVisibleCol; col <= maxVisibleCol; col++) {
            x += GetColumnWidth(col);
            pDC->MoveTo(x-1, nFixedRowHeight);
            pDC->LineTo(x-1, VisRect.bottom);   
        }
    }
    
    // draw horizontal lines (drawn at bottom of each cell)
    if (m_nGridLines == GVL_BOTH || m_nGridLines == GVL_HORZ) 
    {
        int y = nFixedRowHeight;
        for (row = minVisibleRow; row <= maxVisibleRow; row++) {
            y += GetRowHeight(row);
            pDC->MoveTo(nFixedColWidth, y-1);    
            pDC->LineTo(VisRect.right,  y-1);
        }
    }

    pDC->SelectStockObject(NULL_PEN);

#ifdef USE_MEMDC                        // Use a memDC for flicker free update
}
#else                                   // Use normal DC - this helps in debugging
}
#endif

////////////////////////////////////////////////////////////////////////////////////////
// CGridCtrl Cell selection stuff

BOOL CGridCtrl::IsValid(int nRow, int nCol) const
{
    return (nRow >= 0 && nRow < m_nRows && nCol >= 0 && nCol < m_nCols);
}

BOOL CGridCtrl::IsValid(const CCellID& cell) const
{
    return IsValid(cell.row, cell.col);
}

BOOL CGridCtrl::IsValid(const CCellRange& range) const
{
     return (range.GetMinRow() >= 0 && range.GetMinCol() >= 0 && 
            range.GetMaxRow() >= 0 && range.GetMaxCol() >= 0 &&
            range.GetMaxRow() < m_nRows && range.GetMaxCol() < m_nCols &&
             range.GetMinRow() <= range.GetMaxRow() && range.GetMinCol() <= range.GetMaxCol());
}

// Enables/Disables redraw for certain operations like columns auto-sizing etc,
// but not for user caused things such as selection changes.
void CGridCtrl::SetRedraw(BOOL bAllowDraw, BOOL bResetScrollBars /* = FALSE */)
{
	if(bAllowDraw && !m_bAllowDraw) Invalidate();
	m_bAllowDraw = bAllowDraw;
    if (bResetScrollBars) ResetScrollBars();
}

// Forces a redraw of a cell immediately (using a direct DC construction, 
// or the supplied dc)
BOOL CGridCtrl::RedrawCell(const CCellID& cell, CDC* pDC /* = NULL */)
{
    return RedrawCell(cell.row, cell.col, pDC);
}

BOOL CGridCtrl::RedrawCell(int nRow, int nCol, CDC* pDC /* = NULL */)
{
    BOOL bResult = TRUE;
    BOOL bMustReleaseDC = FALSE;

    if (!m_bAllowDraw || !IsCellVisible(nRow, nCol)) return FALSE;

    CRect rect;
    if (!GetCellRect(nRow, nCol, rect)) return FALSE;

    if (!pDC) {
        pDC = GetDC();
        if (pDC) bMustReleaseDC = TRUE;
    }

 //   if (pDC)
 //   {
 //       // Redraw cells directly
 //       if (nRow < m_nFixedRows || nCol < m_nFixedCols)
 //           bResult = DrawFixedCell(pDC, nRow, nCol, rect, TRUE);
 //       else
 //           bResult = DrawCell(pDC, nRow, nCol, rect, TRUE);

        // Since we have erased the background, we will need to redraw the gridlines
 //       CPen pen;
 //       try {
 //           pen.CreatePen(PS_SOLID, 0,m_crGridColour);
 //       } catch (...) {}
        
 //       CPen* pOldPen = (CPen*) pDC->SelectObject(&pen);
 //       if (m_nGridLines == GVL_BOTH || m_nGridLines == GVL_HORZ)
 //       {
 //           pDC->MoveTo(rect.left,    rect.bottom);
 //           pDC->LineTo(rect.right+1, rect.bottom);
 //       }
 //       if (m_nGridLines == GVL_BOTH || m_nGridLines == GVL_VERT) 
 //       {
 //           pDC->MoveTo(rect.right, rect.top);
 //           pDC->LineTo(rect.right, rect.bottom+1);    
 //       }
 //      pDC->SelectObject(pOldPen);

 //   } else
        InvalidateRect(rect, TRUE);     // Could not get a DC - invalidate it anyway
                                        // and hope that OnPaint manages to get one

    if (bMustReleaseDC) 
        ReleaseDC(pDC);

    return bResult;
}

// redraw a complete row
BOOL CGridCtrl::RedrawRow(int row)
{
    BOOL bResult = TRUE;

    CDC* pDC = GetDC();
    for (int col = 0; col < GetColumnCount(); col++)
        bResult = RedrawCell(row, col, pDC) && bResult;
    if (pDC) ReleaseDC(pDC);

    return bResult;
}

// redraw a complete column
BOOL CGridCtrl::RedrawColumn(int col)
{
    BOOL bResult = TRUE;

    CDC* pDC = GetDC();
    for (int row = 0; row < GetRowCount(); row++)
        bResult = RedrawCell(row, col, pDC) && bResult;
    if (pDC) ReleaseDC(pDC);

    return bResult;
}

// Sets the currently selected cell, returning the previous current cell
CCellID CGridCtrl::SetFocusCell(int nRow, int nCol)
{
    return SetFocusCell(CCellID(nRow, nCol));
}

CCellID CGridCtrl::SetFocusCell(CCellID cell)
{
    if (cell == m_idCurrentCell) 
        return m_idCurrentCell;

    CCellID idPrev = m_idCurrentCell;
    m_idCurrentCell = cell;

    if (IsValid(idPrev)) 
    {
        SendMessageToParent(idPrev.row, idPrev.col, GVN_SELCHANGING); 

        SetItemState(idPrev.row, idPrev.col, 
                     GetItemState(idPrev.row, idPrev.col) & ~GVIS_FOCUSED);
        RedrawCell(idPrev);

        if (idPrev.col != m_idCurrentCell.col)
            for (int row = 0; row < m_nFixedRows; row++)
                RedrawCell(row, idPrev.col);
        if (idPrev.row != m_idCurrentCell.row)
            for (int col = 0; col < m_nFixedCols; col++) 
                RedrawCell(idPrev.row, col);
    }

    if (IsValid(m_idCurrentCell)) {
        SetItemState(m_idCurrentCell.row, m_idCurrentCell.col, 
                     GetItemState(m_idCurrentCell.row, m_idCurrentCell.col) | GVIS_FOCUSED);

        RedrawCell(m_idCurrentCell);

        if (idPrev.col != m_idCurrentCell.col)
            for (int row = 0; row < m_nFixedRows; row++) 
                RedrawCell(row, m_idCurrentCell.col);
        if (idPrev.row != m_idCurrentCell.row)
            for (int col = 0; col < m_nFixedCols; col++) 
                RedrawCell(m_idCurrentCell.row, col);

        SendMessageToParent(m_idCurrentCell.row, m_idCurrentCell.col, GVN_SELCHANGED); 
    }

    return idPrev;
}

// Sets the range of currently selected cells
void CGridCtrl::SetSelectedRange(const CCellRange& Range, 
                                 BOOL bForceRepaint /* = FALSE */)
{
    SetSelectedRange(Range.GetMinRow(), Range.GetMinCol(), Range.GetMaxRow(), Range.GetMaxCol(),
                     bForceRepaint);
}

void CGridCtrl::SetSelectedRange(int nMinRow, int nMinCol, int nMaxRow, int nMaxCol,
                                 BOOL bForceRepaint /* = FALSE */)
{
    if (!m_bEnableSelection) return;

    CDC* pDC = NULL;
    if (bForceRepaint) pDC = GetDC();

    // Unselect all previously selected cells
	POSITION pos;
    for (pos = m_SelectedCellMap.GetStartPosition(); pos != NULL; )
    {
        DWORD key;
        CCellID cell;
        m_SelectedCellMap.GetNextAssoc(pos, key, (CCellID&)cell);

        // Reset the selection flag on the cell
        if (IsValid(cell)) {
            SetItemState(cell.row, cell.col, 
                         GetItemState(cell.row, cell.col) & ~GVIS_SELECTED);

            // If this is to be reselected, continue on past the redraw
            if (nMinRow <= cell.row && cell.row <= nMaxRow &&
                nMinCol <= cell.col && cell.col <= nMaxCol)
                    continue;
			SetItemBkColour(cell.row,cell.col,RGB(115,115,115));

            if (bForceRepaint && pDC)                    // Redraw NOW
                RedrawCell(cell.row, cell.col, pDC);
            else
                InvalidateCellRect(cell);                // Redraw at leisure
        }
    }
    
    // if any previous selected cells are to be retained (eg Ctrl is being held down)
    // then copy them to the newly created list, and mark all these cells as
    // selected
    for (pos = m_PrevSelectedCellMap.GetStartPosition(); pos != NULL; )
    {
        DWORD key;
        CCellID cell;
        m_PrevSelectedCellMap.GetNextAssoc(pos, key, (CCellID&)cell);

        if (!IsValid(cell)) continue;

        int nState = GetItemState(cell.row, cell.col);

        // Set state as Selected. This will add the cell to m_SelectedCells[]
        SetItemState(cell.row, cell.col, nState | GVIS_SELECTED);

        // Redraw (immediately or at leisure)
        if (bForceRepaint && pDC)
            RedrawCell(cell.row, cell.col, pDC);
        else
            InvalidateCellRect(cell);
    }

    // Now select all cells in the cell range specified. If the cell has already
    // been marked as selected (above) then ignore it.
    if (nMinRow >= 0 && nMinCol >= 0 && nMaxRow >= 0 && nMaxCol >= 0 &&
        nMaxRow < m_nRows && nMaxCol < m_nCols &&
         nMinRow <= nMaxRow && nMinCol <= nMaxCol)
    {
        for (int row = nMinRow; row <= nMaxRow; row++)
            for (int col = nMinCol; col <= nMaxCol; col++) 
            {
                int nState = GetItemState(row, col);
                if (nState & GVIS_SELECTED) {
                    continue;    // Already selected - ignore
                }

                // Add to list of selected cells
                CCellID cell(row, col);

                // Set state as Selected. This will add the cell to m_SelectedCells[]
                SetItemState(row, col, nState | GVIS_SELECTED);

                // Redraw (immediately or at leisure)
                if (bForceRepaint && pDC)
                    RedrawCell(row, col, pDC);
                else
                    InvalidateCellRect(cell);
            }
    }
//    TRACE(_T("%d cells selected.\n"), m_SelectedCellMap.GetCount());

    if (pDC != NULL) ReleaseDC(pDC);
} 

// selects all cells
void CGridCtrl::SelectAllCells()
{
    if (!m_bEnableSelection) return;

    SetSelectedRange(m_nFixedRows, m_nFixedCols, GetRowCount()-1, GetColumnCount()-1);
}

// selects columns
void CGridCtrl::SelectColumns(CCellID currentCell)
{
    if (!m_bEnableSelection) return;

    //if (currentCell.col == m_idCurrentCell.col) return;
    if (currentCell.col < m_nFixedCols) return;
    if (!IsValid(currentCell)) return;

    SetSelectedRange(GetFixedRowCount(), min(m_SelectionStartCell.col, currentCell.col), 
                     GetRowCount()-1,    max(m_SelectionStartCell.col, currentCell.col)); 
}

// selects rows
void CGridCtrl::SelectRows(CCellID currentCell)
{  
    if (!m_bEnableSelection) return;

    //if (currentCell.row; == m_idCurrentCell.row) return;
    if (currentCell.row < m_nFixedRows) return;
    if (!IsValid(currentCell)) return;

    SetSelectedRange(min(m_SelectionStartCell.row, currentCell.row), GetFixedColumnCount(), 
                     max(m_SelectionStartCell.row, currentCell.row), GetColumnCount()-1); 
}

// selects cells
void CGridCtrl::SelectCells(CCellID currentCell)
{
    if (!m_bEnableSelection) return;

    int row = currentCell.row;
    int col = currentCell.col;
    if (row < m_nFixedRows || col < m_nFixedCols) return;
    if (!IsValid(currentCell)) return;

    // Prevent unnecessary redraws
    //if (currentCell == m_LeftClickDownCell)  return;
    //else if (currentCell == m_idCurrentCell) return;

    SetSelectedRange(min(m_SelectionStartCell.row, row), min(m_SelectionStartCell.col, col), 
                     max(m_SelectionStartCell.row, row), max(m_SelectionStartCell.col, col)); 
}

void CGridCtrl::OnSelecting(const CCellID& currentCell)
{
    if (!m_bEnableSelection) return;

    switch(m_MouseMode)
    {
        case MOUSE_SELECT_ALL:     SelectAllCells();               break;
        case MOUSE_SELECT_COL:     SelectColumns(currentCell);     break;
        case MOUSE_SELECT_ROW:     SelectRows(currentCell);        break;
        case MOUSE_SELECT_CELLS:   SelectCells(currentCell);       break;
    }
}

// Get cell from point
CCellID CGridCtrl::GetCellFromPt(CPoint point, BOOL bAllowFixedCellCheck /*=TRUE*/) const
{  
    CCellID idTopLeft = GetTopleftNonFixedCell();
    CCellID cellID; // return value

    // calculate column index
    int fixedColWidth = GetFixedColumnWidth();

    if (point.x < 0 || (!bAllowFixedCellCheck && point.x < fixedColWidth)) // not in window
        cellID.col = -1;
    else if (point.x < fixedColWidth) // in fixed col
    {
        int xpos = 0;
		int col = 0;
        for ( ;col < m_nFixedCols; col++)
        {
            xpos += GetColumnWidth(col);
            if (xpos > point.x) break;
        }
        cellID.col = col;
    }
    else    // in non-fixed col
    {
        int xpos = fixedColWidth;
			int col = idTopLeft.col;
        for ( ;col < GetColumnCount(); col++)
        {
            xpos += GetColumnWidth(col);
            if (xpos > point.x) break;
        }

        if (col >= GetColumnCount())
            cellID.col = -1;
        else
            cellID.col = col;
    }
    
    // calculate row index
    int fixedRowHeight = GetFixedRowHeight();
    if (point.y < 0 || (!bAllowFixedCellCheck && point.y < fixedRowHeight)) // not in window
        cellID.row = -1;
    else if (point.y < fixedRowHeight) // in fixed col
    {
        int ypos = 0;
		int row = 0;
        for ( ;row < m_nFixedRows; row++)
        {
            ypos += GetRowHeight(row);
            if (ypos > point.y) break;
        }
        cellID.row = row;
    }
    else
    {
        int ypos = fixedRowHeight;
		int row = idTopLeft.row;
        for ( ;row < GetRowCount(); row++)
        {
            ypos += GetRowHeight(row);
            if (ypos > point.y) break;
        }

        if (row >= GetRowCount())
            cellID.row = -1;
        else
            cellID.row = row;
    }

    return cellID;
}

////////////////////////////////////////////////////////////////////////////////
// CGridCtrl cellrange functions

CCellID CGridCtrl::GetTopleftNonFixedCell() const
{
    int nVertScroll = GetScrollPos(SB_VERT), 
        nHorzScroll = GetScrollPos(SB_HORZ);

    int nColumn = m_nFixedCols, nRight = 0;
    while (nRight < nHorzScroll && nColumn < (GetColumnCount()-1))
        nRight += GetColumnWidth(nColumn++);

    int nRow = m_nFixedRows, nTop = 0;
    while (nTop < nVertScroll && nRow < (GetRowCount()-1))
        nTop += GetRowHeight(nRow++);

    //TRACE("TopLeft cell is row %d, col %d\n",nRow, nColumn);
    return CCellID(nRow, nColumn);
}

// This gets even partially visible cells
CCellRange CGridCtrl::GetVisibleNonFixedCellRange(LPRECT pRect /*=NULL*/) const
{
    CRect rect;
    GetClientRect(rect);

    CCellID idTopLeft = GetTopleftNonFixedCell();

    // calc bottom
    int bottom = GetFixedRowHeight();
	int i = idTopLeft.row;
    for ( ;i < GetRowCount(); i++)
    {
        bottom += GetRowHeight(i);
        if (bottom >= rect.bottom) {
            bottom = rect.bottom;
            break;
        }
    }                                
    int maxVisibleRow = min(i, GetRowCount() - 1);
    
    // calc right
    int right = GetFixedColumnWidth();
    for (i = idTopLeft.col; i < GetColumnCount(); i++)
    {
        right += GetColumnWidth(i);
        if (right >= rect.right) {
            right = rect.right;
            break;
        }
    }
    int maxVisibleCol = min(i, GetColumnCount() - 1);
    if (pRect) {
        pRect->left = pRect->top = 0;
        pRect->right = right;
        pRect->bottom = bottom;
    }

    return CCellRange(idTopLeft.row, idTopLeft.col, maxVisibleRow, maxVisibleCol);
}

// used by ResetScrollBars() - This gets only fully visible cells
CCellRange CGridCtrl::GetUnobstructedNonFixedCellRange() const
{
    CRect rect;
    GetClientRect(rect);

    CCellID idTopLeft = GetTopleftNonFixedCell();

    // calc bottom
    int bottom = GetFixedRowHeight();
	int i;
    for (i = idTopLeft.row; i < GetRowCount(); i++)
    {
        bottom += GetRowHeight(i);
        if (bottom >= rect.bottom) break;
    }
    int maxVisibleRow = min(i, GetRowCount() - 1);
    if (maxVisibleRow > 0 && bottom > rect.bottom) maxVisibleRow--;
    
    // calc right
    int right = GetFixedColumnWidth();
    for (i = idTopLeft.col; i < GetColumnCount(); i++)
    {
        right += GetColumnWidth(i);
        if (right >= rect.right) break;
    }
    int maxVisibleCol = min(i, GetColumnCount() - 1);
    if (maxVisibleCol > 0 && right > rect.right) maxVisibleCol--;


    return CCellRange(idTopLeft.row, idTopLeft.col, maxVisibleRow, maxVisibleCol);
}

// Returns the minimum bounding range of the current selection
// If no selection, then the returned CCellRange will be invalid
CCellRange CGridCtrl::GetSelectedCellRange() const
{
    CCellRange Selection(GetRowCount(), GetColumnCount(), -1,-1);

    for (POSITION pos = m_SelectedCellMap.GetStartPosition(); pos != NULL; )
    {
        DWORD key;
        CCellID cell;
        m_SelectedCellMap.GetNextAssoc(pos, key, (CCellID&)cell);

        Selection.SetMinRow( min(Selection.GetMinRow(), cell.row) );
        Selection.SetMinCol( min(Selection.GetMinCol(), cell.col) );
        Selection.SetMaxRow( max(Selection.GetMaxRow(), cell.row) );
        Selection.SetMaxCol( max(Selection.GetMaxCol(), cell.col) );
    }

    return Selection;
}

// Returns ALL the cells in the grid
CCellRange CGridCtrl::GetCellRange() const
{
    return CCellRange(0, 0, GetRowCount() - 1, GetColumnCount() - 1);
}

void CGridCtrl::ResetSelectedRange()
{
    SetSelectedRange(-1,-1,-1,-1);
    SetFocusCell(-1,-1);
}

// Get/Set scroll position using 32 bit functions
int CGridCtrl::GetScrollPos32(int nBar, BOOL bGetTrackPos /* = FALSE */)
{
    SCROLLINFO si;
    si.cbSize = sizeof(SCROLLINFO);

    if (bGetTrackPos)
    {
        if (GetScrollInfo(nBar, &si, SIF_TRACKPOS))
            return si.nTrackPos;
    }
    else 
    {
        if (GetScrollInfo(nBar, &si, SIF_POS))
            return si.nPos;
    }

    return 0;
}

BOOL CGridCtrl::SetScrollPos32(int nBar, int nPos, BOOL bRedraw /* = TRUE */)
{
    SCROLLINFO si;
    si.cbSize = sizeof(SCROLLINFO);
    si.fMask  = SIF_POS;
    si.nPos   = nPos;
    return SetScrollInfo(nBar, &si, bRedraw);
}

void CGridCtrl::ResetScrollBars()
{
    if (!m_bAllowDraw || !::IsWindow(GetSafeHwnd())) 
        return;

    CRect rect;
    GetClientRect(rect);
    rect.left  += GetFixedColumnWidth();
    rect.top   += GetFixedRowHeight();
    if (rect.left >= rect.right || rect.top >= rect.bottom) return;

    CRect VisibleRect(GetFixedColumnWidth(), GetFixedRowHeight(), rect.right, rect.bottom);
    CRect VirtualRect(GetFixedColumnWidth(), GetFixedRowHeight(), GetVirtualWidth(), GetVirtualHeight());

    CCellRange visibleCells = GetUnobstructedNonFixedCellRange();
    if (!IsValid(visibleCells)) return;

    SCROLLINFO si;
    si.cbSize = sizeof(SCROLLINFO);
    si.fMask  = SIF_PAGE;
    si.nPage  = VisibleRect.Width();    SetScrollInfo(SB_HORZ, &si, FALSE); 
    si.nPage  = VisibleRect.Height();   SetScrollInfo(SB_VERT, &si, FALSE); 

    if (VisibleRect.Height() < VirtualRect.Height())
        m_nVScrollMax = VirtualRect.Height()-1; //+ GetRowHeight(visibleCells.GetTopLeft().row);
    else
        m_nVScrollMax = 0;

    if (VisibleRect.Width() < VirtualRect.Width())
        m_nHScrollMax = VirtualRect.Width()-1; //+ GetColumnWidth(visibleCells.GetTopLeft().col);
    else
        m_nHScrollMax = 0;

    ASSERT(m_nVScrollMax < INT_MAX && m_nHScrollMax < INT_MAX); // This should be fine :)
    SetScrollRange(SB_VERT, 0, m_nVScrollMax, TRUE);
    SetScrollRange(SB_HORZ, 0, m_nHScrollMax, TRUE);
}

////////////////////////////////////////////////////////////////////////////////////
// Row/Column position functions

// returns the top left point of the cell. Returns FALSE if cell not visible.
BOOL CGridCtrl::GetCellOrigin(int nRow, int nCol, LPPOINT p) const
{
    int i;

    if (!IsValid(nRow, nCol)) return FALSE;

    CCellID idTopLeft;
    if (nCol >= m_nFixedCols || nRow >= m_nFixedRows)
        idTopLeft = GetTopleftNonFixedCell();

    if ((nRow >= m_nFixedRows && nRow < idTopLeft.row) ||
        (nCol>= m_nFixedCols && nCol < idTopLeft.col))
        return FALSE;

    p->x = 0;
    if (nCol < m_nFixedCols)                      // is a fixed column
        for (i = 0; i < nCol; i++)
            p->x += GetColumnWidth(i);
    else {                                        // is a scrollable data column
        for (i = 0; i < m_nFixedCols; i++)
            p->x += GetColumnWidth(i);
        for (i = idTopLeft.col; i < nCol; i++)
            p->x += GetColumnWidth(i);
    }

    p->y = 0;
    if (nRow < m_nFixedRows)                      // is a fixed row
        for (i = 0; i < nRow; i++)
            p->y += GetRowHeight(i);
    else {                                        // is a scrollable data row
        for (i = 0; i < m_nFixedRows; i++)
            p->y += GetRowHeight(i);
        for (i = idTopLeft.row; i < nRow; i++)
            p->y += GetRowHeight(i);
    }

    return TRUE;
}

BOOL CGridCtrl::GetCellOrigin(const CCellID& cell, LPPOINT p) const
{
    return GetCellOrigin(cell.row, cell.col, p);
}

// Returns the bounding box of the cell
BOOL CGridCtrl::GetCellRect(const CCellID& cell, LPRECT pRect) const
{
    return GetCellRect(cell.row, cell.col, pRect);
}

BOOL CGridCtrl::GetCellRect(int nRow, int nCol, LPRECT pRect) const
{    
    CPoint CellOrigin;
    if (!GetCellOrigin(nRow, nCol, &CellOrigin)) return FALSE;

    pRect->left   = CellOrigin.x;
    pRect->top    = CellOrigin.y;
    pRect->right  = CellOrigin.x + GetColumnWidth(nCol)-1;
    pRect->bottom = CellOrigin.y + GetRowHeight(nRow)-1;

    //TRACE("Row %d, col %d: L %d, T %d, W %d, H %d:  %d,%d - %d,%d\n",
    //      nRow,nCol, CellOrigin.x, CellOrigin.y, GetColumnWidth(nCol), GetRowHeight(nRow),
    //      pRect->left, pRect->top, pRect->right, pRect->bottom);

    return TRUE;
}

// Returns the bounding box of a range of cells
BOOL CGridCtrl::GetCellRangeRect(const CCellRange& cellRange, LPRECT lpRect) const
{
    CPoint MinOrigin,MaxOrigin;

    if (!GetCellOrigin(cellRange.GetMinRow(), cellRange.GetMinCol(), &MinOrigin)) 
        return FALSE;
    if (!GetCellOrigin(cellRange.GetMaxRow(), cellRange.GetMaxCol(), &MaxOrigin)) 
        return FALSE;

    lpRect->left   = MinOrigin.x;
    lpRect->top    = MinOrigin.y;
    lpRect->right  = MaxOrigin.x + GetColumnWidth(cellRange.GetMaxCol()-1);
    lpRect->bottom = MaxOrigin.y + GetRowHeight(cellRange.GetMaxRow()-1);

    return TRUE;
}

////////////////////////////////////////////////////////////////////////////////////
// Grid attribute functions

LRESULT CGridCtrl::OnSetFont(WPARAM hFont, LPARAM /*lParam */)
{
    LRESULT result = Default();

    // Get the logical font
    LOGFONT lf;
    if (!GetObject((HFONT) hFont, sizeof(LOGFONT), &lf))
        return result;
    
    // Store font as the global default
    memcpy(&m_Logfont, &lf, sizeof(LOGFONT));
    
    // reset all cells' fonts
    for (int row = 0; row < GetRowCount(); row++)
        for (int col = 0; col < GetColumnCount(); col++)
            SetItemFont(row, col, &lf);

    // Get the font size and hence the default cell size
    CDC* pDC = GetDC();
    if (pDC) 
    {
        m_Font.DeleteObject();
        m_Font.CreateFontIndirect(&m_Logfont);
        CFont* pOldFont = pDC->SelectObject(&m_Font);
        
        TEXTMETRIC tm;
        pDC->GetTextMetrics(&tm);
        
        m_nMargin = pDC->GetTextExtent(_T(" "),1).cx;
        pDC->SelectObject(pOldFont);
        ReleaseDC(pDC);

        m_nDefCellHeight = tm.tmHeight+tm.tmExternalLeading + 2*m_nMargin;
        m_nDefCellWidth  = tm.tmAveCharWidth*12 + 2*m_nMargin;
    }

    if (::IsWindow(GetSafeHwnd())) 
        Invalidate();

    return result;
}

LRESULT CGridCtrl::OnGetFont(WPARAM /*wParam*/, LPARAM /*lParam*/)
{
    return (LRESULT) (HFONT) m_Font;
}

BOOL CGridCtrl::OnSetCursor(CWnd* pWnd, UINT nHitTest, UINT message) 
{
    if (nHitTest == HTCLIENT)
    {
        switch (m_MouseMode) 
        {
            case MOUSE_OVER_COL_DIVIDE: SetCursor(::LoadCursor(NULL, IDC_SIZEWE)); break;
            case MOUSE_OVER_ROW_DIVIDE: SetCursor(::LoadCursor(NULL, IDC_SIZENS)); break;
            default:                    SetCursor(::LoadCursor(NULL, IDC_ARROW));
        }
        return TRUE;
    }

    return CWnd::OnSetCursor(pWnd, nHitTest, message);
}

////////////////////////////////////////////////////////////////////////////////////
// Row/Column count functions

BOOL CGridCtrl::SetFixedRowCount(int nFixedRows)
{
    ASSERT(nFixedRows >= 0);

    if (nFixedRows > GetRowCount()) 
        if (!SetRowCount(nFixedRows)) return FALSE;

    if (m_idCurrentCell.row < nFixedRows)
        SetFocusCell(-1,-1);

    m_nFixedRows = nFixedRows;

    if (GetSafeHwnd() && m_bAllowDraw)
        Invalidate();

    return TRUE;
}

BOOL CGridCtrl::SetFixedColumnCount(int nFixedCols)
{
    ASSERT(nFixedCols >= 0);
    if (nFixedCols > GetColumnCount())
        if (!SetColumnCount(nFixedCols)) return FALSE;

    if (m_idCurrentCell.col < nFixedCols)
        SetFocusCell(-1,-1);

    m_nFixedCols = nFixedCols;

    if (GetSafeHwnd() && m_bAllowDraw)
        Invalidate();

    return TRUE;
}

BOOL CGridCtrl::SetRowCount(int nRows)
{
    ASSERT(nRows >= 0);
    if (nRows == GetRowCount()) return TRUE;

    if (nRows < m_nFixedRows) 
        m_nFixedRows = nRows;

    if (m_idCurrentCell.row >= nRows)
        SetFocusCell(-1,-1);

    int addedRows = nRows - GetRowCount();

    // If we are about to lose rows, then we need to delete the GridCell objects 
    // in each column within each row
    if (addedRows < 0) {
        for (int row = nRows; row < m_nRows; row++)
        {
            // Delete cells
            for (int col = 0; col < m_nCols; col++) 
            {
                CGridCell* pCell = GetCell(row, col);
                if (pCell) {
                    EmptyCell(pCell, row, col);
                    delete pCell;
                }
            }
            // Delete rows
            GRID_ROW* pRow = m_RowData[row];
            if (pRow) delete pRow;
        }
    }

    // Change the number of rows.
    m_nRows = nRows;
    m_RowData.SetSize(m_nRows);
    m_arRowHeights.SetSize(nRows);

    // If we have just added rows, we need to construct new elements for each cell
    // and set the default row height
    if (addedRows > 0) {
        // initialize row heights and data
        int startRow = nRows - addedRows;
        for (int row = startRow; row < GetRowCount(); row++) {
            m_arRowHeights[row] = m_nDefCellHeight;
            m_RowData[row] = new GRID_ROW;
            m_RowData[row]->SetSize(m_nCols);
            for (int col = 0; col < m_nCols; col++)
            {
                GRID_ROW* pRow = m_RowData[row];
                if (pRow) pRow->SetAt(col, CreateCell(row, col));
            }
        }
    }
    
    if (GetSafeHwnd() && m_bAllowDraw)
    {
  //      ResetScrollBars();
        Invalidate();
    }
    return TRUE;
}

BOOL CGridCtrl::SetColumnCount(int nCols)
{
    ASSERT(nCols >= 0);

    if (nCols == GetColumnCount()) return TRUE;

    if (nCols < m_nFixedCols) 
        m_nFixedCols = nCols;

    if (m_idCurrentCell.col >= nCols)
        SetFocusCell(-1,-1);

    int addedCols = nCols - GetColumnCount();

    // If we are about to lose columns, then we need to delete the GridCell objects 
    // within each column
    if (addedCols < 0) {
        for (int row = 0; row < m_nRows; row++)
            for (int col = nCols; col < GetColumnCount(); col++)
            {
                CGridCell* pCell = GetCell(row, col);
                if (pCell) {
                    EmptyCell(pCell, row, col);
                    delete pCell;
                }
            }
    }

    // Change the number of columns.
    m_nCols = nCols;
    m_arColWidths.SetSize(nCols);

    // Change the number of columns in each row.
    for (int i = 0; i < m_nRows; i++)
        if (m_RowData[i]) m_RowData[i]->SetSize(nCols);

    // If we have just added columns, we need to construct new elements for each cell
    // and set the default column width
    if (addedCols > 0)
    {
        // initialized column widths
        int startCol = nCols - addedCols;
		int col;
        for ( col= startCol; col < GetColumnCount(); col++)
            m_arColWidths[col] = m_nDefCellWidth;

        // initialise column data
		int row = 0;
        for (; row < m_nRows; row++)
            for (col = startCol; col < GetColumnCount(); col++)
            {
                GRID_ROW* pRow = m_RowData[row];
                if (pRow) pRow->SetAt(col, CreateCell(row,col));
            }
    }
    
    if (GetSafeHwnd() && m_bAllowDraw)
    {
//        ResetScrollBars();
        Invalidate();
    }
    return TRUE;
}

// Insert a column at a given position, or add to end of columns (if nColumn = -1)
int CGridCtrl::InsertColumn(LPCTSTR strHeading, 
                            UINT nFormat /* = DT_CENTER|DT_VCENTER|DT_SINGLELINE */,
                            int nColumn  /* = -1 */)
{
    // If the insertion is for a specific column, check it's within range.
    if (nColumn >= 0 && nColumn >= GetColumnCount())
        return -1;

    ResetSelectedRange();

    // Gotta be able to at least _see_ some of the column.
    if (m_nRows < 1)
        SetRowCount(1);    

    if (nColumn < 0)
    {
        nColumn = m_nCols;
        m_arColWidths.Add(0);
        for (int row = 0; row < m_nRows; row++) 
        {
            GRID_ROW* pRow = m_RowData[row];
            if (!pRow) return -1;
            pRow->Add(CreateCell(row, nColumn));
        }
    } 
    else
    {
        m_arColWidths.InsertAt(nColumn, (int)0,NULL);
		int row;
        for ( row = 0; row < m_nRows; row++) 
        {
            GRID_ROW* pRow = m_RowData[row];
            if (!pRow) return -1;
            pRow->InsertAt(nColumn, CreateCell(row, nColumn));
        }
    }

    m_nCols++;

    // Initialise column data
    SetItemText(0, nColumn, strHeading);
    for (int row = 0; row < m_nRows; row++) 
    {
        SetItemFormat(row, nColumn, nFormat);
    }

    // initialized column width
    m_arColWidths[nColumn] = GetTextExtent(strHeading).cx;

    if (m_idCurrentCell.col != -1 && nColumn < m_idCurrentCell.col)
        m_idCurrentCell.col++;
    
    ResetScrollBars();

    return nColumn;
}

// Insert a row at a given position, or add to end of rows (if nRow = -1)
int CGridCtrl::InsertRow(LPCTSTR strHeading, int nRow /* = -1 */)
{
    // If the insertion is for a specific row, check it's within range.
    if (nRow >= 0 && nRow >= GetRowCount())
        return -1;

    ResetSelectedRange();

    // Gotta be able to at least _see_ some of the row.
    if (m_nCols < 1) 
        SetColumnCount(1);    

    // Adding a row to the bottom
    if (nRow < 0) 
    {
        nRow = m_nRows;
        m_arRowHeights.Add(0);
        m_RowData.Add(new GRID_ROW);
    } 
    else 
    {
        m_arRowHeights.InsertAt(nRow, (int)0,NULL);
        m_RowData.InsertAt(nRow, new GRID_ROW);
    }
    
    m_nRows++;
    m_RowData[nRow]->SetSize(m_nCols);

    // Initialise cell data
    for (int col = 0; col < m_nCols; col++)
    {
        GRID_ROW* pRow = m_RowData[nRow];
        if (!pRow) return -1;
        pRow->SetAt(col, CreateCell(nRow, col));
    }

    // Set row title
    SetItemText(nRow, 0, strHeading);

    // initialized row height
    m_arRowHeights[nRow] = GetTextExtent(strHeading).cy;

    if (m_idCurrentCell.row != -1 && nRow < m_idCurrentCell.row)
        m_idCurrentCell.row++;
    
    ResetScrollBars();

    return nRow;
}

// Creates a new grid cell and performs any necessary initialisation
CGridCell* CGridCtrl::CreateCell(int nRow, int nCol)
{
    CGridCell* pCell = new CGridCell;
    if (!pCell)
        return NULL;
    
    // Make format same as cell above
    if (nRow > 0 && nCol > 0 && nCol < m_nCols)
        pCell->nFormat = GetItemFormat(nRow-1, nCol);
 
    // Make font default grid font
    memcpy(&(pCell->lfFont), &m_Logfont, sizeof(LOGFONT));
    
    return pCell;
}

// Performs any cell cleanup necessary to maintain grid integrity
void CGridCtrl::EmptyCell(CGridCell* pCell, int nRow, int nCol)
{
    // Set the cells state to 0. If the cell is selected, this
    // will remove the cell from the selected list.
    SetItemState(nRow, nCol, 0);

    // Empty strings
    pCell->szText.Empty();
}

BOOL CGridCtrl::DeleteColumn(int nColumn)
{
    if (nColumn < 0 || nColumn >= GetColumnCount())
        return FALSE;

    ResetSelectedRange();

    for (int row = 0; row < GetRowCount(); row++) 
    {
        GRID_ROW* pRow = m_RowData[row];
        if (!pRow) return FALSE;

        CGridCell* pCell = pRow->GetAt(nColumn);
        if (pCell) {
            EmptyCell(pCell, row, nColumn);
            delete pCell;
        }
        pRow->RemoveAt(nColumn);
    }
    m_arColWidths.RemoveAt(nColumn);
    m_nCols--;
    if (nColumn < m_nFixedCols) m_nFixedCols--;

    if (nColumn == m_idCurrentCell.col)
        m_idCurrentCell.row = m_idCurrentCell.col = -1;
    else if (nColumn < m_idCurrentCell.col)
        m_idCurrentCell.col--;

    ResetScrollBars();

    return FALSE;
}

BOOL CGridCtrl::DeleteRow(int nRow)
{
    if ( nRow < 0 || nRow >= GetRowCount())
        return FALSE;

    GRID_ROW* pRow = m_RowData[nRow];
    if (!pRow) return FALSE;

    ResetSelectedRange();

    for (int col = 0; col < GetColumnCount(); col++) 
    {
        CGridCell* pCell = pRow->GetAt(col);
        if (pCell) {
            EmptyCell(pCell, nRow, col);
            delete pCell;
        }
    }
    delete pRow;
    m_RowData.RemoveAt(nRow);
    m_arRowHeights.RemoveAt(nRow);

    m_nRows--;
    if (nRow < m_nFixedRows) m_nFixedRows--;

    if (nRow == m_idCurrentCell.row)
        m_idCurrentCell.row = m_idCurrentCell.col = -1;
    else if (nRow < m_idCurrentCell.row)
        m_idCurrentCell.row--;

    ResetScrollBars();

    return TRUE;
}

// Handy function that removes all non-fixed rows
BOOL CGridCtrl::DeleteNonFixedRows()
{
    int nFixed = GetFixedRowCount();
    int nCount = GetRowCount();

    // Delete all data rows
    for (int nRow = nCount; nRow >= nFixed; nRow--) 
        DeleteRow(nRow);

    return TRUE;
}

// Removes all rows, columns and data from the grid.
BOOL CGridCtrl::DeleteAllItems()
{
    ResetSelectedRange();

    m_arColWidths.RemoveAll();
    m_arRowHeights.RemoveAll();

    // Delete all cells in the grid
    for (int row = 0; row < m_nRows; row++) 
    {
        GRID_ROW* pRow = m_RowData[row];
        if (!pRow) continue;
        for (int col = 0; col < m_nCols; col++)
        {
            CGridCell* pCell = pRow->GetAt(col);
            if (pCell) {
                EmptyCell(pCell, row, col);  // TODO - this is a bit of a performance hit.
                delete pCell;                // better to call m_SelectedCells.RemoveAll()
            }                                // instead. This is safer for changes though.
        }
        delete pRow;
    }

    // Remove all rows
    m_RowData.RemoveAll();

    m_idCurrentCell.row = m_idCurrentCell.col = -1;
    m_nRows = m_nFixedRows = m_nCols = m_nFixedCols = 0;

    ResetScrollBars();

    return TRUE;
}

/////////////////////////////////////////////////////////////////////////////
// CGridCtrl data functions

// Set CListCtrl::GetNextItem for details
CCellID CGridCtrl::GetNextItem(CCellID& cell, int nFlags) const
{
    if ((nFlags & GVNI_BELOW) && (nFlags & GVNI_TORIGHT))
    {
        int row = cell.row;
        if (row <= 0) row = GetFixedRowCount();
        for (; row < GetRowCount(); row++)
        {
            int col = cell.col + 1;
            if (col <= 0) col = GetFixedColumnCount();
            for (; col < GetColumnCount(); col++) 
            {
                int nState = GetItemState(row, col);
                if (nFlags & GVNI_DROPHILITED && nState & GVIS_DROPHILITED) 
                    return CCellID(row, col);
                if (nFlags & GVNI_FOCUSED && nState & GVIS_FOCUSED)     
                    return CCellID(row, col);
                if (nFlags & GVNI_SELECTED && nState & GVIS_SELECTED)   
                    return CCellID(row, col);
                if (nFlags & GVNI_READONLY && nState & GVIS_READONLY)   
                    return CCellID(row, col);    
            }
        }
    }
    if (nFlags & GVNI_ABOVE) 
    {
        for (int row = cell.row-1; row >= GetFixedRowCount(); row--) 
        {
            int nState = GetItemState(row, cell.col);
            if (nFlags & GVNI_DROPHILITED && nState & GVIS_DROPHILITED) return CCellID(row, cell.col);
            if (nFlags & GVNI_FOCUSED && nState & GVIS_FOCUSED)     return CCellID(row, cell.col);
            if (nFlags & GVNI_SELECTED && nState & GVIS_SELECTED)   return CCellID(row, cell.col);
            if (nFlags & GVNI_READONLY && nState & GVIS_READONLY)   return CCellID(row, cell.col);
        }
    }
    else if (nFlags & GVNI_BELOW)
    {
        for (int row = cell.row+1; row < GetRowCount(); row++) 
        {
            int nState = GetItemState(row, cell.col);
            if (nFlags & GVNI_DROPHILITED && nState & GVIS_DROPHILITED) return CCellID(row, cell.col);
            if (nFlags & GVNI_FOCUSED && nState & GVIS_FOCUSED)     return CCellID(row, cell.col);
            if (nFlags & GVNI_SELECTED && nState & GVIS_SELECTED)   return CCellID(row, cell.col);
            if (nFlags & GVNI_READONLY && nState & GVIS_READONLY)   return CCellID(row, cell.col);
        }
    } 
    else if (nFlags & GVNI_TOLEFT)
    {
        for (int col = cell.col-1; col >= GetFixedColumnCount(); col--) 
        {
            int nState = GetItemState(cell.row, col);
            if (nFlags & GVNI_DROPHILITED && nState & GVIS_DROPHILITED) return CCellID(cell.row, col);
            if (nFlags & GVNI_FOCUSED && nState & GVIS_FOCUSED)     return CCellID(cell.row, col);
            if (nFlags & GVNI_SELECTED && nState & GVIS_SELECTED)   return CCellID(cell.row, col);
            if (nFlags & GVNI_READONLY && nState & GVIS_READONLY)   return CCellID(cell.row, col);
       }
    }
    else if (nFlags & GVNI_TORIGHT) 
    {
        for (int col = cell.col+1; col < GetColumnCount(); col++) 
        {
            int nState = GetItemState(cell.row, col);
            if (nFlags & GVNI_DROPHILITED && nState & GVIS_DROPHILITED) return CCellID(cell.row, col);
            if (nFlags & GVNI_FOCUSED && nState & GVIS_FOCUSED)     return CCellID(cell.row, col);
            if (nFlags & GVNI_SELECTED && nState & GVIS_SELECTED)   return CCellID(cell.row, col);
            if (nFlags & GVNI_READONLY && nState & GVIS_READONLY)   return CCellID(cell.row, col);
        }
    }

    return CCellID(-1, -1);
}

// Sorts on a given column using the cell text
BOOL CGridCtrl::SortTextItems(int nCol, BOOL bAscending)
{
    ResetSelectedRange();
    SetFocusCell(-1,-1);
    return SortTextItems(nCol, bAscending, GetFixedRowCount(),-1);
}

// recursive sort implementation
BOOL CGridCtrl::SortTextItems(int nCol, BOOL bAscending, int low, int high)
{
    if (nCol >= GetColumnCount()) return FALSE;

    if (high == -1) high = GetRowCount() - 1;

    int lo = low;
    int hi = high;

    if( hi <= lo ) return FALSE;

    CString midItem = GetItemText( (lo+hi)/2, nCol );

    // loop through the list until indices cross
    while( lo <= hi )
    {
        // Find the first element that is greater than or equal to the partition 
        // element starting from the left Index.
        if( bAscending )
            while (lo < high  && GetItemText(lo, nCol) < midItem)
                ++lo;
        else
            while (lo < high && GetItemText(lo, nCol) > midItem)
                ++lo;

        // Find an element that is smaller than or equal to  the partition 
        // element starting from the right Index.
        if( bAscending )
            while (hi > low && GetItemText(hi, nCol) > midItem)
                --hi;
        else
            while (hi > low && GetItemText(hi, nCol) < midItem)
                --hi;

        // If the indexes have not crossed, swap if the items are not equal
        if (lo <= hi)
        {
            // swap only if the items are not equal
            if (GetItemText(lo, nCol) != GetItemText(hi, nCol))
            {
                for (int col = 0; col < GetColumnCount(); col++)
                {
                    CGridCell *pCell = GetCell(lo, col);
                    SetCell(lo, col, GetCell(hi, col));
                    SetCell(hi, col, pCell);
                }
                UINT nRowHeight = m_arRowHeights[lo];
                m_arRowHeights[lo] = m_arRowHeights[hi];
                m_arRowHeights[hi] = nRowHeight;
            }

            ++lo;
            --hi;
        }
    }

    // If the right index has not reached the left side of array
    // must now sort the left partition.
    if( low < hi )
        SortTextItems(nCol, bAscending, low, hi);

    // If the left index has not reached the right side of array
    // must now sort the right partition.
    if( lo < high )
        SortTextItems(nCol, bAscending, lo, high);

    return TRUE;
}

// Sorts on a given column using the supplied compare function (see CListCtrl::SortItems)
BOOL CGridCtrl::SortItems(PFNLVCOMPARE pfnCompare, int nCol, BOOL bAscending, 
                          LPARAM data /* = 0 */)
{
    ResetSelectedRange();
    SetFocusCell(-1,-1);
    return SortItems(pfnCompare, nCol, bAscending, data, GetFixedRowCount(), -1);
}

// recursive sort implementation
BOOL CGridCtrl::SortItems(PFNLVCOMPARE pfnCompare, int nCol, BOOL bAscending, LPARAM data,
                          int low, int high)
{
    if (nCol >= GetColumnCount()) return FALSE;

    if (high == -1) high = GetRowCount() - 1;

    int lo = low;
    int hi = high;

    if( hi <= lo ) return FALSE;

    LPARAM midItem = GetItemData( (lo+hi)/2, nCol );

    // loop through the list until indices cross
    while( lo <= hi )
    {
        // Find the first element that is greater than or equal to the partition 
        // element starting from the left Index.
        if( bAscending )
            while (lo < high  && pfnCompare(GetItemData(lo, nCol), midItem, data) < 0)
                ++lo;
        else
            while (lo < high && pfnCompare(GetItemData(lo, nCol), midItem, data) > 0)
                ++lo;

        // Find an element that is smaller than or equal to  the partition 
        // element starting from the right Index.
        if( bAscending )
            while (hi > low && pfnCompare(GetItemData(hi, nCol), midItem, data) > 0)
                --hi;
        else
            while (hi > low && pfnCompare(GetItemData(hi, nCol), midItem, data) < 0)
                --hi;

        // If the indexes have not crossed, swap if the items are not equal
        if (lo <= hi)
        {
            // swap only if the items are not equal
            if (pfnCompare(GetItemData(lo, nCol), GetItemData(hi, nCol), data) != 0)
            {
                for (int col = 0; col < GetColumnCount(); col++)
                {
                    CGridCell *pCell = GetCell(lo, col);
                    SetCell(lo, col, GetCell(hi, col));
                    SetCell(hi, col, pCell);
                }
                UINT nRowHeight = m_arRowHeights[lo];
                m_arRowHeights[lo] = m_arRowHeights[hi];
                m_arRowHeights[hi] = nRowHeight;
            }

            ++lo;
            --hi;
        }
    }

    // If the right index has not reached the left side of array
    // must now sort the left partition.
    if( low < hi )
        SortItems(pfnCompare, nCol, bAscending, data, low, hi);

    // If the left index has not reached the right side of array
    // must now sort the right partition.
    if( lo < high )
        SortItems(pfnCompare, nCol, bAscending, data, lo, high);

    return TRUE;
}

/////////////////////////////////////////////////////////////////////////////
// CGridCtrl data functions

BOOL CGridCtrl::SetItem(const GV_ITEM* pItem)
{
    if (!pItem) return FALSE;
    CGridCell* pCell = GetCell(pItem->row, pItem->col);
    if (!pCell) return FALSE;

    if (pItem->mask & GVIF_TEXT)   pCell->szText  = pItem->szText;
    if (pItem->mask & GVIF_PARAM)  pCell->lParam  = pItem->lParam;
    if (pItem->mask & GVIF_IMAGE)  pCell->iImage  = pItem->iImage;
    if (pItem->mask & GVIF_STATE)  pCell->state   = pItem->state;
    if (pItem->mask & GVIF_FORMAT) pCell->nFormat = pItem->nFormat;
    if (pItem->mask & GVIF_BKCLR)  pCell->crBkClr = pItem->crBkClr;
    if (pItem->mask & GVIF_FGCLR)  pCell->crFgClr = pItem->crFgClr;
    if (pItem->mask & GVIF_FONT)   memcpy(&(pCell->lfFont), &(pItem->lfFont), sizeof(LOGFONT));

    return TRUE;
}

BOOL CGridCtrl::GetItem(GV_ITEM* pItem)
{
    if (!pItem) return FALSE;
    CGridCell* pCell = GetCell(pItem->row, pItem->col);
    if (!pCell) return FALSE;

    if (pItem->mask & GVIF_TEXT)
		pItem->szText  = GetItemText(pItem->row, pItem->col);
    if (pItem->mask & GVIF_PARAM)	pItem->lParam  = pCell->lParam;
    if (pItem->mask & GVIF_IMAGE)	pItem->iImage  = pCell->iImage;
    if (pItem->mask & GVIF_STATE)	pItem->state   = pCell->state;
    if (pItem->mask & GVIF_FORMAT)	pItem->nFormat = pCell->nFormat;
    if (pItem->mask & GVIF_BKCLR)	pItem->crBkClr = pCell->crBkClr;
    if (pItem->mask & GVIF_FGCLR)	pItem->crFgClr = pCell->crFgClr;
    if (pItem->mask & GVIF_FONT)	memcpy(&(pItem->lfFont), &(pCell->lfFont), sizeof(LOGFONT));

    return TRUE;
}

BOOL CGridCtrl::SetItemText(int nRow, int nCol, LPCTSTR str)
{
    CGridCell* pCell = GetCell(nRow, nCol);
    if (!pCell) return FALSE;

    pCell->szText = str;
    return TRUE;
}

BOOL CGridCtrl::SetItemData(int nRow, int nCol, LPARAM lParam)
{
    CGridCell* pCell = GetCell(nRow, nCol);
    if (!pCell) return FALSE;

    pCell->lParam = lParam;
    return TRUE;
}

LPARAM CGridCtrl::GetItemData(int nRow, int nCol) const
{    
    CGridCell* pCell = GetCell(nRow, nCol);
    if (!pCell) return (LPARAM) 0;

    return pCell->lParam;
}

BOOL CGridCtrl::SetItemImage(int nRow, int nCol, int iImage)
{
    CGridCell* pCell = GetCell(nRow, nCol);
    if (!pCell) return FALSE;

    pCell->iImage = iImage;
    return TRUE;
}

int CGridCtrl::GetItemImage(int nRow, int nCol) const
{
    CGridCell* pCell = GetCell(nRow, nCol);
    ASSERT(pCell);
    if (!pCell) return -1;

    return pCell->iImage;
}

BOOL CGridCtrl::SetItemState(int nRow, int nCol, UINT state)
{
    CGridCell* pCell = GetCell(nRow, nCol);
    ASSERT(pCell);
    if (!pCell) return FALSE;

    // If the cell is being unselected, remove it from the selected list
    if ((pCell->state & GVIS_SELECTED) && !(state & GVIS_SELECTED))
    {
        CCellID cell;
        DWORD key = MAKELONG(nRow, nCol);

        if (m_SelectedCellMap.Lookup(key, (CCellID&)cell))
            m_SelectedCellMap.RemoveKey(key);
    }

    // If cell is being selected, add it to the list of selected cells
    else if (!(pCell->state & GVIS_SELECTED) && (state & GVIS_SELECTED))
    {
        CCellID cell(nRow, nCol);
        m_SelectedCellMap.SetAt(MAKELONG(nRow, nCol), cell);
    }

    // Set the cell's state
    pCell->state = state;

    return TRUE;
}

UINT CGridCtrl::GetItemState(int nRow, int nCol) const
{
    CGridCell* pCell = GetCell(nRow, nCol);
    ASSERT(pCell);
    if (!pCell) return 0;

    return pCell->state;
}

BOOL CGridCtrl::SetItemFormat(int nRow, int nCol, UINT nFormat)
{
    CGridCell* pCell = GetCell(nRow, nCol);
    ASSERT(pCell);
    if (!pCell) return FALSE;

    pCell->nFormat = nFormat;
    return TRUE;
}

UINT CGridCtrl::GetItemFormat(int nRow, int nCol) const
{
    CGridCell* pCell = GetCell(nRow, nCol);
    ASSERT(pCell);
    if (!pCell) return 0;

    return pCell->nFormat;
}

BOOL CGridCtrl::SetItemBkColour(int nRow, int nCol, COLORREF cr /* = CLR_DEFAULT */)
{
    CGridCell* pCell = GetCell(nRow, nCol);
    ASSERT(pCell);
    if (!pCell) return FALSE;

    pCell->crBkClr = cr;
    return TRUE;
}

COLORREF CGridCtrl::GetItemBkColour(int nRow, int nCol) const
{
    CGridCell* pCell = GetCell(nRow, nCol);
    ASSERT(pCell);
    if (!pCell) return 0;

    return pCell->crBkClr;
}

BOOL CGridCtrl::SetItemFgColour(int nRow, int nCol, COLORREF cr /* = CLR_DEFAULT */)
{
    CGridCell* pCell = GetCell(nRow, nCol);
    ASSERT(pCell);
    if (!pCell) return FALSE;

    pCell->crFgClr = cr;
    return TRUE;
}

COLORREF CGridCtrl::GetItemFgColour(int nRow, int nCol) const
{
    CGridCell* pCell = GetCell(nRow, nCol);
    ASSERT(pCell);
    if (!pCell) return 0;

    return pCell->crFgClr;
}

BOOL CGridCtrl::SetItemFont(int nRow, int nCol, LOGFONT* lf)
{
    CGridCell* pCell = GetCell(nRow, nCol);
    ASSERT(pCell);
    if (!pCell) return FALSE;

    memcpy(&(pCell->lfFont), lf, sizeof(LOGFONT));
    return TRUE;
}

LOGFONT* CGridCtrl::GetItemFont(int nRow, int nCol) const
{
    CGridCell* pCell = GetCell(nRow, nCol);
    ASSERT(pCell);
    if (!pCell) return NULL;

    return &pCell->lfFont;
}

////////////////////////////////////////////////////////////////////////////////////
// Row/Column size functions

long CGridCtrl::GetVirtualWidth() const
{
    long lVirtualWidth = 0;
    int iColCount = GetColumnCount();
    for (int i = 0; i < iColCount; i++)
        lVirtualWidth += m_arColWidths[i];

    return lVirtualWidth;
}

long CGridCtrl::GetVirtualHeight() const
{
    long lVirtualHeight = 0;
    int iRowCount = GetRowCount();
    for (int i = 0; i < iRowCount; i++)
        lVirtualHeight += m_arRowHeights[i];

    return lVirtualHeight;
}

int CGridCtrl::GetRowHeight(int nRow) const
{
    ASSERT(nRow >= 0 && nRow < m_nRows);
    if (nRow < 0 || nRow >= m_nRows) return -1;

    return m_arRowHeights[nRow];
}

int CGridCtrl::GetColumnWidth(int nCol) const
{
    ASSERT(nCol >= 0 && nCol < m_nCols);
    if (nCol < 0 || nCol >= m_nCols) return -1;

    return m_arColWidths[nCol];
}

BOOL CGridCtrl::SetRowHeight(int nRow, int height)
{
    ASSERT(nRow >= 0 && nRow < m_nRows && height >= 0);
    if (nRow < 0 || nRow >= m_nRows || height < 0) return FALSE;

    m_arRowHeights[nRow] = height;    
    return TRUE;
}

BOOL CGridCtrl::SetColumnWidth(int nCol, int width)
{
    ASSERT(nCol >= 0 && nCol < m_nCols && width >= 0);
    if (nCol < 0 || nCol >= m_nCols || width < 0) return FALSE;

    m_arColWidths[nCol] = width;
    return TRUE;
}

int CGridCtrl::GetFixedRowHeight() const
{
    int nHeight = 0;
    for (int i = 0; i < m_nFixedRows; i++)
        nHeight += GetRowHeight(i);

    return nHeight;
}

int CGridCtrl::GetFixedColumnWidth() const
{
    int nWidth = 0;
    for (int i = 0; i < m_nFixedCols; i++)
        nWidth += GetColumnWidth(i);

    return nWidth;
}

BOOL CGridCtrl::AutoSizeColumn(int nCol)
{
    ASSERT(nCol >= 0 && nCol < m_nCols);
    if (nCol < 0 || nCol >= m_nCols) return FALSE;

    CSize size;
    CDC* pDC = GetDC();
    if (!pDC) return FALSE;

    int nWidth = 0;
    int nNumRows = GetRowCount();
    for (int nRow = 0; nRow < nNumRows; nRow++)
    {
        size = GetCellExtent(nRow, nCol, pDC);
        if (size.cx > nWidth) nWidth = size.cx;
    }

    m_arColWidths[nCol] = nWidth;

    ReleaseDC(pDC);
    ResetScrollBars();

    return TRUE;
}

BOOL CGridCtrl::AutoSizeRow(int nRow)
{
    ASSERT(nRow >= 0 && nRow < m_nRows);
    if (nRow < 0 || nRow >= m_nRows) return FALSE;

    CSize size;
    CDC* pDC = GetDC();
    if (!pDC) return FALSE;

    int nHeight = 0;
    int nNumColumns = GetColumnCount();
    for (int nCol = 0; nCol < nNumColumns; nCol++)
    {  
        size = GetCellExtent(nRow, nCol, pDC);
        if (size.cy > nHeight) nHeight = size.cy;
    }  
    m_arRowHeights[nRow] = nHeight;

    ReleaseDC(pDC);
    ResetScrollBars();

    return TRUE;
}

void CGridCtrl::AutoSizeColumns()
{
    int nNumColumns = GetColumnCount();
    for (int nCol = 0; nCol < nNumColumns; nCol++)
        AutoSizeColumn(nCol);
}

void CGridCtrl::AutoSizeRows()
{
    int nNumRows = GetRowCount();
    for (int nRow = 0; nRow < nNumRows; nRow++)
        AutoSizeRow(nRow);
}

// sizes all rows and columns
// faster than calling both AutoSizeColumns() and AutoSizeRows()
void CGridCtrl::AutoSize()
{
    CDC* pDC = GetDC();
    if (!pDC) return;

    int nNumColumns = GetColumnCount();
    int nNumRows = GetRowCount();
    
    // initialize column widths to zero
	int nCol = 0;
    for ( ;nCol < nNumColumns; nCol++)
        m_arColWidths[nCol] = 0;
    
    // initialize row heights to zero
	int nRow = 0;
    for ( ;nRow < nNumRows; nRow++)
        m_arRowHeights[nRow] = 0;
    
    CSize size;
    for (nCol = 0; nCol < nNumColumns; nCol++)
        for (nRow = 0; nRow < nNumRows; nRow++)
        {
            size = GetCellExtent(nRow, nCol, pDC);
            if (size.cx > (int) m_arColWidths[nCol])  m_arColWidths[nCol] = size.cx;
            if (size.cy > (int) m_arRowHeights[nRow]) m_arRowHeights[nRow] = size.cy;
        }
    
    ReleaseDC(pDC);

	if(m_bAllowDraw) {
		ResetScrollBars();
		Invalidate();
	}
}

void CGridCtrl::ExpandColumnsToFit()
{
    if (GetColumnCount() <= 0) return;

    CRect rect;
    GetClientRect(rect);

    long virtualWidth = GetVirtualWidth();
    int nDifference = rect.Width() - (int) virtualWidth;
    int nColumnAdjustment = nDifference / GetColumnCount();
int i ;
    for (i = 0; i < GetColumnCount(); i++)
       m_arColWidths[i] += nColumnAdjustment;

    if (nDifference > 0)
    {
        int leftOver = nDifference % GetColumnCount();
        for (i = 0; i < leftOver; i++)
            m_arColWidths[i] += 1;
    } 
    else 
    {
        int leftOver = (-nDifference) % GetColumnCount();
        for (i = 0; i < leftOver; i++)
            m_arColWidths[i] -= 1;
    }

	if(m_bAllowDraw)	Invalidate();
}

void CGridCtrl::ExpandRowsToFit()
{
    if (GetRowCount() <= 0) return;

    CRect rect;
    GetClientRect(rect);

    long virtualHeight = GetVirtualHeight();
    int nDifference = rect.Height() - (int) virtualHeight;
    int nRowAdjustment = nDifference / GetRowCount();
	int i;
    for (i = 0; i < GetRowCount(); i++)
       m_arRowHeights[i] += nRowAdjustment;

    if (nDifference > 0)
    {
        int leftOver = nDifference % GetRowCount();
        for (i = 0; i < leftOver; i++)
            m_arRowHeights[i] += 1;
    } 
    else 
    {
        int leftOver = (-nDifference) % GetRowCount();
        for (i = 0; i < leftOver; i++)
            m_arRowHeights[i] -= 1;
    }

	if(m_bAllowDraw)	Invalidate();
}

void CGridCtrl::ExpandToFit()
{
    ExpandColumnsToFit();   // This will remove any existing horz scrollbar
    ExpandRowsToFit();      // This will remove any existing vert scrollbar
    ExpandColumnsToFit();   // Just in case the first adjustment was with a vert
                            // scrollbar in place
}

/////////////////////////////////////////////////////////////////////////////////////
// GridCtrl cell visibility tests and invalidation/redraw functions

// EnsureVisible supplied by Roelf Werkman
void CGridCtrl::EnsureVisible(int nRow, int nCol)
{
    CCellRange VisibleCells = GetVisibleNonFixedCellRange();

    int right = nCol - VisibleCells.GetMaxCol();
    int left  = VisibleCells.GetMinCol() - nCol;
    int down  = nRow - VisibleCells.GetMaxRow();
    int up    = VisibleCells.GetMinRow() - nRow;

    while (right > 0)
    {
        SendMessage(WM_HSCROLL, SB_LINERIGHT, 0);
        right--;
    }
    while (left > 0)
    {
        SendMessage(WM_HSCROLL, SB_LINELEFT, 0);
        left--;
    }
    while (down > 0)
    {
        SendMessage(WM_VSCROLL, SB_LINEDOWN, 0);
        down--;
   }
   while (up > 0)
   {
       SendMessage(WM_VSCROLL, SB_LINEUP, 0);
       up--;
   }

   // Move one more if we only see a snall bit of the cell
   CRect rectCell, rectWindow;
   GetCellRect(nRow, nCol, rectCell);
   GetClientRect(rectWindow);
   if (rectCell.right > rectWindow.right)
        SendMessage(WM_HSCROLL, SB_LINERIGHT, 0);
   if (rectCell.bottom > rectWindow.bottom)
        SendMessage(WM_VSCROLL, SB_LINEDOWN, 0);
}

BOOL CGridCtrl::IsCellEditable(CCellID &cell) const
{
    return IsCellEditable(cell.row, cell.col); 
}

BOOL CGridCtrl::IsCellEditable(int nRow, int nCol) const
{
    return ( IsEditable() && ((GetItemState(nRow, nCol) & GVIS_READONLY) != GVIS_READONLY) );
}

BOOL CGridCtrl::IsCellVisible(CCellID cell) const
{  
    return IsCellVisible(cell.row, cell.col);
}

BOOL CGridCtrl::IsCellVisible(int nRow, int nCol) const
{  
    if (!IsWindow(m_hWnd))
        return FALSE;

    int x,y;

    CCellID TopLeft;
    if (nCol >= GetFixedColumnCount() || nRow >= GetFixedRowCount())
    {
        TopLeft = GetTopleftNonFixedCell();
        if (nCol >= GetFixedColumnCount() && nCol < TopLeft.col) return FALSE;
        if (nRow >= GetFixedRowCount() && nRow < TopLeft.row) return FALSE;
    }

    CRect rect;
    GetClientRect(rect);
    if (nCol < GetFixedColumnCount())
    {
        x = 0;
        for (int i = 0; i <= nCol; i++) 
        {
            if (x >= rect.right) return FALSE;
            x += GetColumnWidth(i);    
        }
    } 
    else 
    {
        x = GetFixedColumnWidth();
        for (int i = TopLeft.col; i <= nCol; i++) 
        {
            if (x >= rect.right) return FALSE;
            x += GetColumnWidth(i);    
        }
    }

    if (nRow < GetFixedRowCount())
    {
        y = 0;
        for (int i = 0; i <= nRow; i++) 
        {
            if (y >= rect.bottom) return FALSE;
            y += GetRowHeight(i);    
        }
    } 
    else 
    {
        if (nRow < TopLeft.row) return FALSE;
        y = GetFixedRowHeight();
        for (int i = TopLeft.row; i <= nRow; i++) 
        {
            if (y >= rect.bottom) return FALSE;
            y += GetRowHeight(i);    
        }
    }

    return TRUE;
}

BOOL CGridCtrl::InvalidateCellRect(const CCellID& cell)
{
    if (!::IsWindow(GetSafeHwnd()) || !m_bAllowDraw)
        return FALSE;

    ASSERT(IsValid(cell));

    if (!IsCellVisible(cell.row, cell.col)) return FALSE;

    CRect rect;
    if (!GetCellRect(cell, rect)) return FALSE;
    rect.right++; rect.bottom++;
    InvalidateRect(rect, TRUE);

    return TRUE;
}

BOOL CGridCtrl::InvalidateCellRect(const CCellRange& cellRange)
{
    ASSERT(IsValid(cellRange));
    if (!::IsWindow(GetSafeHwnd()) || !m_bAllowDraw) return FALSE;

    CCellRange visibleCellRange = GetVisibleNonFixedCellRange().Intersect(cellRange);

    CRect rect;
    if (!GetCellRangeRect(visibleCellRange, rect)) return FALSE;

    rect.right++; rect.bottom++;
    InvalidateRect(rect, TRUE);

    return TRUE;
}

/////////////////////////////////////////////////////////////////////////////
// CGridCtrl Mouse stuff

// Handles mouse wheel notifications
// Note - if this doesn't work for win95 then use OnRegisteredMouseWheel instead
#if _MFC_VER >= 0x0421
BOOL CGridCtrl::OnMouseWheel(UINT nFlags, short zDelta, CPoint pt) 
{
    // A m_nRowsPerWheelNotch value less than 0 indicates that the mouse
    // wheel scrolls whole pages, not just lines.
    if (m_nRowsPerWheelNotch == -1)
    {
        int nPagesScrolled = zDelta / 120;

        if (nPagesScrolled > 0)
            for (int i = 0; i < nPagesScrolled; i++)
                PostMessage(WM_VSCROLL, SB_PAGEUP, 0);
        else
            for (int i = 0; i > nPagesScrolled; i--)
                PostMessage(WM_VSCROLL, SB_PAGEDOWN, 0);
    }
    else
    {
        int nRowsScrolled = m_nRowsPerWheelNotch * zDelta / 120;

        if (nRowsScrolled > 0)
            for (int i = 0; i < nRowsScrolled; i++)
                PostMessage(WM_VSCROLL, SB_LINEUP, 0);
        else
            for (int i = 0; i > nRowsScrolled; i--)
                PostMessage(WM_VSCROLL, SB_LINEDOWN, 0);
    }

    return CWnd::OnMouseWheel(nFlags, zDelta, pt);
}
#endif

void CGridCtrl::OnMouseMove(UINT nFlags, CPoint point)
{
    CRect rect;
    GetClientRect(rect);

    // If outside client area, return
    if (!rect.PtInRect(point))	return;

    // If the left mouse button is up, then test to see if row/column sizing is imminent
    if (!(nFlags & MK_LBUTTON))
    {
        if (point.y < GetFixedRowHeight() && m_bAllowColResize)
        {
            CCellID idCurrentCell = GetCellFromPt(point);
            CPoint start;
            if (!GetCellOrigin(idCurrentCell, &start)) return;

            int endx = start.x + GetColumnWidth(idCurrentCell.col);

            if ((point.x - start.x <= m_nResizeCaptureRange && idCurrentCell.col != 0) || 
                endx - point.x <= m_nResizeCaptureRange)
            {
                if (m_MouseMode != MOUSE_OVER_COL_DIVIDE)
                    SetCursor(::LoadCursor(NULL, IDC_SIZEWE));
                m_MouseMode = MOUSE_OVER_COL_DIVIDE;
            }
            else 
            {
                if (m_MouseMode != MOUSE_NOTHING)
                    SetCursor(::LoadCursor(NULL, IDC_ARROW));
                m_MouseMode = MOUSE_NOTHING;
            }
        }
        else if (point.x < GetFixedColumnWidth() && m_bAllowRowResize)
        {
            CCellID idCurrentCell = GetCellFromPt(point);
            CPoint start;
            if (!GetCellOrigin(idCurrentCell, &start)) return;

            int endy = start.y + GetRowHeight(idCurrentCell.row);

            if ((point.y - start.y <= m_nResizeCaptureRange && idCurrentCell.row != 0) || 
                endy - point.y <= m_nResizeCaptureRange)
            {
                if (m_MouseMode != MOUSE_OVER_ROW_DIVIDE)
                    SetCursor(::LoadCursor(NULL, IDC_SIZENS));
                m_MouseMode = MOUSE_OVER_ROW_DIVIDE;
            }
            else
            {
                if (m_MouseMode != MOUSE_NOTHING)
                    SetCursor(::LoadCursor(NULL, IDC_ARROW));
                m_MouseMode = MOUSE_NOTHING;
            }
        }
        else
        {
            if (m_MouseMode != MOUSE_NOTHING)
                SetCursor(::LoadCursor(NULL, IDC_ARROW));
            m_MouseMode = MOUSE_NOTHING;
        }

        m_LastMousePoint = point;
        return;
    }

    if (!IsValid(m_LeftClickDownCell))
    {
        m_LastMousePoint = point;
        return;
    }

    // If the left mouse button is down, the process appropriately
    if (nFlags & MK_LBUTTON) 
    {
        switch(m_MouseMode)
        {
            case MOUSE_SELECT_ALL:        break;

            case MOUSE_SELECT_COL:
            case MOUSE_SELECT_ROW:    
            case MOUSE_SELECT_CELLS:    {
                                            CCellID idCurrentCell = GetCellFromPt(point);
                                            if (!IsValid(idCurrentCell)) return;
                                            OnSelecting(idCurrentCell);
                                            //SetFocusCell(max(idCurrentCell.row, m_nFixedRows),
                                            //             max(idCurrentCell.col, m_nFixedCols));
                                            if (idCurrentCell.row >= m_nFixedRows &&
                                                idCurrentCell.col >= m_nFixedCols)
                                                    SetFocusCell(idCurrentCell);
                                            break;
                                        }

            case MOUSE_SIZING_COL:        {
                                            CDC* pDC = GetDC();
                                            if (!pDC) break;

                                            CRect oldInvertedRect(m_LastMousePoint.x, rect.top, 
                                                                  m_LastMousePoint.x + 2, rect.bottom);
                                            pDC->InvertRect(&oldInvertedRect);
                                            CRect newInvertedRect(point.x, rect.top, 
                                                                  point.x + 2, rect.bottom);
                                            pDC->InvertRect(&newInvertedRect);
                                            ReleaseDC(pDC);
                                        }
                                        break;

            case MOUSE_SIZING_ROW:        {
                                            CDC* pDC = GetDC();
                                            if (!pDC) break;

                                            CRect oldInvertedRect(rect.left, m_LastMousePoint.y, 
                                                                  rect.right, m_LastMousePoint.y + 2);
                                            pDC->InvertRect(&oldInvertedRect);
                                            CRect newInvertedRect(rect.left, point.y, 
                                                                  rect.right, point.y + 2);
                                            pDC->InvertRect(&newInvertedRect);
                                            ReleaseDC(pDC);
                                        }
                                        break;
        }    
    }

    m_LastMousePoint = point;
}

void CGridCtrl::OnLButtonDown(UINT nFlags, CPoint point)
{
    m_LeftClickDownPoint = point;
    m_LeftClickDownCell = GetCellFromPt(point);
    if (!IsValid(m_LeftClickDownCell)) return;

    m_SelectionStartCell = (nFlags & MK_SHIFT)? m_idCurrentCell : m_LeftClickDownCell;

    SetFocus();        // Auto-destroy any InPlaceEdit's

    // If the user clicks on the current cell, then prepare to edit it.
	if (m_LeftClickDownCell == m_idCurrentCell)
		return;
	else
	{
		SetFocusCell(-1,-1);
		SetFocusCell(max(m_LeftClickDownCell.row, m_nFixedRows),
					max(m_LeftClickDownCell.col, m_nFixedCols));
    }
    // If the user clicks on a selected cell, then prepare to drag it.
    // (If the user moves the mouse, then dragging occurs)
	SetCapture();

    if (m_MouseMode == MOUSE_OVER_COL_DIVIDE) // sizing column
    {
        m_MouseMode = MOUSE_SIZING_COL;
        CPoint start;
        if (!GetCellOrigin(0, m_LeftClickDownCell.col, &start)) return;

        CRect rect;
        GetClientRect(rect);
        CRect invertedRect(point.x, rect.top, point.x + 2, rect.bottom);

        CDC* pDC = GetDC();
        if (pDC) {
            pDC->InvertRect(&invertedRect);
            ReleaseDC(pDC);
        }

        if (point.x - start.x <= m_nResizeCaptureRange)        // clicked right of border
            if (!GetCellOrigin(0, --m_LeftClickDownCell.col, &start)) return;

        rect.left = start.x;
        ClientToScreen(rect);
        ClipCursor(rect);
    }
    else if (m_MouseMode == MOUSE_OVER_ROW_DIVIDE) // sizing row
    {
        m_MouseMode = MOUSE_SIZING_ROW;
        CPoint start;
        if (!GetCellOrigin(m_LeftClickDownCell, &start)) return;

        CRect rect;
        GetClientRect(rect);
        CRect invertedRect(rect.left, point.y, rect.right, point.y + 2);

        CDC* pDC = GetDC();
        if (pDC) {
            pDC->InvertRect(&invertedRect);
            ReleaseDC(pDC);
        }

        if (point.y - start.y <= m_nResizeCaptureRange)            // clicked below border
            if (!GetCellOrigin(--m_LeftClickDownCell.row, 0, &start)) return;

        rect.top = start.y;
        ClientToScreen(rect);
        ClipCursor(rect);
    }
    else // not sizing or editing -- selecting
    {    
        // If Ctrl pressed, save the current cell selection. This will get added
        // to the new cell selection at the end of the cell selection process
        m_PrevSelectedCellMap.RemoveAll();
        if (nFlags & MK_CONTROL) {
            for (POSITION pos = m_SelectedCellMap.GetStartPosition(); pos != NULL; )
            {
                DWORD key;
                CCellID cell;
                m_SelectedCellMap.GetNextAssoc(pos, key, (CCellID&)cell);
                m_PrevSelectedCellMap.SetAt(key, cell);
            }
        }
        
        if (m_LeftClickDownCell.row < GetFixedRowCount())
            OnFixedRowClick(m_LeftClickDownCell);
        else if (m_LeftClickDownCell.col < GetFixedColumnCount())
            OnFixedColumnClick(m_LeftClickDownCell);
        else
        {
            m_MouseMode = MOUSE_SELECT_CELLS;
            OnSelecting(m_LeftClickDownCell);
        }

    }   
    m_LastMousePoint = point;
}

void CGridCtrl::OnLButtonUp(UINT nFlags, CPoint point)
{
    CWnd::OnLButtonUp(nFlags, point);
    ClipCursor(NULL);

    if (GetCapture()->GetSafeHwnd() == GetSafeHwnd())
    {
        ReleaseCapture();
    }

	if((m_MouseMode == MOUSE_SELECT_CELLS)||(m_MouseMode == MOUSE_NOTHING))
	{
		OnEditCell(m_idCurrentCell.row,m_idCurrentCell.col,VK_LBUTTON);
	}

    else if (m_MouseMode == MOUSE_SIZING_COL)
    {
        CRect rect;
        GetClientRect(rect);
        CRect invertedRect(m_LastMousePoint.x, rect.top, m_LastMousePoint.x + 2, rect.bottom);

        CDC* pDC = GetDC();
        if (pDC) {
            pDC->InvertRect(&invertedRect);
            ReleaseDC(pDC);
        }

        if (m_LeftClickDownPoint != point) 
        {   
            CPoint start;
            if (!GetCellOrigin(m_LeftClickDownCell, &start)) return;
            SetColumnWidth(m_LeftClickDownCell.col, point.x - start.x);
            ResetScrollBars();
            Invalidate();
        }
    }
    else if (m_MouseMode == MOUSE_SIZING_ROW)
    {
        CRect rect;
        GetClientRect(rect);
        CRect invertedRect(rect.left, m_LastMousePoint.y, rect.right, m_LastMousePoint.y + 2);
    
        CDC* pDC = GetDC();
        if (pDC) {
            pDC->InvertRect(&invertedRect);
            ReleaseDC(pDC);
        }
    
        if (m_LeftClickDownPoint != point) 
        {
            CPoint start;
            if (!GetCellOrigin(m_LeftClickDownCell, &start)) return;

            SetRowHeight(m_LeftClickDownCell.row, point.y - start.y);
            ResetScrollBars();
            Invalidate();
        }
    } 

	m_MouseMode = MOUSE_NOTHING;
	SetCursor(::LoadCursor(NULL, IDC_ARROW));

	if (!IsValid(m_LeftClickDownCell)) return;

	CWnd *pOwner = GetOwner();
	if (pOwner && IsWindow(pOwner->m_hWnd))
		pOwner->PostMessage(WM_COMMAND, MAKELONG(GetDlgCtrlID(), BN_CLICKED), 
			(LPARAM) GetSafeHwnd());
}

/////////////////////////////////////////////////////////////////////////////
// CGridCtrl overrideables

void CGridCtrl::OnFixedRowClick(CCellID& cell)
{
    if (!IsValid(cell)) 
        return;

    if (cell.col < GetFixedColumnCount()) 
    {
        m_MouseMode = MOUSE_SELECT_ALL;
        OnSelecting(cell);
    } 
    else 
    {
        m_MouseMode = MOUSE_SELECT_COL;
        OnSelecting(cell);
    }
}

void CGridCtrl::OnFixedColumnClick(CCellID& cell)
{
    if (!IsValid(cell))
        return;

    if (cell.row < GetFixedRowCount()) 
    {
        m_MouseMode = MOUSE_SELECT_ALL;
        OnSelecting(cell);
    }
    else 
    {
        m_MouseMode = MOUSE_SELECT_ROW;
        OnSelecting(cell);
    }
}

// Gets the extent of the text pointed to by str (no CDC needed)
// By default this uses the selected font (which is a bigger font)
CSize CGridCtrl::GetTextExtent(LPCTSTR str, BOOL bUseSelectedFont /* = TRUE */)
{
    CDC* pDC = GetDC();
    if (!pDC) return CSize(0,0);

    CFont *pOldFont, font;

    if (bUseSelectedFont)
    {
        LOGFONT lf;
        memcpy(&lf, &m_Logfont, sizeof(LOGFONT));
        lf.lfWeight = SELECTED_CELL_FONT_WEIGHT;
            
        font.CreateFontIndirect(&lf);
    
        pOldFont = pDC->SelectObject(&font);
    }
    else
        pOldFont = pDC->SelectObject(&m_Font);

    CSize size = pDC->GetTextExtent(str);
    pDC->SelectObject(pOldFont);
    ReleaseDC(pDC);

    return size + CSize(2*m_nMargin, 2*m_nMargin);
}

CSize CGridCtrl::GetCellExtent(int nRow, int nCol, CDC* pDC)
{
    LOGFONT *pLF = GetItemFont(nRow, nCol);
    
    // use selected font since it's thicker   
    LOGFONT lf;
    memcpy(&lf, pLF, sizeof(LOGFONT));
    
    if (nRow < m_nFixedRows || nCol < m_nFixedCols)
        lf.lfWeight = SELECTED_CELL_FONT_WEIGHT;
            
    CFont font;
    font.CreateFontIndirect(&lf);
    
    CFont* pOldFont = pDC->SelectObject(&font);
    CSize size = pDC->GetTextExtent(GetItemText(nRow, nCol));
    pDC->SelectObject(pOldFont);

    size += CSize(4*m_nMargin, 2*m_nMargin);

    CSize ImageSize(0,0);
    if (m_pImageList) {
        int nImage = GetItemImage(nRow, nCol);
        if (nImage >= 0) {
            IMAGEINFO Info;
            if (m_pImageList->GetImageInfo(nImage, &Info))
                ImageSize = CSize(Info.rcImage.right-Info.rcImage.left+1, 
                                  Info.rcImage.bottom-Info.rcImage.top+1);
        }
    }
    
    return CSize(size.cx + ImageSize.cx, max(size.cy, ImageSize.cy));
}

BOOL CGridCtrl::DrawFixedCell(CDC* pDC, int nRow, int nCol, CRect rect, BOOL bEraseBk)
{
	if(!m_bAllowDraw) return FALSE;
    GV_ITEM Item;
    Item.mask = GVIF_TEXT | GVIF_FORMAT | GVIF_IMAGE | GVIF_BKCLR | GVIF_FGCLR;
    Item.row = nRow;
    Item.col = nCol;
    if (!GetItem(&Item)) return FALSE;

    // Force redraw of background if custom colour
    if (Item.crBkClr != CLR_DEFAULT)
        bEraseBk = TRUE;

    if (!pDC->IsPrinting() && bEraseBk)
    {
        CBrush brush((Item.crBkClr == CLR_DEFAULT)? m_crFixedBkColour : Item.crBkClr);
        pDC->FillRect(rect, &brush);
    }
    pDC->SetTextColor((Item.crFgClr == CLR_DEFAULT)? m_crFixedTextColour : Item.crFgClr);
    
    int nSavedDC = pDC->SaveDC();
    
    // Create the appropriate font and select into DC
    LOGFONT lf, *pLF = GetItemFont(nRow, nCol);
    if (pLF)
        memcpy(&lf, pLF, sizeof(LOGFONT));
    else
        memcpy(&lf, &m_Logfont, sizeof(LOGFONT));
        
    CCellID FocusCell = GetFocusCell();
//    if (FocusCell.row == nRow || FocusCell.col == nCol)
//        lf.lfWeight = SELECTED_CELL_FONT_WEIGHT;
    
    CFont Font;
    Font.CreateFontIndirect(&lf);
    pDC->SelectObject(&Font);

    if (IsValid(FocusCell) &&  (FocusCell.row == nRow || FocusCell.col == nCol) && (!pDC->IsPrinting()) )
    {
        rect.right++; rect.bottom++;
        pDC->DrawEdge(rect, EDGE_RAISED, BF_RECT);
        rect.DeflateRect(1,1);
    }
    else
    {
        CPen lightpen(PS_SOLID, 1,  ::GetSysColor(COLOR_3DHIGHLIGHT)),
              darkpen(PS_SOLID,  1, ::GetSysColor(COLOR_3DDKSHADOW)),
             *pOldPen = pDC->GetCurrentPen();
    
        pDC->SelectObject(&lightpen);
        pDC->MoveTo(rect.right, rect.top);
        pDC->LineTo(rect.left, rect.top);
        pDC->LineTo(rect.left, rect.bottom);

        pDC->SelectObject(&darkpen);
        pDC->MoveTo(rect.right, rect.top);
        pDC->LineTo(rect.right, rect.bottom);
        pDC->LineTo(rect.left, rect.bottom);

        pDC->SelectObject(pOldPen);
        rect.DeflateRect(1,1);
    }

    pDC->SetBkMode(TRANSPARENT);
    rect.DeflateRect(m_nMargin, 0);

    if (m_pImageList && Item.iImage >= 0) {
        IMAGEINFO Info;
        if (m_pImageList->GetImageInfo(Item.iImage, &Info)) {
            int nImageWidth = Info.rcImage.right-Info.rcImage.left+1;
				m_pImageList->Draw(pDC, Item.iImage, rect.TopLeft(), ILD_NORMAL);
            rect.left += nImageWidth+m_nMargin;
        }
    }

    DrawText(pDC->m_hDC, Item.szText, -1, rect, Item.nFormat);

    pDC->RestoreDC(nSavedDC);
    return TRUE;
}

BOOL CGridCtrl::DrawCell(CDC* pDC, int nRow, int nCol, CRect rect, BOOL bEraseBk)
{
	if(!m_bAllowDraw)	return FALSE;
    GV_ITEM Item;
    Item.mask = GVIF_TEXT | GVIF_FORMAT | GVIF_STATE | GVIF_IMAGE | GVIF_BKCLR | GVIF_FGCLR;
    Item.row = nRow;
    Item.col = nCol;
    if (!GetItem(&Item)) return FALSE;

    COLORREF TextBkClr = (Item.crBkClr == CLR_DEFAULT)? m_crTextBkColour : Item.crBkClr;
    COLORREF TextClr   = (Item.crFgClr == CLR_DEFAULT)? m_crTextColour   : Item.crFgClr;

    // Force redraw of background if custom colour
    if (TextBkClr != CLR_DEFAULT)
        bEraseBk = TRUE;

    int nSavedDC = pDC->SaveDC();

    pDC->SetBkMode(TRANSPARENT);

    if (Item.state & GVIS_FOCUSED && !pDC->IsPrinting()) 
    {
        rect.right++; rect.bottom++;    // FillRect doesn't draw RHS or bottom
        if (bEraseBk) 
        {
            CBrush brush(TextBkClr);
            pDC->FillRect(rect, &brush);
        }
        rect.right--; rect.bottom--;    
        pDC->SelectStockObject(BLACK_PEN);
        pDC->SelectStockObject(NULL_BRUSH);
        pDC->Rectangle(rect);
        pDC->SetTextColor(TextClr);

        rect.DeflateRect(1,1);

    }
    else if (Item.state & GVIS_SELECTED && !pDC->IsPrinting()) 
    {
        rect.right++; rect.bottom++;    // FillRect doesn't draw RHS or bottom
        pDC->FillSolidRect(rect, ::GetSysColor(COLOR_HIGHLIGHT));
        rect.right--; rect.bottom--;
        pDC->SetTextColor(::GetSysColor(COLOR_HIGHLIGHTTEXT));
    } else {
        rect.right++; rect.bottom++;    // FillRect doesn't draw RHS or bottom
        if (bEraseBk) 
        {
            CBrush brush(TextBkClr);
            pDC->FillRect(rect, &brush);
        }
        rect.right--; rect.bottom--;
        pDC->SetTextColor(TextClr);
    }

    if (Item.state & GVIS_DROPHILITED && !pDC->IsPrinting()) {
        pDC->SelectStockObject(BLACK_PEN);
        pDC->SelectStockObject(NULL_BRUSH);
        pDC->Rectangle(rect);
    }

    // Create the appropriate font and select into DC
    CFont Font;
    LOGFONT *pLF = GetItemFont(nRow, nCol);
    if (pLF)
        Font.CreateFontIndirect(pLF);
    else
        Font.CreateFontIndirect(&m_Logfont);
        
    CFont* pOldFont = pDC->SelectObject(&Font);
    
    pDC->SelectObject(&Font);
    rect.DeflateRect(m_nMargin, 0);

    if (m_pImageList && Item.iImage >= 0) {
        IMAGEINFO Info;
        if (m_pImageList->GetImageInfo(Item.iImage, &Info)) {
            int nImageWidth = Info.rcImage.right-Info.rcImage.left+1;
 			m_pImageList->Draw(pDC, Item.iImage, rect.TopLeft(), ILD_NORMAL);
            rect.left += nImageWidth+m_nMargin;
        }
    }

    DrawText(pDC->m_hDC, Item.szText, -1, rect, Item.nFormat);

    pDC->RestoreDC(nSavedDC);
    return TRUE;
}

void CGridCtrl::OnEditCell(int nRow, int nCol, UINT nChar)
{
    EnsureVisible(nRow, nCol);

    CCellID cell(nRow, nCol);
    if (!IsValid(cell) || !IsCellEditable(nRow, nCol) || !IsCellVisible(nRow, nCol)) 
        return;
    CRect rect;
    if (!GetCellRect(cell, rect)) return;

    SendMessageToParent(nRow, nCol, GVN_BEGINLABELEDIT);
}

CString CGridCtrl::GetItemText(int nRow, int nCol)
{
    if (nRow < 0 || nRow >= m_nRows || nCol < 0 || nCol >= m_nCols)
		return _T("");

    CGridCell* pCell = GetCell(nRow, nCol);
    ASSERT(pCell);
    if (!pCell) return _T("");

    return pCell->szText;
}

void CGridCtrl::SetGridResize(BOOL bResizeRow, BOOL bResizeCol)
{
	m_bAllowRowResize	= bResizeRow;
	m_bAllowColResize	= bResizeCol;
}

void CGridCtrl::PrintColumnHeadings(CDC *pDC, CPrintInfo *pInfo)
{
	CRect rect;
	rect.bottom = -1;
	for(int row = 0; row < GetFixedRowCount(); row++)
	{
		rect.top	= rect.bottom + 1;
		rect.bottom	= rect.top + GetRowHeight(row) - 1;

		rect.right	= -1;
		for(int col = 0; col < GetColumnCount(); col++)
		{
			rect.left	= rect.right + 1;
			rect.right	= rect.left + GetColumnWidth(col) - 1;

			DrawFixedCell(pDC,row,col,rect);

			if(m_nGridLines == GVL_BOTH || m_nGridLines == GVL_HORZ)
			{
				int Overlap = (col ==0 )? 0:1;
				pDC->MoveTo(rect.left-Overlap,rect.bottom);
				pDC->LineTo(rect.right,rect.bottom);
				if(row == 0)
				{
					pDC->MoveTo(rect.left-Overlap,rect.top);
					pDC->LineTo(rect.right,rect.top);
				}
			}

			if(m_nGridLines == GVL_BOTH || m_nGridLines == GVL_VERT)
			{
				int Overlap = (row == 0)? 0:1;
				pDC->MoveTo(rect.right,rect.top-Overlap);
				pDC->LineTo(rect.right,rect.bottom);
				if(col == 0)
				{
					pDC->MoveTo(rect.left,rect.top-Overlap);
					pDC->LineTo(rect.left,rect.bottom);
				}
			}
		}
	}
}

void CGridCtrl::PrintRow(CDC* pDC,CPrintInfo* pInfo,int nRow)
{
	CRect rect;
	rect.bottom	= -1;
	rect.top	= rect.bottom+1;
	rect.bottom	= rect.top + GetRowHeight(nRow)-1;

	rect.right	= -1;
	for(int col = 0; col < GetColumnCount(); col++)
	{
		rect.left	= rect.right + 1;
		rect.right	= rect.left + GetColumnWidth(col) - 1;

		DrawCell(pDC,nRow,col,rect);

		if(m_nGridLines == GVL_BOTH || m_nGridLines == GVL_HORZ)
		{
			int Overlap = (col == 0)? 0:1;
			pDC->MoveTo(rect.left-Overlap,rect.bottom);
			pDC->LineTo(rect.right,rect.bottom);
			if(nRow == 0)
			{
				pDC->MoveTo(rect.left-Overlap,rect.top);
				pDC->LineTo(rect.right,rect.top);
			}
		}

		if(m_nGridLines == GVL_BOTH || m_nGridLines == GVL_VERT)
		{
			int Overlap = (nRow ==1)? 0:1;
			pDC->MoveTo(rect.right,rect.top-Overlap);
			pDC->LineTo(rect.right,rect.bottom);
			if( col==0 )
			{
				pDC->MoveTo(rect.left,rect.top-Overlap);
				pDC->LineTo(rect.left,rect.bottom);
			}
		}
	}
}
BOOL CGridCtrl::PreTranslateMessage(MSG* pMsg) 
{
	// TODO: Add your specialized code here and/or call the base class
	if( pMsg->message == WM_KEYDOWN)
	{
		if(pMsg->wParam == VK_RETURN)
			OnEditCell(m_idCurrentCell.row,m_idCurrentCell.col,VK_LBUTTON);
	}
	return CWnd::PreTranslateMessage(pMsg);
}

void CGridCtrl::OnRButtonUp(UINT nFlags, CPoint point) 
{
	// TODO: Add your message handler code here and/or call default
	if(GetSelectedCellRange().IsValid())
	{
 	CMenu* menu_bar = AfxGetMainWnd()->GetMenu();
	CMenu* edit_menu = menu_bar->GetSubMenu(1);    
	ASSERT(edit_menu);

	ClientToScreen(&point);
	edit_menu->TrackPopupMenu(TPM_LEFTALIGN |TPM_RIGHTBUTTON, point.x,
		point.y,AfxGetMainWnd());
	}
	CWnd::OnRButtonUp(nFlags, point);
}
