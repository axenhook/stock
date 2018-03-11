/*******************************************************************************
��Ȩ����, 2010~2013, ������ (zeng_hr@163.com)
�ļ���: WNDLIB.CPP
����: ������ (zeng_hr@163.com)  ����: 2010��4��13��
��������:
�����б�:
  1. ...:

�޸���ʷ:
  ����: ������ (zeng_hr@163.com)  ����: 2010��4��13��
  ------------------------------------------------------------------------------
  ��ʼ�汾
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
    char *caption;     /* ���� */
    char *className;   /* ���� */
    HWND hwnd;          /* ��ǰ���ھ�� */
} WIN_INFO;

/* ��MyFindWind�������� */
BOOL CALLBACK MyEnumProc(HWND hwnd, LPARAM lParam)
{
    char text[MAX_LENGTH];
    CString str;
    BOOL bRet;

    if (NULL != ((WIN_INFO *)lParam)->caption)
    { /* ��Ҫ�Ƚϱ��� */
        GetWindowText(hwnd, text, MAX_LENGTH);
        /*printf("%s: %d\n", text, hwnd);*/

        str.Format("%s", text);

        if (str.Find(((WIN_INFO *)lParam)->caption) != -1)
        {
            ((WIN_INFO *)lParam)->hwnd = hwnd;
            bRet = FALSE; /* ��ֹ��ǰö�� */
        }
        else
        { /* ���ⲻ�� */
            bRet = TRUE; /* ����ö�� */
        }
    }
    else
    { /* ����Ҫ�Ƚϱ��� */
        bRet = FALSE;
    }

    if (NULL != ((WIN_INFO *)lParam)->className)
    { /* ��Ҫ�Ƚ����� */
        if (FALSE == bRet)
        { /* ����Ƚ�û������ */
            GetClassName(hwnd, text, MAX_LENGTH);
            //printf("%s: %d\n", text, hwnd);

            str.Format("%s", text);

            if (str.Find(((WIN_INFO *)lParam)->className) != -1)
            {
                ((WIN_INFO *)lParam)->hwnd = hwnd;
                bRet = FALSE; /* ��ֹ��ǰö�� */
            }
            else
            { /* �������� */
                bRet = TRUE; /* ����ö�� */
            }
        }
    }
    
    return bRet;
}

