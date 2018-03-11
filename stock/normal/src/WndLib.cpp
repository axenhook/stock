/*******************************************************************************
版权所有, 2010~2013, 曾华荣 (zeng_hr@163.com)
文件名: WNDLIB.CPP
作者: 曾华荣 (zeng_hr@163.com)  日期: 2010年4月13日
功能描述:
函数列表:
  1. ...:

修改历史:
  作者: 曾华荣 (zeng_hr@163.com)  日期: 2010年4月13日
  ------------------------------------------------------------------------------
  初始版本
*******************************************************************************/
#include <afx.h>
#include <assert.h>

#include "WndLib.h"

#include <psapi.h>
#pragma comment (lib, "../src/psapi.lib")

#define MY_SEND_MESSAGE(hwnd, msg, code, para) PostMessage(hwnd, msg, code, para)
//#define MY_SEND_MESSAGE(hwnd, msg, code, para) SendMessage(hwnd, msg, code, para)

typedef struct _WIN_INFO
{
    char *caption;     /* 标题 */
    char *className;   /* 类名 */
    HWND hwnd;          /* 当前窗口句柄 */
} WIN_INFO;

/* 供MyFindWind函数调用 */
BOOL CALLBACK MyEnumProc(HWND hwnd, LPARAM lParam)
{
    char text[MAX_LENGTH];
    CString str;
    BOOL bRet;

    if (NULL != ((WIN_INFO *)lParam)->caption)
    { /* 需要比较标题 */
        GetWindowText(hwnd, text, MAX_LENGTH);
        /*printf("%s: %d\n", text, hwnd);*/

        str.Format("%s", text);

        if (str.Find(((WIN_INFO *)lParam)->caption) != -1)
        {
            ((WIN_INFO *)lParam)->hwnd = hwnd;
            bRet = FALSE; /* 终止当前枚举 */
        }
        else
        { /* 标题不对 */
            bRet = TRUE; /* 继续枚举 */
        }
    }
    else
    { /* 不需要比较标题 */
        bRet = FALSE;
    }

    if (NULL != ((WIN_INFO *)lParam)->className)
    { /* 需要比较类名 */
        if (FALSE == bRet)
        { /* 标题比较没有问题 */
            GetClassName(hwnd, text, MAX_LENGTH);
            //printf("%s: %d\n", text, hwnd);

            str.Format("%s", text);

            if (str.Find(((WIN_INFO *)lParam)->className) != -1)
            {
                ((WIN_INFO *)lParam)->hwnd = hwnd;
                bRet = FALSE; /* 终止当前枚举 */
            }
            else
            { /* 类名不对 */
                bRet = TRUE; /* 继续枚举 */
            }
        }
    }
    
    return bRet;
}

/* 模糊查找带指定字符串的窗口 */
HWND WindowFind(HWND hParent, const char *className, const char *caption)
{
    WIN_INFO wi;

    wi.caption = (char *)caption;
    wi.className = (char *)className;
    wi.hwnd = NULL;

    if (NULL == hParent)
    { /* 枚举所有父窗口 */
        EnumWindows(MyEnumProc, (LPARAM)(&wi));
    }
    else
    { /* 枚举某个窗口的所有子窗口 */
        EnumChildWindows(hParent, MyEnumProc, (LPARAM)(&wi));
    }
    
    return wi.hwnd;
}

int GetExePath(HWND hwnd, char *path, int len)
{
    DWORD processID, threadID;
    HANDLE hProcess;

    threadID = GetWindowThreadProcessId(hwnd, &processID);
    hProcess = OpenProcess(PROCESS_ALL_ACCESS, false, processID);
    if (!hProcess)
    {
        /*MessageBox(NULL, "获取进程句柄失败", "错误", NULL ); */
        return -1;
    }

    GetModuleFileNameEx(hProcess, 0, path, len);
    CloseHandle(hProcess);
    return 0;
}

void GetModulePath(HWND hwnd, char *path, int len)
{
    HMODULE hm;
    hm = (HMODULE)GetClassLong(hwnd, GCL_HMODULE);
    GetModuleFileName(hm, path, len);
}


void WindowActivate(HWND hwnd)
{
    /*DWORD threadID, processID; */

    /*threadID = GetWindowThreadProcessId(hwnd, &processID); */
   /*AttachThreadInput(threadID, processID, TRUE); */
    ShowWindow(hwnd, SW_SHOW);
    /*SetWindowPos(hwnd, HWND_TOP, 0, 0, 0, 0, SWP_NOMOVE); */
    /*SetForegroundWindow(hwnd); */
    /*AttachThreadInput(threadID, processID, FALSE); */
    SwitchToThisWindow(hwnd, TRUE);
}

void WindowClose(HWND hwnd)
{
    MY_SEND_MESSAGE(hwnd, WM_CLOSE, 0, 0);
}

