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
                DWORD Level = 2L;
                LV_ITEM lvi = { 0 };
                LV_COLUMN lvc = { 0 };
                HWND hListData = NULL;
                DWORD pcReturned = 0L;
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

                ::EnumPrinters(Flags, Name, Level, NULL, 0, &cbBuf, &pcReturned);
                if (GetLastError() == ERROR_INSUFFICIENT_BUFFER)
                {
                    lpPrinterInfo2 = reinterpret_cast<LPPRINTER_INFO_2>(LocalAlloc(LPTR, cbBuf + sizeof(cbBuf)));
                    if (lpPrinterInfo2 != NULL)
                    {
                        if (EnumPrinters(Flags, Name, Level, (LPBYTE)lpPrinterInfo2, cbBuf, &cbBuf, &pcReturned))
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
                                    for (size_t i = 0; *(pPrinterInfo2->pPrinterName + i) != '\0'; i++)
                                    {
                                        switch (*(pPrinterInfo2->pPrinterName + i))
                                        {
                                        case _T(' '):*(pPrinterInfo2->pPrinterName + i) = _T('_'); break;
                                        case _T('\r'):*(pPrinterInfo2->pPrinterName + i) = _T('-'); break;
                                        case _T('\n'):*(pPrinterInfo2->pPrinterName + i) = _T('~'); break;
                                        default:break;
                                        }
                                    }
                                    lvi.mask = LVIF_TEXT;
                                    lvi.iItem = i;
                                    for (size_t n = 0; n < sizeof(tTextList) / sizeof(*tTextList); n++)
                                    {
                                        lvi.iSubItem = n;
                                        switch (lvi.iSubItem)
                                        {
                                        case 0:lvi.pszText = pPrinterInfo2->pPrinterName; break;
                                        case 1:
                                            switch (pPrinterInfo2->Status)
                                            {
                                            case PRINTER_STATUS_BUSY:lvi.pszText = (LPTSTR)_T("打印机正忙"); break;
                                            case PRINTER_STATUS_DOOR_OPEN: lvi.pszText = (LPTSTR)_T("打印机门打开"); break;
                                            case PRINTER_STATUS_ERROR:lvi.pszText = (LPTSTR)_T("打印机处于错误状态"); break;
                                            case  PRINTER_STATUS_INITIALIZING:lvi.pszText = (LPTSTR)_T("打印机正在初始化"); break;
                                            case  PRINTER_STATUS_IO_ACTIVE:lvi.pszText = (LPTSTR)_T("打印机处于活动输入/输出状态"); break;
                                            case  PRINTER_STATUS_MANUAL_FEED:lvi.pszText = (LPTSTR)_T("打印机处于手动进纸状态"); break;
                                            case  PRINTER_STATUS_NO_TONER:lvi.pszText = (LPTSTR)_T("打印机墨粉不足"); break;
                                            case  PRINTER_STATUS_NOT_AVAILABLE:lvi.pszText = (LPTSTR)_T("打印机不可用于打印"); break;
                                            case  PRINTER_STATUS_OFFLINE:lvi.pszText = (LPTSTR)_T("打印机离线"); break;
                                            case  PRINTER_STATUS_OUT_OF_MEMORY:lvi.pszText = (LPTSTR)_T("打印机内存不足"); break;
                                            case  PRINTER_STATUS_OUTPUT_BIN_FULL:lvi.pszText = (LPTSTR)_T("打印机的出纸槽已满"); break;
                                            case  PRINTER_STATUS_PAGE_PUNT:lvi.pszText = (LPTSTR)_T("打印机无法打印当前页面。Windows 95/98/Me：表示页面正在“打印”（即未打印），因为打印机打印过于复杂"); break;
                                            case  PRINTER_STATUS_PAPER_JAM:lvi.pszText = (LPTSTR)_T("纸张卡在打印机中"); break;
                                            case  PRINTER_STATUS_PAPER_OUT:lvi.pszText = (LPTSTR)_T("打印机缺纸"); break;
                                            case  PRINTER_STATUS_PAPER_PROBLEM:lvi.pszText = (LPTSTR)_T("打印机有纸问题"); break;
                                            case  PRINTER_STATUS_PAUSED:lvi.pszText = (LPTSTR)_T("打印机已暂停"); break;
                                            case  PRINTER_STATUS_PENDING_DELETION:lvi.pszText = (LPTSTR)_T("打印机正在删除"); break;
                                            case  PRINTER_STATUS_POWER_SAVE:lvi.pszText = (LPTSTR)_T("打印机处于省电模式"); break;
                                            case  PRINTER_STATUS_PRINTING:lvi.pszText = (LPTSTR)_T("打印机正在打印"); break;
                                            case  PRINTER_STATUS_PROCESSING:lvi.pszText = (LPTSTR)_T("打印机正在处理打印作业"); break;
                                            case  PRINTER_STATUS_SERVER_UNKNOWN:lvi.pszText = (LPTSTR)_T("打印机状态未知"); break;
                                            case  PRINTER_STATUS_TONER_LOW:lvi.pszText = (LPTSTR)_T("打印机墨粉不足"); break;
                                            case  PRINTER_STATUS_USER_INTERVENTION:lvi.pszText = (LPTSTR)_T("打印机有一个错误，需要用户做某事"); break;
                                            case  PRINTER_STATUS_WAITING:lvi.pszText = (LPTSTR)_T("打印机正在等待"); break;
                                            case  PRINTER_STATUS_WARMING_UP:lvi.pszText = (LPTSTR)_T("打印机正在升温"); break;
                                            default:lvi.pszText = (LPTSTR)_T("打印机状态未知"); break;
                                            }
                                            break;
                                        case 2:lvi.pszText = pPrinterInfo2->pServerName; break;
                                        case 3: lvi.pszText = pPrinterInfo2->pDriverName; break;
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
                                    for (size_t i = 0; *(pPrinterInfo2->pPrinterName + i) != '\0'; i++)
                                    {
                                        switch (*(pPrinterInfo2->pPrinterName + i))
                                        {
                                        case _T(' '):*(pPrinterInfo2->pPrinterName + i) = _T('_'); break;
                                        case _T('\r'):*(pPrinterInfo2->pPrinterName + i) = _T('-'); break;
                                        case _T('\n'):*(pPrinterInfo2->pPrinterName + i) = _T('~'); break;
                                        default:break;
                                        }
                                    }
                                    lvfi.flags = LVFI_PARTIAL | LVFI_STRING;
                                    lvfi.psz = pPrinterInfo2->pPrinterName;
                                    lvi.mask = LVIF_TEXT;
                                    lvi.iItem = ListView_FindItem(hListData, -1, &lvfi);
                                    if (lvi.iItem != (-1))
                                    {
                                        lvi.iSubItem = 1;
                                        switch (lvi.iSubItem)
                                        {
                                        case 0:lvi.pszText = pPrinterInfo2->pPrinterName; break;
                                        case 1:
                                            switch (pPrinterInfo2->Status)
                                            {
                                            case PRINTER_STATUS_BUSY:lvi.pszText = (LPTSTR)_T("打印机正忙"); break;
                                            case PRINTER_STATUS_DOOR_OPEN: lvi.pszText = (LPTSTR)_T("打印机门打开"); break;
                                            case PRINTER_STATUS_ERROR:lvi.pszText = (LPTSTR)_T("打印机处于错误状态"); break;
                                            case  PRINTER_STATUS_INITIALIZING:lvi.pszText = (LPTSTR)_T("打印机正在初始化"); break;
                                            case  PRINTER_STATUS_IO_ACTIVE:lvi.pszText = (LPTSTR)_T("打印机处于活动输入/输出状态"); break;
                                            case  PRINTER_STATUS_MANUAL_FEED:lvi.pszText = (LPTSTR)_T("打印机处于手动进纸状态"); break;
                                            case  PRINTER_STATUS_NO_TONER:lvi.pszText = (LPTSTR)_T("打印机墨粉不足"); break;
                                            case  PRINTER_STATUS_NOT_AVAILABLE:lvi.pszText = (LPTSTR)_T("打印机不可用于打印"); break;
                                            case  PRINTER_STATUS_OFFLINE:lvi.pszText = (LPTSTR)_T("打印机离线"); break;
                                            case  PRINTER_STATUS_OUT_OF_MEMORY:lvi.pszText = (LPTSTR)_T("打印机内存不足"); break;
                                            case  PRINTER_STATUS_OUTPUT_BIN_FULL:lvi.pszText = (LPTSTR)_T("打印机的出纸槽已满"); break;
                                            case  PRINTER_STATUS_PAGE_PUNT:lvi.pszText = (LPTSTR)_T("打印机无法打印当前页面。Windows 95/98/Me：表示页面正在“打印”（即未打印），因为打印机打印过于复杂"); break;
                                            case  PRINTER_STATUS_PAPER_JAM:lvi.pszText = (LPTSTR)_T("纸张卡在打印机中"); break;
                                            case  PRINTER_STATUS_PAPER_OUT:lvi.pszText = (LPTSTR)_T("打印机缺纸"); break;
                                            case  PRINTER_STATUS_PAPER_PROBLEM:lvi.pszText = (LPTSTR)_T("打印机有纸问题"); break;
                                            case  PRINTER_STATUS_PAUSED:lvi.pszText = (LPTSTR)_T("打印机已暂停"); break;
                                            case  PRINTER_STATUS_PENDING_DELETION:lvi.pszText = (LPTSTR)_T("打印机正在删除"); break;
                                            case  PRINTER_STATUS_POWER_SAVE:lvi.pszText = (LPTSTR)_T("打印机处于省电模式"); break;
                                            case  PRINTER_STATUS_PRINTING:lvi.pszText = (LPTSTR)_T("打印机正在打印"); break;
                                            case  PRINTER_STATUS_PROCESSING:lvi.pszText = (LPTSTR)_T("打印机正在处理打印作业"); break;
                                            case  PRINTER_STATUS_SERVER_UNKNOWN:lvi.pszText = (LPTSTR)_T("打印机状态未知"); break;
                                            case  PRINTER_STATUS_TONER_LOW:lvi.pszText = (LPTSTR)_T("打印机墨粉不足"); break;
                                            case  PRINTER_STATUS_USER_INTERVENTION:lvi.pszText = (LPTSTR)_T("打印机有一个错误，需要用户做某事"); break;
                                            case  PRINTER_STATUS_WAITING:lvi.pszText = (LPTSTR)_T("打印机正在等待"); break;
                                            case  PRINTER_STATUS_WARMING_UP:lvi.pszText = (LPTSTR)_T("打印机正在升温"); break;
                                            default:lvi.pszText = (LPTSTR)_T("打印机状态未知"); break;
                                            }
                                            break;
                                        case 2:lvi.pszText = pPrinterInfo2->pServerName; break;
                                        case 3: lvi.pszText = pPrinterInfo2->pDriverName; break;
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
                                            case 0:lvi.pszText = pPrinterInfo2->pPrinterName; break;
                                            case 1:
                                                switch (pPrinterInfo2->Status)
                                                {
                                                case PRINTER_STATUS_BUSY:lvi.pszText = (LPTSTR)_T("打印机正忙"); break;
                                                case PRINTER_STATUS_DOOR_OPEN: lvi.pszText = (LPTSTR)_T("打印机门打开"); break;
                                                case PRINTER_STATUS_ERROR:lvi.pszText = (LPTSTR)_T("打印机处于错误状态"); break;
                                                case  PRINTER_STATUS_INITIALIZING:lvi.pszText = (LPTSTR)_T("打印机正在初始化"); break;
                                                case  PRINTER_STATUS_IO_ACTIVE:lvi.pszText = (LPTSTR)_T("打印机处于活动输入/输出状态"); break;
                                                case  PRINTER_STATUS_MANUAL_FEED:lvi.pszText = (LPTSTR)_T("打印机处于手动进纸状态"); break;
                                                case  PRINTER_STATUS_NO_TONER:lvi.pszText = (LPTSTR)_T("打印机墨粉不足"); break;
                                                case  PRINTER_STATUS_NOT_AVAILABLE:lvi.pszText = (LPTSTR)_T("打印机不可用于打印"); break;
                                                case  PRINTER_STATUS_OFFLINE:lvi.pszText = (LPTSTR)_T("打印机离线"); break;
                                                case  PRINTER_STATUS_OUT_OF_MEMORY:lvi.pszText = (LPTSTR)_T("打印机内存不足"); break;
                                                case  PRINTER_STATUS_OUTPUT_BIN_FULL:lvi.pszText = (LPTSTR)_T("打印机的出纸槽已满"); break;
                                                case  PRINTER_STATUS_PAGE_PUNT:lvi.pszText = (LPTSTR)_T("打印机无法打印当前页面。Windows 95/98/Me：表示页面正在“打印”（即未打印），因为打印机打印过于复杂"); break;
                                                case  PRINTER_STATUS_PAPER_JAM:lvi.pszText = (LPTSTR)_T("纸张卡在打印机中"); break;
                                                case  PRINTER_STATUS_PAPER_OUT:lvi.pszText = (LPTSTR)_T("打印机缺纸"); break;
                                                case  PRINTER_STATUS_PAPER_PROBLEM:lvi.pszText = (LPTSTR)_T("打印机有纸问题"); break;
                                                case  PRINTER_STATUS_PAUSED:lvi.pszText = (LPTSTR)_T("打印机已暂停"); break;
                                                case  PRINTER_STATUS_PENDING_DELETION:lvi.pszText = (LPTSTR)_T("打印机正在删除"); break;
                                                case  PRINTER_STATUS_POWER_SAVE:lvi.pszText = (LPTSTR)_T("打印机处于省电模式"); break;
                                                case  PRINTER_STATUS_PRINTING:lvi.pszText = (LPTSTR)_T("打印机正在打印"); break;
                                                case  PRINTER_STATUS_PROCESSING:lvi.pszText = (LPTSTR)_T("打印机正在处理打印作业"); break;
                                                case  PRINTER_STATUS_SERVER_UNKNOWN:lvi.pszText = (LPTSTR)_T("打印机状态未知"); break;
                                                case  PRINTER_STATUS_TONER_LOW:lvi.pszText = (LPTSTR)_T("打印机墨粉不足"); break;
                                                case  PRINTER_STATUS_USER_INTERVENTION:lvi.pszText = (LPTSTR)_T("打印机有一个错误，需要用户做某事"); break;
                                                case  PRINTER_STATUS_WAITING:lvi.pszText = (LPTSTR)_T("打印机正在等待"); break;
                                                case  PRINTER_STATUS_WARMING_UP:lvi.pszText = (LPTSTR)_T("打印机正在升温"); break;
                                                default:lvi.pszText = (LPTSTR)_T("打印机状态未知"); break;
                                                }
                                                break;
                                            case 2:lvi.pszText = pPrinterInfo2->pServerName; break;
                                            case 3: lvi.pszText = pPrinterInfo2->pDriverName; break;
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
