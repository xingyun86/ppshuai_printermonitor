// PrinterMonitor.cpp : Defines the entry point for the application.
//

#include "framework.h"
#include "PrinterMonitor.h"

#define TIMER_SPACE_1S 1000
INT_PTR CALLBACK PrinterMonitorBox(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam);

INT APIENTRY _tWinMain(_In_ HINSTANCE hInstance,
                     _In_opt_ HINSTANCE hPrevInstance,
                     _In_ LPTSTR    lpCmdLine,
                     _In_ INT       nCmdShow)
{
    UNREFERENCED_PARAMETER(hPrevInstance);
    UNREFERENCED_PARAMETER(lpCmdLine);

    // TODO: Place code here.
   
    /*HANDLE hPrinter = NULL;
    DWORD cByteUsed = 0L;
    DWORD cByteNeeded = 0L;
    if (!GetPrinter(hPrinter, 2, NULL, 0, &cByteNeeded))
    {
        DWORD dwErrorCode = ::GetLastError();
        if (dwErrorCode != ERROR_INSUFFICIENT_BUFFER)
            return FALSE;
    }

    PRINTER_INFO_2 * pPrinterInfo = (PRINTER_INFO_2*)malloc(cByteNeeded);
    if (!(pPrinterInfo))
    {
        return FALSE;
    }

    if (!GetPrinter(hPrinter,
        2,
        (LPBYTE)pPrinterInfo,
        cByteNeeded,
        &cByteUsed))
    {
        free(pPrinterInfo);
        pPrinterInfo = NULL;
        return FALSE;
    }*/
    // Initialize global strings
    return DialogBox(hInstance, MAKEINTRESOURCE(IDD_PRINTERMONITORBOX), NULL, PrinterMonitorBox);
}
LPTSTR GetPrinterAliasName(LPPRINTER_INFO_2 pPrinterInfo2)
{
    for (size_t i = 0; *(pPrinterInfo2->pPrinterName + i) != '\0'; i++)
    {
        switch (*(pPrinterInfo2->pPrinterName + i))
        {
        case _T(' '):*(pPrinterInfo2->pPrinterName + i) = _T('#'); break;
        case _T('\r'):*(pPrinterInfo2->pPrinterName + i) = _T('&'); break;
        case _T('\n'):*(pPrinterInfo2->pPrinterName + i) = _T('@'); break;
        default:break;
        }
    }
    return pPrinterInfo2->pPrinterName;
}
LPTSTR GetPrinterRawName(LPPRINTER_INFO_2 pPrinterInfo2)
{
    for (size_t i = 0; *(pPrinterInfo2->pPrinterName + i) != '\0'; i++)
    {
        switch (*(pPrinterInfo2->pPrinterName + i))
        {
        case _T('#'):*(pPrinterInfo2->pPrinterName + i) = _T(' '); break;
        case _T('&'):*(pPrinterInfo2->pPrinterName + i) = _T('\r'); break;
        case _T('@'):*(pPrinterInfo2->pPrinterName + i) = _T('\n'); break;
        default:break;
        }
    }
    return pPrinterInfo2->pPrinterName;
}
LPCTSTR StatusTotext(CString& text, DWORD status)
{
    text = _T("");
    if ((status & PRINTER_STATUS_BUSY) == PRINTER_STATUS_BUSY)
    {
        text.Append(_T("打印机正忙。"));
    }
    if ((status & PRINTER_STATUS_DOOR_OPEN) == PRINTER_STATUS_DOOR_OPEN)
    {
        text.Append(_T("打印机门打开。"));
    }
    if ((status & PRINTER_STATUS_ERROR) == PRINTER_STATUS_ERROR)
    {
        text.Append(_T("打印机处于错误状态。"));
    }
    if ((status & PRINTER_STATUS_INITIALIZING) == PRINTER_STATUS_INITIALIZING)
    {
        text.Append(_T("打印机正在初始化。"));
    }
    if ((status & PRINTER_STATUS_IO_ACTIVE) == PRINTER_STATUS_IO_ACTIVE)
    {
        text.Append(_T("打印机处于活动输入/输出状态。"));
    }
    if ((status & PRINTER_STATUS_MANUAL_FEED) == PRINTER_STATUS_MANUAL_FEED)
    {
        text.Append(_T("打印机处于手动进纸状态。"));
    }
    if ((status & PRINTER_STATUS_NO_TONER) == PRINTER_STATUS_NO_TONER)
    {
        text.Append(_T("打印机墨粉不足。"));
    }
    if ((status & PRINTER_STATUS_NOT_AVAILABLE) == PRINTER_STATUS_NOT_AVAILABLE)
    {
        text.Append(_T("打印机不可用于打印。"));
    }
    if ((status & PRINTER_STATUS_OFFLINE) == PRINTER_STATUS_OFFLINE)
    {
        text.Append(_T("打印机离线。"));
    }
    if ((status & PRINTER_STATUS_OUT_OF_MEMORY) == PRINTER_STATUS_OUT_OF_MEMORY)
    {
        text.Append(_T("打印机内存不足。"));
    }
    if ((status & PRINTER_STATUS_OUTPUT_BIN_FULL) == PRINTER_STATUS_OUTPUT_BIN_FULL)
    {
        text.Append(_T("打印机的出纸槽已满。"));
    }
    if ((status & PRINTER_STATUS_PAGE_PUNT) == PRINTER_STATUS_PAGE_PUNT)
    {
        text.Append(_T("打印机无法打印当前页面。Windows 95/98/Me：表示页面正在“打印”（即未打印），因为打印机打印过于复杂。"));
    }
    if ((status & PRINTER_STATUS_PAPER_JAM) == PRINTER_STATUS_PAPER_JAM)
    {
        text.Append(_T("纸张卡在打印机中。"));
    }
    if ((status & PRINTER_STATUS_PAPER_OUT) == PRINTER_STATUS_PAPER_OUT)
    {
        text.Append(_T("打印机缺纸。"));
    }
    if ((status & PRINTER_STATUS_PAPER_PROBLEM) == PRINTER_STATUS_PAPER_PROBLEM)
    {
        text.Append(_T("打印机有纸问题。"));
    }
    if ((status & PRINTER_STATUS_PAUSED) == PRINTER_STATUS_PAUSED)
    {
        text.Append(_T("打印机已暂停。"));
    }
    if ((status & PRINTER_STATUS_PENDING_DELETION) == PRINTER_STATUS_PENDING_DELETION)
    {
        text.Append(_T("打印机正在删除。"));
    }
    if ((status & PRINTER_STATUS_POWER_SAVE) == PRINTER_STATUS_POWER_SAVE)
    {
        text.Append(_T("打印机处于省电模式。"));
    }
    if ((status & PRINTER_STATUS_PRINTING) == PRINTER_STATUS_PRINTING)
    {
        text.Append(_T("打印机正在打印。"));
    }
    if ((status & PRINTER_STATUS_PROCESSING) == PRINTER_STATUS_PROCESSING)
    {
        text.Append(_T("打印机正在处理打印作业。"));
    }
    if ((status & PRINTER_STATUS_SERVER_UNKNOWN) == PRINTER_STATUS_SERVER_UNKNOWN)
    {
        text.Append(_T("打印机状态未知。"));
    }
    if ((status & PRINTER_STATUS_TONER_LOW) == PRINTER_STATUS_TONER_LOW)
    {
        text.Append(_T("打印机墨粉不足。"));
    }
    if ((status & PRINTER_STATUS_USER_INTERVENTION) == PRINTER_STATUS_USER_INTERVENTION)
    {
        text.Append(_T("打印机有一个错误，需要用户做某事。"));
    }
    if ((status & PRINTER_STATUS_WAITING) == PRINTER_STATUS_WAITING)
    {
        text.Append(_T("打印机正在等待。"));
    }
    if ((status & PRINTER_STATUS_WARMING_UP) == PRINTER_STATUS_WARMING_UP)
    {
        text.Append(_T("打印机正在升温。"));
    }
    if((status | NORMAL_PRINT) == NORMAL_PRINT)
    {
        text.Append(_T("打印机无状态。"));
    }
    return text;
}
DWORD GetPrinterStatus(LPPRINTER_INFO_2 pPrinterInfo2, DWORD LEVEL_2/* = 2L*/)
{
    HANDLE _hPrinter = NULL;
    DWORD _dwPrinterStatus = 0L;

    // Open a handle to the printer. 
    if (OpenPrinter(GetPrinterRawName(pPrinterInfo2), &_hPrinter, NULL))
    {
        DWORD _cByteUsed = 0L;
        DWORD _nReturned = 0L;
        DWORD _cByteNeeded = 0L;
        JOB_INFO_2* _pJobInfo_2 = NULL;
        LPPRINTER_INFO_2 _pPrinterInfo_2 = NULL;

        if (!GetPrinter(_hPrinter, LEVEL_2, NULL, 0, &_cByteNeeded) &&
            GetLastError() == ERROR_INSUFFICIENT_BUFFER)
        {
            _pPrinterInfo_2 = reinterpret_cast<LPPRINTER_INFO_2>(LocalAlloc(LPTR, _cByteNeeded + sizeof(_cByteNeeded)));
            if (_pPrinterInfo_2 != NULL)
            {
                if (GetPrinter(_hPrinter, LEVEL_2, (LPBYTE)_pPrinterInfo_2, _cByteNeeded, &_cByteUsed))
                {
                    //_dwPrinterStatus = _pPrinterInfo_2->Status;
                    _cByteNeeded = 0L;
                    _nReturned = 0L;
                    if (_pPrinterInfo_2->cJobs > 0 &&
                        !EnumJobs(_hPrinter, 0, _pPrinterInfo_2->cJobs, LEVEL_2, NULL, 0, (LPDWORD)&_cByteNeeded, (LPDWORD)&_nReturned) &&
                        GetLastError() == ERROR_INSUFFICIENT_BUFFER)
                    {
                        _pJobInfo_2 = reinterpret_cast<LPJOB_INFO_2>(LocalAlloc(LPTR, _cByteNeeded + sizeof(_cByteNeeded)));
                        if (_pJobInfo_2 != NULL)
                        {
                            if (EnumJobs(_hPrinter, 0, _pPrinterInfo_2->cJobs, LEVEL_2,
                                (LPBYTE)_pJobInfo_2, _cByteNeeded, (LPDWORD)&_cByteUsed, (LPDWORD)&_nReturned))
                            {
                                for (size_t i = 0; i < _pPrinterInfo_2->cJobs; i++)
                                {
                                    _dwPrinterStatus = _pJobInfo_2[i].Status;
                                    pPrinterInfo2->Status |= _dwPrinterStatus;
                                }
                            }
                            LocalFree(_pJobInfo_2);
                            _pJobInfo_2 = NULL;
                        }
                    }
                }
                LocalFree(_pPrinterInfo_2);
                _pPrinterInfo_2 = NULL;
            }
        }
        ClosePrinter(_hPrinter);
        _hPrinter = NULL;
    }
    return pPrinterInfo2->Status;
}
// Message handler for PrinterMonitor box.
INT_PTR CALLBACK PrinterMonitorBox(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam)
{
    UNREFERENCED_PARAMETER(lParam);
    switch (message)
    {
    case WM_INITDIALOG:
    {
        SendMessage(hDlg, WM_SETICON, ICON_BIG, (LPARAM)LoadIcon(GetModuleHandle(NULL), MAKEINTRESOURCE(IDI_PRINTERMONITOR)));
        SendMessage(hDlg, WM_SETICON, ICON_SMALL, (LPARAM)LoadIcon(GetModuleHandle(NULL), MAKEINTRESOURCE(IDI_PRINTERMONITOR)));
        SendMessage(hDlg, WM_SETICON, ICON_SMALL2, (LPARAM)LoadIcon(GetModuleHandle(NULL), MAKEINTRESOURCE(IDI_PRINTERMONITOR)));
        CenterWindowInScreen(hDlg);
        SetTimer(hDlg, TIMER_SPACE_1S, TIMER_SPACE_1S, NULL);
        return (INT_PTR)TRUE;
    }
    break;
    case WM_TIMER:
    {
        switch (wParam)
        {
        case TIMER_SPACE_1S:
        {
            {
                DWORD cbBuf = 0L;
                DWORD LEVEL_2 = 2L;
                LV_ITEM lvi = { 0 };
                LV_COLUMN lvc = { 0 };
                HWND hListData = NULL;
                DWORD pcReturned = 0L;
                CString text = _T("");
                LVFINDINFO lvfi = { 0 };
                RECT rcListData = { 0 };
                TCHAR Name[MAXBYTE] = { 0 };
                CONST TCHAR* tTextList[] = {
                                _T("打印机名称"),
                                _T("状态"),
                                //_T("服务器名称"),
                                //_T("驱动名称"),
                                //_T("打印机处理器")
                };
                LPPRINTER_INFO_2 pPrinterInfo2 = NULL;
                LPPRINTER_INFO_2 lpPrinterInfo2 = NULL;
                DWORD Flags = PRINTER_ENUM_LOCAL | PRINTER_ENUM_FAVORITE |
                    PRINTER_ENUM_CONNECTIONS | PRINTER_ENUM_NETWORK;

                if (!EnumPrinters(Flags, Name, LEVEL_2, NULL, 0, &cbBuf, &pcReturned) && 
                    GetLastError() == ERROR_INSUFFICIENT_BUFFER)
                {
                    lpPrinterInfo2 = reinterpret_cast<LPPRINTER_INFO_2>(LocalAlloc(LPTR, cbBuf + sizeof(cbBuf)));
                    if (lpPrinterInfo2 != NULL)
                    {
                        if (EnumPrinters(Flags, Name, LEVEL_2, (LPBYTE)lpPrinterInfo2, cbBuf, &cbBuf, &pcReturned))
                        {
                            hListData = GetDlgItem(hDlg, IDC_LIST_DATA);                        
                            if (Header_GetItemCount(ListView_GetHeader(hListData)) == 0)
                            {
                                ListView_DeleteAllItems(hListData);
                                while (ListView_DeleteColumn(hListData, 0));
                                ListView_SetExtendedListViewStyle(hListData,
                                    ListView_GetExtendedListViewStyle(hListData)
                                    | LVS_EX_DOUBLEBUFFER | LVS_EX_FULLROWSELECT);
                                GetClientRect(hListData, &rcListData);
                                for (size_t i = 0; i < sizeof(tTextList) / sizeof(*tTextList); i++)
                                {
                                    lvc.mask = LVCF_WIDTH | LVCF_FMT | LVCF_TEXT;
                                    lvc.pszText = (LPTSTR)tTextList[i];
                                    lvc.fmt = LVCFMT_LEFT;
                                    lvc.cx = (rcListData.right - rcListData.left) / (sizeof(tTextList) / sizeof(*tTextList) + 1);
                                    lvc.iSubItem = i;
                                    ListView_InsertColumn(hListData, Header_GetItemCount(ListView_GetHeader(hListData)), &lvc);
                                }

                                for (DWORD i = 0; i < pcReturned; i++)
                                {
                                    pPrinterInfo2 = &lpPrinterInfo2[i];
                                    lvi.mask = LVIF_TEXT;
                                    lvi.iItem = i;
                                    for (size_t n = 0; n < sizeof(tTextList) / sizeof(*tTextList); n++)
                                    {
                                        lvi.iSubItem = n;
                                        switch (lvi.iSubItem)
                                        {
                                        case 0:lvi.pszText = GetPrinterAliasName(pPrinterInfo2); break;
                                        case 1:lvi.pszText = (LPTSTR)StatusTotext(text, GetPrinterStatus(pPrinterInfo2, LEVEL_2)); break;
                                        case 2:lvi.pszText = pPrinterInfo2->pServerName; break;
                                        case 3:lvi.pszText = pPrinterInfo2->pDriverName; break;
                                        case 4:lvi.pszText = pPrinterInfo2->pPrintProcessor; break;
                                        default:lvi.pszText = (LPTSTR)_T(""); break;
                                        }
                                        if (lvi.iSubItem == 0)
                                        {
                                            ListView_InsertItem(hListData, &lvi);
                                        }
                                        else
                                        {
                                            ListView_SetItem(hListData, &lvi);
                                        }
                                    }
                                }
                            }                           
                            else
                            {
                                for (DWORD i = 0; i < pcReturned; i++)
                                {
                                    pPrinterInfo2 = &lpPrinterInfo2[i];
                                                                        
                                    lvfi.flags = LVFI_PARTIAL | LVFI_STRING;
                                    lvfi.psz = GetPrinterAliasName(pPrinterInfo2);
                                    lvi.mask = LVIF_TEXT;
                                    lvi.iItem = ListView_FindItem(hListData, -1, &lvfi);
                                    if (lvi.iItem != (-1))
                                    {
                                        lvi.iSubItem = 1;
                                        switch (lvi.iSubItem)
                                        {
                                        case 0:lvi.pszText = GetPrinterAliasName(pPrinterInfo2); break;
                                        case 1:lvi.pszText = (LPTSTR)StatusTotext(text, GetPrinterStatus(pPrinterInfo2, LEVEL_2)); break;
                                        case 2:lvi.pszText = pPrinterInfo2->pServerName; break;
                                        case 3:lvi.pszText = pPrinterInfo2->pDriverName; break;
                                        case 4:lvi.pszText = pPrinterInfo2->pPrintProcessor; break;
                                        default:lvi.pszText = (LPTSTR)_T(""); break;
                                        }
                                        if (lvi.iSubItem == 0)
                                        {
                                            ListView_InsertItem(hListData, &lvi);
                                        }
                                        else
                                        {
                                            ListView_SetItem(hListData, &lvi);
                                        }
                                    }
                                    else
                                    {
                                        lvi.iItem = ListView_GetItemCount(hListData);
                                        for (size_t n = 0; n < sizeof(tTextList) / sizeof(*tTextList); n++)
                                        {
                                            lvi.iSubItem = n;
                                            switch (lvi.iSubItem)
                                            {
                                            case 0:lvi.pszText = GetPrinterAliasName(pPrinterInfo2); break;
                                            case 1:lvi.pszText = (LPTSTR)StatusTotext(text, GetPrinterStatus(pPrinterInfo2, LEVEL_2)); break;
                                            case 2:lvi.pszText = pPrinterInfo2->pServerName; break;
                                            case 3:lvi.pszText = pPrinterInfo2->pDriverName; break;
                                            case 4:lvi.pszText = pPrinterInfo2->pPrintProcessor; break;
                                            default:lvi.pszText = (LPTSTR)_T(""); break;
                                            }
                                            if (lvi.iSubItem == 0)
                                            {
                                                ListView_InsertItem(hListData, &lvi);
                                            }
                                            else
                                            {
                                                ListView_SetItem(hListData, &lvi);
                                            }
                                        }
                                    }
                                }
                            }
                            
                        }
                        LocalFree(lpPrinterInfo2);
                        lpPrinterInfo2 = NULL;
                    }
                }
            }
        }
            break;
        default:
            break;
        }
    }
    break;
    case WM_COMMAND:
    {
        switch (LOWORD(wParam))
        {
        case IDOK:
        {
            EndDialog(hDlg, LOWORD(wParam));
            return (INT_PTR)TRUE;
        }
        break;
        case IDCANCEL:
        {
            KillTimer(hDlg, TIMER_SPACE_1S);
            EndDialog(hDlg, LOWORD(wParam));
            return (INT_PTR)TRUE;
        }
        break;
        default:
            break;
        }
    }
        break;
    }
    return (INT_PTR)FALSE;
}