void WindowMinimize(HWND hwnd)
{
    ShowWindow(hwnd, SW_MINIMIZE);
}

void WindowMaximize(HWND hwnd)
{
    ShowWindow(hwnd, SW_MAXIMIZE);
}

void WindowHide(HWND hwnd)
{
    ShowWindow(hwnd, SW_HIDE);
}

void WindowShow(HWND hwnd)
{
    ShowWindow(hwnd, SW_SHOW);
}

void WindowRestore(HWND hwnd)
{
    ShowWindow(hwnd, SW_RESTORE);
}

/* 检查窗口类型 */
static int LVCheckWndType(HWND hwnd)
{
    /* 获取窗口的类名 */
    char strClassName[MAX_LENGTH]; /* 定义存放窗口类名 */

    assert (NULL != hwnd);

    GetClassName(hwnd, strClassName, MAX_LENGTH);

    /* 判断是否是ListBox类型的控件 */
    CString str = strClassName;
    /*MessageBox(NULL, str, "消息框标题", MB_OKCANCEL); */
    int pos = str.Find("ListView");
    if (pos < 0)
    { /* 不是ListBox类型的控件 */
        return -1;
    }

    return 0;
}

/* 获取总行数 */
long LVGetRowNum(HWND hwnd)
{
    if (NULL == hwnd)
    {
        return -1;
    }

    if (LVCheckWndType(hwnd))
    {
        return -2;
    }

    return (SendMessage(hwnd, LVM_GETITEMCOUNT, 0, 0));
}

/* 获取总列数 */
long LVGetColumnNum(HWND hwnd)
{
    if (NULL == hwnd)
    {
        return -1;
    }

    if (LVCheckWndType(hwnd))
    {
        return -1;
    }

    return (Header_GetItemCount(ListView_GetHeader(hwnd)));
}


/* 获取指定行列的内容 */
int LVGetItemContent(HWND hwnd, long row, long column, char *content, int len)
{
    /* 窗口的线程ID和进程ID */
    DWORD processID, threadID;
    /* 窗口所在进程的句柄 */
    HANDLE hProcess;
    LVITEMA *plvitem = NULL;
    char *pItem = NULL;
    LVITEMA lvitem;

    if (NULL == hwnd)
    {
        return -1;
    }

    if (LVCheckWndType(hwnd))
    {
        return -1;
    }

    /* 获取线程id和进程id */
    threadID = GetWindowThreadProcessId(hwnd, &processID);

    /* 获取进程句柄 */
    hProcess = OpenProcess(PROCESS_ALL_ACCESS, false, processID);
    /*hProcess = OpenProcess(PROCESS_VM_READ, false, processID); */
    if (!hProcess)
    {
        /*MessageBox(NULL, "获取进程句柄失败", "错误", NULL); */
        //printf("error code: %d\n", GetLastError());
        return -3;
    }

    /* 在指定进程的虚拟空间中保留或提交内存区域 */
    plvitem = (LVITEMA *)VirtualAllocEx(hProcess, NULL,
                            /*sizeof(LVITEMA), MEM_COMMIT, PAGE_READONLY); */
                            sizeof(LVITEMA), MEM_COMMIT, PAGE_READWRITE);
    if (!plvitem)
    {
        /*MessageBox(NULL, "无法分配内存！", "错误！", NULL); */
        //printf("error code: %d\n", GetLastError());
        CloseHandle(hProcess);
        return -4;
    }

    pItem = (char *)VirtualAllocEx(hProcess, NULL, MAX_LENGTH,
                                /*MEM_COMMIT, PAGE_READONLY); */
                                MEM_COMMIT, PAGE_READWRITE);
    if (!pItem)
    {
        //printf("error code: %d\n", GetLastError());
        /*MessageBox(NULL, "无法分配内存！", "错误！", NULL); */
        VirtualFreeEx(hProcess, plvitem, 0, MEM_RELEASE);
        CloseHandle(hProcess);
        return -5;
    }

    lvitem.cchTextMax = MAX_LENGTH;
    lvitem.iSubItem = column;
    lvitem.pszText = pItem;

    /* 将本地内容写入到指定进程的内存 */
    WriteProcessMemory(hProcess, plvitem, &lvitem, sizeof(LVITEMA), NULL);

    SendMessage(hwnd, LVM_GETITEMTEXT, (WPARAM)row, (LPARAM)plvitem);
    /* 读取指定进程的内存内容到本地 */
    ReadProcessMemory(hProcess, pItem, content, len, NULL);

    VirtualFreeEx(hProcess, plvitem, 0, MEM_RELEASE);
    VirtualFreeEx(hProcess, pItem, 0, MEM_RELEASE);
    CloseHandle(hProcess);

    return 0;
}