/* ģ�����Ҵ�ָ���ַ����Ĵ��� */
HWND WindowFind(HWND hParent, const char *className, const char *caption)
{
    WIN_INFO wi;

    wi.caption = (char *)caption;
    wi.className = (char *)className;
    wi.hwnd = NULL;

    if (NULL == hParent)
    { /* ö�����и����� */
        EnumWindows(MyEnumProc, (LPARAM)(&wi));
    }
    else
    { /* ö��ĳ�����ڵ������Ӵ��� */
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
        /*MessageBox(NULL, "��ȡ���̾��ʧ��", "����", NULL ); */
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

/* ��鴰������ */
static int LVCheckWndType(HWND hwnd)
{
    /* ��ȡ���ڵ����� */
    char strClassName[MAX_LENGTH]; /* �����Ŵ������� */

    assert (NULL != hwnd);

    GetClassName(hwnd, strClassName, MAX_LENGTH);

    /* �ж��Ƿ���ListBox���͵Ŀؼ� */
    CString str = strClassName;
    /*MessageBox(NULL, str, "��Ϣ�����", MB_OKCANCEL); */
    int pos = str.Find("ListView");
    if (pos < 0)
    { /* ����ListBox���͵Ŀؼ� */
        return -1;
    }

    return 0;
}

/* ��ȡ������ */
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

/* ��ȡ������ */
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


/* ��ȡָ�����е����� */
int LVGetItemContent(HWND hwnd, long row, long column, char *content, int len)
{
    /* ���ڵ��߳�ID�ͽ���ID */
    DWORD processID, threadID;
    /* �������ڽ��̵ľ�� */
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

    /* ��ȡ�߳�id�ͽ���id */
    threadID = GetWindowThreadProcessId(hwnd, &processID);

    /* ��ȡ���̾�� */
    hProcess = OpenProcess(PROCESS_ALL_ACCESS, false, processID);
    /*hProcess = OpenProcess(PROCESS_VM_READ, false, processID); */
    if (!hProcess)
    {
        /*MessageBox(NULL, "��ȡ���̾��ʧ��", "����", NULL); */
        //printf("error code: %d\n", GetLastError());
        return -3;
    }

    /* ��ָ�����̵�����ռ��б������ύ�ڴ����� */
    plvitem = (LVITEMA *)VirtualAllocEx(hProcess, NULL,
                            /*sizeof(LVITEMA), MEM_COMMIT, PAGE_READONLY); */
                            sizeof(LVITEMA), MEM_COMMIT, PAGE_READWRITE);
    if (!plvitem)
    {
        /*MessageBox(NULL, "�޷������ڴ棡", "����", NULL); */
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
        /*MessageBox(NULL, "�޷������ڴ棡", "����", NULL); */
        VirtualFreeEx(hProcess, plvitem, 0, MEM_RELEASE);
        CloseHandle(hProcess);
        return -5;
    }

    lvitem.cchTextMax = MAX_LENGTH;
    lvitem.iSubItem = column;
    lvitem.pszText = pItem;

    /* ����������д�뵽ָ�����̵��ڴ� */
    WriteProcessMemory(hProcess, plvitem, &lvitem, sizeof(LVITEMA), NULL);

    SendMessage(hwnd, LVM_GETITEMTEXT, (WPARAM)row, (LPARAM)plvitem);
    /* ��ȡָ�����̵��ڴ����ݵ����� */
    ReadProcessMemory(hProcess, pItem, content, len, NULL);

    VirtualFreeEx(hProcess, plvitem, 0, MEM_RELEASE);
    VirtualFreeEx(hProcess, pItem, 0, MEM_RELEASE);
    CloseHandle(hProcess);

    return 0;
}

/* ��ȡָ���е����� */
int LVGetRowContent(HWND hwnd, long row, char *content, int len)
{
    /* ���ڵ��߳�ID�ͽ���ID */
    DWORD processID, threadID;
    /* �������ڽ��̵ľ�� */
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

    /* ��ȡ�߳�id�ͽ���id */
    threadID = GetWindowThreadProcessId(hwnd, &processID);

    /* ��ȡ���̾�� */
    hProcess = OpenProcess(PROCESS_ALL_ACCESS, false, processID);
    /*hProcess = OpenProcess(PROCESS_VM_READ, false, processID); */
    if (!hProcess)
    {
        /*MessageBox(NULL, "��ȡ���̾��ʧ��", "����", NULL); */
        //printf("error code: %d\n", GetLastError());
        return -3;
    }

    /* ��ָ�����̵�����ռ��б������ύ�ڴ����� */
    plvitem = (LVITEMA *)VirtualAllocEx(hProcess, NULL,
                            /*sizeof(LVITEMA), MEM_COMMIT, PAGE_READONLY); */
                            sizeof(LVITEMA), MEM_COMMIT, PAGE_READWRITE);
    if (!plvitem)
    {
        /*MessageBox(NULL, "�޷������ڴ棡", "����", NULL); */
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
        /*MessageBox(NULL, "�޷������ڴ棡", "����", NULL); */
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

        /* ����������д�뵽ָ�����̵��ڴ� */
        WriteProcessMemory(hProcess, plvitem, &lvitem, sizeof(LVITEMA), NULL);

        SendMessage(hwnd, LVM_GETITEMTEXT, (WPARAM)row, (LPARAM)plvitem);
        /* ��ȡָ�����̵��ڴ����ݵ����� */
        ReadProcessMemory(hProcess, pItem, content, len, NULL);

        column++;
        if (column >= columnNum)
        {
            break;
        }

        contentLen = strlen(content);

        if (len == contentLen + 1)
        { /* ˵���Ѿ����� */
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

    if(!result)              // �򿪽��̴���
        return result;

    result   =   LookupPrivilegeValue(NULL,
        SE_DEBUG_NAME,
        &tkp.Privileges[0].Luid);

    if(!result)              //�鿴����Ȩ�޴���
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
    
    char *utf_8 = Convert("���Ž���", CP_ACP, CP_UTF8);
    char *ascii = Convert(utf_8, CP_UTF8, CP_ACP);
    
    hwnd = WindowFind(NULL, NULL, "���Ž���");
    if (hwnd != NULL)
    {
        printf("hwnd = %d\n", hwnd);
    }

#if 1
    hwnd = WindowFind(NULL, NULL, "���������");
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
    hwnd = WindowFind(NULL, NULL, "��ͼ");
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
    MessageBox(NULL, str, "��Ϣ�����", MB_OKCANCEL);
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

    MessageBox(NULL, str, "��Ϣ�����", MB_OKCANCEL);
#endif

    system("pause");
}

#endif

