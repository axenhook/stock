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

typedef struct _WIN_INFO
{
    WCHAR *caption;     /* ���� */
    WCHAR *className;   /* ���� */
    HWND hwnd;          /* ��ǰ���ھ�� */
} WIN_INFO;

/* ��MyFindWind�������� */
BOOL CALLBACK MyEnumProc(HWND hwnd, LPARAM lParam)
{
    WCHAR text[MAX_LENGTH];
    CString str;
    BOOL bRet;

    if (NULL != ((WIN_INFO *)lParam)->caption)
    { /* ��Ҫ�Ƚϱ��� */
        GetWindowText(hwnd, text, MAX_LENGTH);
        wprintf(_T("%s: %d\n"), text, hwnd);

        str.Format(_T("%s"), text);

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
            wprintf(_T("%s: %d\n"), text, hwnd);

            str.Format(_T("%s"), text);

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
HWND MyFindWind(HWND hParent, WCHAR *className, WCHAR *caption)
{
    WIN_INFO wi;

    wi.caption = caption;
    wi.className = className;
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

int GetExePath(HWND hwnd, WCHAR *path, int len)
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

void GetModulePath(HWND hwnd, WCHAR *path, int len)
{
    HMODULE hm;
    hm = (HMODULE)GetClassLong(hwnd, GCL_HMODULE);
    GetModuleFileName(hm, path, len);
}


void SetWindowActive(HWND hwnd)
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

/* ��鴰������ */
static int LVCheckWndType(HWND hwnd)
{
    /* ��ȡ���ڵ����� */
    WCHAR strClassName[MAX_LENGTH]; /* �����Ŵ������� */

    assert (NULL != hwnd);

    GetClassName(hwnd, strClassName, MAX_LENGTH);

    /* �ж��Ƿ���ListBox���͵Ŀؼ� */
    CString str = strClassName;
    /*MessageBox(NULL, str, "��Ϣ�����", MB_OKCANCEL); */
    int pos = str.Find(L"ListView");
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
        printf("error code: %d\n", GetLastError());
        return -3;
    }

    /* ��ָ�����̵�����ռ��б������ύ�ڴ����� */
    plvitem = (LVITEMA *)VirtualAllocEx(hProcess, NULL,
                            /*sizeof(LVITEMA), MEM_COMMIT, PAGE_READONLY); */
                            sizeof(LVITEMA), MEM_COMMIT, PAGE_READWRITE);
    if (!plvitem)
    {
        /*MessageBox(NULL, "�޷������ڴ棡", "����", NULL); */
        printf("error code: %d\n", GetLastError());
        CloseHandle(hProcess);
        return -4;

    }

    pItem = (char *)VirtualAllocEx(hProcess, NULL, MAX_LENGTH,
                                /*MEM_COMMIT, PAGE_READONLY); */
                                MEM_COMMIT, PAGE_READWRITE);
    if (!pItem)
    {
        printf("error code: %d\n", GetLastError());
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

#if 1

void DebugWndLib(void)
{
    CString str;
    HWND hwnd;
    long rowNum;
    long columnNum;
    char lvContent[MAX_LENGTH];

    EnableDebugPrivilege();

#if 1
    hwnd = MyFindWind(NULL, NULL, _T("���������"));
    if (hwnd != NULL)
    {
        hwnd = MyFindWind(hwnd, _T("SysListView32"), NULL);
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
            wprintf(_T("Content: %s\n"), lvContent);
        }
    }

#endif

#if 0
    hwnd = MyFindWind(NULL, NULL, L"��ͼ");
    if (hwnd != NULL)
    {
        WCHAR caption[MAX_LENGTH];
        WCHAR exePath[MAX_LENGTH], modulePath[MAX_LENGTH];
        GetWindowText(hwnd, caption, MAX_LENGTH);
        GetExePath(hwnd, exePath, sizeof(exePath));
        GetModulePath(hwnd, modulePath, sizeof(modulePath));
        str.Format(_T("hwnd = %d, caption = %s, exepath = %s, modulepath = %s"),
            hwnd, caption, exePath, modulePath);
    }
    else
    {
        str.Format(_T("%s"), _T("Can't find the windows"));
    }
    MessageBox(NULL, str, L"��Ϣ�����", MB_OKCANCEL);
#endif

#if 0
    hwnd = FindWindow(NULL, "TestWindow");
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