/* 获取指定行的内容 */
int LVGetRowContent(HWND hwnd, long row, char *content, int len)
{
    /* 窗口的线程ID和进程ID */
    DWORD processID, threadID;
    /* 窗口所在进程的句柄 */
    HANDLE hProcess;
    LVITEMA *plvitem = NULL;
    char *pItem = NULL;
    LVITEMA lvitem;
    int columnNum;

    if (NULL == hwnd)
    {
        return -1;
    }

    columnNum = LVGetColumnNum(hwnd);
    if (columnNum <= 0)
    {
        return -1;
    }

    /* 获取线程id和进程id */
    threadID = GetWindowThreadProcessId(hwnd, &processID);

    /* 获取进程句柄 */
    hProcess = OpenProcess(PROCESS_ALL_ACCESS, false, processID);
    /*hProcess = OpenProcess(PROCESS_VM_READ, false, processID); */
    if (!hProcess)
    {
        /*MessageBox(NULL, "获取进程句柄失败", "错误", NULL); */
        //printf("error code: %d\n", GetLastError());
        return -3;
    }

    /* 在指定进程的虚拟空间中保留或提交内存区域 */
    plvitem = (LVITEMA *)VirtualAllocEx(hProcess, NULL,
                            /*sizeof(LVITEMA), MEM_COMMIT, PAGE_READONLY); */
                            sizeof(LVITEMA), MEM_COMMIT, PAGE_READWRITE);
    if (!plvitem)
    {
        /*MessageBox(NULL, "无法分配内存！", "错误！", NULL); */
        //printf("error code: %d\n", GetLastError());
        CloseHandle(hProcess);
        return -4;

    }

    pItem = (char *)VirtualAllocEx(hProcess, NULL, MAX_LENGTH,
                                /*MEM_COMMIT, PAGE_READONLY); */
                                MEM_COMMIT, PAGE_READWRITE);
    if (!pItem)
    {
        //printf("error code: %d\n", GetLastError());
        /*MessageBox(NULL, "无法分配内存！", "错误！", NULL); */
        VirtualFreeEx(hProcess, plvitem, 0, MEM_RELEASE);
        CloseHandle(hProcess);
        return -5;
    }

    for (int column = 0; ;)
    {
        int contentLen;
        
        lvitem.cchTextMax = MAX_LENGTH;
        lvitem.iSubItem = column;
        lvitem.pszText = pItem;

        /* 将本地内容写入到指定进程的内存 */
        WriteProcessMemory(hProcess, plvitem, &lvitem, sizeof(LVITEMA), NULL);

        SendMessage(hwnd, LVM_GETITEMTEXT, (WPARAM)row, (LPARAM)plvitem);
        /* 读取指定进程的内存内容到本地 */
        ReadProcessMemory(hProcess, pItem, content, len, NULL);

        column++;
        if (column >= columnNum)
        {
            break;
        }

        contentLen = strlen(content);

        if (len == contentLen + 1)
        { /* 说明已经满了 */
            break;
        }
        
        content[contentLen] = COLUMN_SIGNATURE;
        content += (contentLen + 1);
        len -= (contentLen + 1);
    }
    
    VirtualFreeEx(hProcess, plvitem, 0, MEM_RELEASE);
    VirtualFreeEx(hProcess, pItem, 0, MEM_RELEASE);
    CloseHandle(hProcess);

    return 0;
}


bool EnableDebugPrivilege(void)
{
    HANDLE hToken;
    LUID sedebugnameValue;
    TOKEN_PRIVILEGES tkp;

    if (!OpenProcessToken(GetCurrentProcess(),
        TOKEN_ADJUST_PRIVILEGES | TOKEN_QUERY, &hToken))
    {
        return   FALSE;
    }

    if (!LookupPrivilegeValue(NULL, SE_DEBUG_NAME, &sedebugnameValue))
    {
        CloseHandle(hToken);
        return false;
    }

    tkp.PrivilegeCount = 1;
    tkp.Privileges[0].Luid = sedebugnameValue;
    tkp.Privileges[0].Attributes = SE_PRIVILEGE_ENABLED;

    if (!AdjustTokenPrivileges(hToken, FALSE, &tkp, sizeof(tkp), NULL, NULL))
    {
        CloseHandle(hToken);
        return false;
    }

    return true;
}

bool UpPrivilege(void)
{
    HANDLE hToken;           // handle   to   process   token
    TOKEN_PRIVILEGES tkp;    // pointer   to   token   structure
    bool result = OpenProcessToken(GetCurrentProcess(),
        TOKEN_ADJUST_PRIVILEGES|TOKEN_QUERY,
        &hToken);

    if(!result)              // 打开进程错误
        return result;

    result   =   LookupPrivilegeValue(NULL,
        SE_DEBUG_NAME,
        &tkp.Privileges[0].Luid);

    if(!result)              //查看进程权限错误
        return   result;

    tkp.PrivilegeCount   =   1;     //   one   privilege   to   set
    tkp.Privileges[0].Attributes   =   SE_PRIVILEGE_ENABLED;
    result   =   AdjustTokenPrivileges(hToken,
        FALSE,
        &tkp,
        sizeof(TOKEN_PRIVILEGES),
        (PTOKEN_PRIVILEGES)   NULL,
        (PDWORD)   NULL);

    return   result;
}

BOOL EnablePrivilege(LPCTSTR lpszPrivilegeName,BOOL bEnable)
{
    HANDLE hToken;
    TOKEN_PRIVILEGES tp;
    LUID luid;
    
    if(!OpenProcessToken(GetCurrentProcess(),TOKEN_ADJUST_PRIVILEGES |
        TOKEN_QUERY | TOKEN_READ,&hToken))
        return FALSE;
    if(!LookupPrivilegeValue(NULL, lpszPrivilegeName, &luid))
        return TRUE;
    
    tp.PrivilegeCount = 1;
    tp.Privileges[0].Luid = luid;
    tp.Privileges[0].Attributes = (bEnable) ? SE_PRIVILEGE_ENABLED : 0;
    
    AdjustTokenPrivileges(hToken,FALSE,&tp,NULL,NULL,NULL);
    
    CloseHandle(hToken);
    
    return (GetLastError() == ERROR_SUCCESS);
    
}

LPSTR Convert(LPCSTR str, int sourceCodepage, int targetCodepage)
{
    int len = _tcslen(str);
    int unicodeLen = MultiByteToWideChar(sourceCodepage, 0, str, -1, NULL, 0);
    wchar_t* pUnicode = new wchar_t[unicodeLen + 1];
    
    memset(pUnicode, 0, (unicodeLen + 1) * sizeof(wchar_t));

    MultiByteToWideChar(sourceCodepage, 0, str, -1,
        (LPWSTR)pUnicode, unicodeLen);

    int targetLen = WideCharToMultiByte(targetCodepage, 0,
        (LPWSTR)pUnicode, -1, NULL, 0, NULL, NULL);
    LPSTR szResult = new TCHAR[targetLen + 1];
    
    memset(szResult, 0, targetLen + 1);

    WideCharToMultiByte(targetCodepage, 0, (LPWSTR)pUnicode, -1,
        (char *)szResult, targetLen, NULL, NULL);

    delete pUnicode;
    
    return szResult;
}

#if 1

void DebugWndLib(void)
{
    CString str;
    HWND hwnd;
    long rowNum;
    long columnNum;
    char lvContent[MAX_LENGTH];

    EnableDebugPrivilege();
    
    char *utf_8 = Convert("安信交易", CP_ACP, CP_UTF8);
    char *ascii = Convert(utf_8, CP_UTF8, CP_ACP);
    
    hwnd = WindowFind(NULL, NULL, "安信交易");
    if (hwnd != NULL)
    {
        printf("hwnd = %d\n", hwnd);
    }

#if 1
    hwnd = WindowFind(NULL, NULL, "任务管理器");
    if (hwnd != NULL)
    {
        hwnd = WindowFind(hwnd, "SysListView32", NULL);
        rowNum = LVGetRowNum(hwnd);
        columnNum = LVGetColumnNum(hwnd);
        printf("Row = %d, Column = %d\n", rowNum, columnNum);

        if (LVGetItemContent(hwnd, 0, 0, lvContent, MAX_LENGTH))
        {
            printf("Get ListView content failed\n");
        }
        else
        {
            printf("Get ListView content success\n");
            printf("Content: %s\n", lvContent);
        }
    }

#endif

#if 1
    hwnd = WindowFind(NULL, NULL, "画图");
    if (hwnd != NULL)
    {
        char caption[MAX_LENGTH];
        char exePath[MAX_LENGTH], modulePath[MAX_LENGTH];
        GetWindowText(hwnd, caption, MAX_LENGTH);
        GetExePath(hwnd, exePath, sizeof(exePath));
        GetModulePath(hwnd, modulePath, sizeof(modulePath));
        str.Format("hwnd = %d, caption = %s, exepath = %s, modulepath = %s",
            hwnd, caption, exePath, modulePath);
    }
    else
    {
        str.Format("%s", "Can't find the windows");
    }
    MessageBox(NULL, str, "消息框标题", MB_OKCANCEL);
#endif

#if 1
    hwnd = WindowFind(NULL, NULL, "TestWindow");
    if (hwnd != NULL)
    {
        str.Format("hwnd = %d", hwnd);
    }
    else
    {
        str.Format("%s", "Can't find the windows");
    }

    MessageBox(NULL, str, "消息框标题", MB_OKCANCEL);
#endif

    system("pause");
}

#endif

