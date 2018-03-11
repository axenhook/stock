/*******************************************************************************
��Ȩ����, 2010~2013, ������ (zeng_hr@163.com)
�ļ���: TCLEXTCMD.CPP
����: ������ (zeng_hr@163.com)  ����: 2010��4��13��
��������: 
�����б�: 
  1. ...: 
  
�޸���ʷ: 
  ����: ������ (zeng_hr@163.com)  ����: 2010��4��13��
  ------------------------------------------------------------------------------
  ��ʼ�汾
*******************************************************************************/

#define USE_INTERP_RESULT

#include <stdlib.h>
#include <tcl.h>

#include <afx.h>

#include "KbdMouseLib.h"
#include "WndLib.h"

#pragma comment(lib, "tcl85.lib")

#define INT_MAX_LEN    16
#define CMD_MAX_LEN    1024

//#define USING_UNICODE

/*
  interp->result��Ԥ����ռ�ֻ��200���ֽڣ��������Ľ���ĳ��ȳ���200���ֽڣ�
  ��ô��ʹ�ú���Tcl_SetResult��Tcl_AppendResult��������������
*/
#define TCL_RESULT_MAX_LEN    200

/******************************************************************************
�����ǲ���������غ���
******************************************************************************/

/* ��������������� */
int ParseKeyCode(Tcl_Interp* interp, int objc, Tcl_Obj * const objv[],
                int *piKeyCode)
{
    int iLen;
    
    if (objc != 2)
    {
        _snprintf(interp->result, TCL_RESULT_MAX_LEN, "Usage: %s keyCode",
                Tcl_GetStringFromObj(objv[0], &iLen));
        return TCL_ERROR;
    }
    
    if (TCL_OK != Tcl_GetIntFromObj(interp, objv[1], piKeyCode))
    {
        _snprintf(interp->result, TCL_RESULT_MAX_LEN, "Expect integer but got %s", 
            Tcl_GetStringFromObj(objv[1], &iLen));
        return TCL_ERROR;
    }
    
    return TCL_OK;
}

/* ������ʱ�� */
int ParseTime(Tcl_Interp* interp, int objc, Tcl_Obj * const objv[],
                int *piTime)
{
    int iLen;
    
    if (objc != 2)
    {
        _snprintf(interp->result, TCL_RESULT_MAX_LEN, "Usage: %s time_ms",
                Tcl_GetStringFromObj(objv[0], &iLen));
        return TCL_ERROR;
    }
    
    if (TCL_OK != Tcl_GetIntFromObj(interp, objv[1], piTime))
    {
        _snprintf(interp->result, TCL_RESULT_MAX_LEN, "Expect integer but got %s", 
            Tcl_GetStringFromObj(objv[1], &iLen));
        return TCL_ERROR;
    }
    
    return TCL_OK;
}

/* ������������� */
int ParseXY(Tcl_Interp* interp, int objc, Tcl_Obj * const objv[],
                int *piX, int *piY)
{
    int iLen;
    
    if (objc != 3)
    {
        _snprintf(interp->result, TCL_RESULT_MAX_LEN, "Usage: %s x y",
                Tcl_GetStringFromObj(objv[0], &iLen));
        return TCL_ERROR;
    }
    
    if (TCL_OK != Tcl_GetIntFromObj(interp, objv[1], piX))
    {
        _snprintf(interp->result, TCL_RESULT_MAX_LEN, "Expect integer but got %s", 
            Tcl_GetStringFromObj(objv[1], &iLen));
        return TCL_ERROR;
    }
    
    if (TCL_OK != Tcl_GetIntFromObj(interp, objv[2], piY))
    {
        _snprintf(interp->result, TCL_RESULT_MAX_LEN, "Expect integer but got %s", 
            Tcl_GetStringFromObj(objv[2], &iLen));
        return TCL_ERROR;
    }
    
    return TCL_OK;
}

/* ���������ھ������ */
int ParseHwnd(Tcl_Interp* interp, int objc, Tcl_Obj * const objv[],
                HWND *pHwnd)
{
    int iLen;
    
    if (objc != 2)
    {
        _snprintf(interp->result, TCL_RESULT_MAX_LEN, "Usage: %s hwnd",
                Tcl_GetStringFromObj(objv[0], &iLen));
        return TCL_ERROR;
    }
    
    if (TCL_OK != Tcl_GetLongFromObj(interp, objv[1], (long *)pHwnd))
    {
        _snprintf(interp->result, TCL_RESULT_MAX_LEN, "Expect integer but got %s", 
            Tcl_GetStringFromObj(objv[1], &iLen));
        return TCL_ERROR;
    }
    
    return TCL_OK;
}

/* ���������ھ����������� */
int ParseHwndAndKeyCode(Tcl_Interp* interp, int objc, Tcl_Obj * const objv[],
                    HWND *pHwnd, int *piKeyCode)
{
    if (objc != 3)
    {
        int iLen;
        
        _snprintf(interp->result, TCL_RESULT_MAX_LEN, "Usage: %s hwnd keyCode",
                Tcl_GetStringFromObj(objv[0], &iLen));
        return TCL_ERROR;
    }
    
    if (TCL_OK != ParseHwnd(interp, 2, &objv[0], pHwnd))
    {
        return TCL_ERROR;
    }
    
    return (ParseKeyCode(interp, 2, &objv[1], piKeyCode));
}

/* ���������ھ����������� */
int ParseHwndAndXY(Tcl_Interp* interp, int objc, Tcl_Obj * const objv[],
                    HWND *pHwnd, int *piX, int *piY)
{
    if (objc != 4)
    {
        int iLen;
        
        _snprintf(interp->result, TCL_RESULT_MAX_LEN, "Usage: %s hwnd x y",
                Tcl_GetStringFromObj(objv[0], &iLen));
        return TCL_ERROR;
    }
    
    if (TCL_OK != ParseHwnd(interp, 2, &objv[0], pHwnd))
    {
        return TCL_ERROR;
    }
    
    return (ParseXY(interp, 3, &objv[1], piX, piY));
}

/******************************************************************************
���º�����ʱ��������
******************************************************************************/
/* �ȴ�һ��ʱ�� */
int CmdSleep(ClientData clientdata, Tcl_Interp* interp,
                int objc, Tcl_Obj * const objv[])
{
    int tm;
    
    if (TCL_OK != ParseTime(interp, objc, objv, &tm))
    {
        return TCL_ERROR;
    }
    
    // �ȴ�һ��ʱ��, ��λms
    Sleep(tm);
    
    return TCL_OK;
}

/* �ַ������� */
int CmdSplit(ClientData clientdata, Tcl_Interp* interp,
                    int argc, CONST84 char * argv[])
{
    int pos;
    char *src;
    char split;
    char target[INT_MAX_LEN];
    int i, j;
    int len;
    
    if (argc != 4)
    {
        _snprintf(interp->result, TCL_RESULT_MAX_LEN, "Usage: %s str split pos",
                argv[0]);
        return TCL_ERROR;
    }
    
    if (strlen(argv[2]) != 1)
    {
        sprintf(interp->result,"Expect single char but got %s", argv[2]);
        return TCL_ERROR;
    }
    
    if (TCL_OK != Tcl_GetInt(interp, argv[3], &pos))
    {
        sprintf(interp->result,"Expect integer but got %s", argv[3]);
        return TCL_ERROR;
    }
    
    src = (char *)argv[1];
    split = argv[2][0];
    len = strlen(src);

    for (i = 0; i < len; i++)
    {
        if (pos == 0)
        {
            break;
        }
        
        if (src[i] == split)
        {
            pos--;
        }
    }

    if (i == len)
    {
        sprintf(interp->result,"The target not found. %s", argv[1]);
        return TCL_ERROR;
    }

    memset(target, 0, INT_MAX_LEN);
    for (j = 0; j < (INT_MAX_LEN - 1), i < len; j++, i++)
    {
        if (src[i] == split)
        {
            break;
        }

        target[j] = src[i];
    }
    
    _snprintf(interp->result, TCL_RESULT_MAX_LEN, "%s", target);
    return TCL_OK;
}

/* ����� */
int CmdRand(ClientData clientdata, Tcl_Interp* interp,
                    int argc, CONST84 char * argv[])
{
    if (argc != 1)
    {
        _snprintf(interp->result, TCL_RESULT_MAX_LEN, "Usage: %s",
                argv[0]);
        return TCL_ERROR;
    }

    _snprintf(interp->result, TCL_RESULT_MAX_LEN, "%d", rand());
    return TCL_OK;
}

/* ִ���ⲿ���� */
int CmdExec(ClientData clientdata, Tcl_Interp* interp,
                    int argc, CONST84 char * argv[])
{
    if (argc != 2)
    {
        _snprintf(interp->result, TCL_RESULT_MAX_LEN, "Usage: %s path", argv[0]);
        return TCL_ERROR;
    }

    #ifdef USING_UNICODE
    
    /* �����ַ�ת��ΪASCII */
    char *ascii = Convert(argv[1], CP_UTF8, CP_ACP);
    WinExec(ascii, SW_SHOW);
    delete ascii;
    
    #else
    
    WinExec(argv[1], SW_SHOW);
    
    #endif
    
    return TCL_OK;
}

/******************************************************************************
���º����봰�ڲ������
******************************************************************************/
/* ���ұ����а���ָ���ַ����ĵ�һ������ */
int CmdWindowFind(ClientData clientdata, Tcl_Interp* interp,
                    int argc, CONST84 char * argv[])
{
    HWND hwnd;
    char *className;
    char *caption;
    
    if (argc != 4)
    {
        _snprintf(interp->result, TCL_RESULT_MAX_LEN,
            "Usage: %s hwnd_parent class_name caption", argv[0]);
        return TCL_ERROR;
    }
    
    if (TCL_OK != Tcl_GetInt(interp, argv[1], (int *)&hwnd))
    {
        sprintf(interp->result,"Expect integer but got %s", argv[1]);
        return TCL_ERROR;
    }

    if ((strlen(argv[2]) == 1) && (argv[2][0] == '0'))
    {
        className = NULL;
    }
    else
    {
        className = (char *)argv[2];
    }
    
    if ((strlen(argv[3]) == 1) && (argv[3][0] == '0'))
    {
        caption = NULL;
    }
    else
    {
        caption = (char *)argv[3];
    }

    #ifdef USING_UNICODE
    
    /* �����ַ�ת��ΪASCII */
    char *ascii = Convert(caption, CP_UTF8, CP_ACP);
    hwnd = WindowFind(hwnd, className, ascii);
	delete ascii;
    
    #else
    
    hwnd = WindowFind(hwnd, className, caption);

    #endif
    
    _snprintf(interp->result, TCL_RESULT_MAX_LEN, "%ld", hwnd);
    return TCL_OK;
}

/* ��ȡ���ڵĸ����� */
int CmdWindowRoot(ClientData clientdata, Tcl_Interp* interp,
                    int argc, CONST84 char * argv[])
{
    HWND hwnd;
    
    if (argc != 2)
    {
        _snprintf(interp->result, TCL_RESULT_MAX_LEN, "Usage: %s hwnd",
                argv[0]);
        return TCL_ERROR;
    }
    
    if (TCL_OK != Tcl_GetInt(interp, argv[1], (int *)&hwnd))
    {
        sprintf(interp->result,"Expect integer but got %s", argv[1]);
        return TCL_ERROR;
    }
    
    // ��ȡroot
    hwnd = GetAncestor(hwnd, GA_ROOT);
    
    _snprintf(interp->result, TCL_RESULT_MAX_LEN, "%ld", hwnd);
    return TCL_OK;
}

/* ��ȡ���ڵ�ӵ����, Ҳ���Ǵ����� */
int CmdWindowOwner(ClientData clientdata, Tcl_Interp* interp,
                    int argc, CONST84 char * argv[])
{
    HWND hwnd;
    
    if (argc != 2)
    {
        _snprintf(interp->result, TCL_RESULT_MAX_LEN, "Usage: %s hwnd",
                argv[0]);
        return TCL_ERROR;
    }
    
    if (TCL_OK != Tcl_GetInt(interp, argv[1], (int *)&hwnd))
    {
        sprintf(interp->result,"Expect integer but got %s", argv[1]);
        return TCL_ERROR;
    }
    
    // ��ȡowner
    hwnd = GetAncestor(hwnd, GA_ROOTOWNER);
    
    _snprintf(interp->result, TCL_RESULT_MAX_LEN, "%ld", hwnd);
    return TCL_OK;
}

/* ��ȡ���ڵĸ����� */
int CmdWindowParent(ClientData clientdata, Tcl_Interp* interp,
                    int argc, CONST84 char * argv[])
{
    HWND hwnd;
    
    if (argc != 2)
    {
        _snprintf(interp->result, TCL_RESULT_MAX_LEN, "Usage: %s hwnd",
                argv[0]);
        return TCL_ERROR;
    }
    
    if (TCL_OK != Tcl_GetInt(interp, argv[1], (int *)&hwnd))
    {
        sprintf(interp->result,"Expect integer but got %s", argv[1]);
        return TCL_ERROR;
    }
    
    // ��ȡparent
    hwnd = GetParent(hwnd);
    
    _snprintf(interp->result, TCL_RESULT_MAX_LEN, "%ld", hwnd);
    return TCL_OK;
}

/* ��ȡ���ڵ�λ�� */
int CmdWindowRect(ClientData clientdata, Tcl_Interp* interp,
                    int argc, CONST84 char * argv[])
{
    HWND hwnd;
    
    if (argc != 2)
    {
        _snprintf(interp->result, TCL_RESULT_MAX_LEN, "Usage: %s hwnd",
                argv[0]);
        return TCL_ERROR;
    }
    
    if (TCL_OK != Tcl_GetInt(interp, argv[1], (int *)&hwnd))
    {
        sprintf(interp->result,"Expect integer but got %s", argv[1]);
        return TCL_ERROR;
    }
    
    // ��ȡ����λ��
	RECT tRect;
	GetWindowRect(hwnd, &tRect);

    _snprintf(interp->result, TCL_RESULT_MAX_LEN, "%d %d %d %d",
        tRect.left, tRect.top, tRect.right-tRect.left, tRect.bottom-tRect.top);
    return TCL_OK;
}

/* ��ȡָ�����ڵĿ�ִ�г������� */
int CmdGetExePath(ClientData clientdata, Tcl_Interp* interp,
                int objc, Tcl_Obj * const objv[])
{
    HWND hwnd;
    int iLen;
    char szPath[MAX_LENGTH];
    
    if (TCL_OK != ParseHwnd(interp, objc, objv, &hwnd))
    {
        return TCL_ERROR;
    }
    
    /* ��ȡִ��·�� */
    if (GetExePath(hwnd, szPath, MAX_LENGTH))
    { // ʧ����
        _snprintf(interp->result, TCL_RESULT_MAX_LEN, "%s", "Get exe path failed");
        return TCL_ERROR;
    }
    
    _snprintf(interp->result, TCL_RESULT_MAX_LEN, "%s", szPath);
    return TCL_OK;
}

/* �����ڼ��� */
int CmdWindowActivate(ClientData clientdata, Tcl_Interp* interp,
                int objc, Tcl_Obj * const objv[])
{
    HWND hwnd;
    
    if (TCL_OK != ParseHwnd(interp, objc, objv, &hwnd))
    {
        return TCL_ERROR;
    }
    
    /* ����� */
    WindowActivate(hwnd);
    return TCL_OK;
}

/* �����ڹر� */
int CmdWindowClose(ClientData clientdata, Tcl_Interp* interp,
                int objc, Tcl_Obj * const objv[])
{
    HWND hwnd;
    
    if (TCL_OK != ParseHwnd(interp, objc, objv, &hwnd))
    {
        return TCL_ERROR;
    }
    
    /* �رմ��� */
    WindowClose(hwnd);
    return TCL_OK;
}

/* ��������С�� */
int CmdWindowMinimize(ClientData clientdata, Tcl_Interp* interp,
                int objc, Tcl_Obj * const objv[])
{
    HWND hwnd;
    
    if (TCL_OK != ParseHwnd(interp, objc, objv, &hwnd))
    {
        return TCL_ERROR;
    }
    
    /* ��С������ */
    WindowMinimize(hwnd);
    return TCL_OK;
}

/* ��������� */
int CmdWindowMaximize(ClientData clientdata, Tcl_Interp* interp,
                int objc, Tcl_Obj * const objv[])
{
    HWND hwnd;
    
    if (TCL_OK != ParseHwnd(interp, objc, objv, &hwnd))
    {
        return TCL_ERROR;
    }
    
    /* ��󻯴��� */
    WindowMaximize(hwnd);
    return TCL_OK;
}

/* ���������� */
int CmdWindowHide(ClientData clientdata, Tcl_Interp* interp,
                int objc, Tcl_Obj * const objv[])
{
    HWND hwnd;
    
    if (TCL_OK != ParseHwnd(interp, objc, objv, &hwnd))
    {
        return TCL_ERROR;
    }
    
    /* ���ش��� */
    WindowHide(hwnd);
    return TCL_OK;
}

/* ��������ʾ */
int CmdWindowShow(ClientData clientdata, Tcl_Interp* interp,
                int objc, Tcl_Obj * const objv[])
{
    HWND hwnd;
    
    if (TCL_OK != ParseHwnd(interp, objc, objv, &hwnd))
    {
        return TCL_ERROR;
    }
    
    /* ��ʾ���� */
    WindowShow(hwnd);
    return TCL_OK;
}

/* �����ڻ�ԭ */
int CmdWindowRestore(ClientData clientdata, Tcl_Interp* interp,
                int objc, Tcl_Obj * const objv[])
{
    HWND hwnd;
    
    if (TCL_OK != ParseHwnd(interp, objc, objv, &hwnd))
    {
        return TCL_ERROR;
    }
    
    /* ��ԭ���� */
    WindowRestore(hwnd);
    return TCL_OK;
}

/* ��ȡListView���ڵ���Ŀ��Ŀ */
int CmdGetLVItemNum(ClientData clientdata, Tcl_Interp* interp,
                int objc, Tcl_Obj * const objv[])
{
    HWND hwnd;
    
    if (TCL_OK != ParseHwnd(interp, objc, objv, &hwnd))
    {
        return TCL_ERROR;
    }
    
    _snprintf(interp->result, TCL_RESULT_MAX_LEN, "%ld", LVGetRowNum(hwnd));
    return TCL_OK;
}

/* ��ȡListView������ָ��λ�õ����� */
int CmdGetLVItemContent(ClientData clientdata, Tcl_Interp* interp,
                int objc, Tcl_Obj * const objv[])
{
    HWND hwnd;
    int row, column;
    char content[MAX_LENGTH];
    
    if (TCL_OK != ParseHwndAndXY(interp, objc, objv, &hwnd, &row, &column))
    {
        return TCL_ERROR;
    }
    
    content[0] = 0; /* �����ַ���Ϊ�� */
    LVGetItemContent(hwnd, row, column, content, MAX_LENGTH);
    
    _snprintf(interp->result, TCL_RESULT_MAX_LEN, "%s", content);
    return TCL_OK;
}

/******************************************************************************
���º����밴���������
******************************************************************************/

/* ���Ͱ������µ���Ϣ������ָ��λ�� */
int CmdSendKeyDown(ClientData clientdata, Tcl_Interp* interp,
                    int objc, Tcl_Obj * const objv[])
{
    int iKeyCode;
    HWND hwnd;
    
    if (TCL_OK != ParseHwndAndKeyCode(interp, objc, objv, &hwnd, &iKeyCode))
    {
        return TCL_ERROR;
    }
    
    // ���Ͱ������µ���Ϣ
    SendKeyDown(hwnd, iKeyCode);
    return TCL_OK;
}

/* ���Ͱ����������Ϣ������ָ��λ�� */
int CmdSendKeyUp(ClientData clientdata, Tcl_Interp* interp,
                    int objc, Tcl_Obj * const objv[])
{
    int iKeyCode;
    HWND hwnd;
    
    if (TCL_OK != ParseHwndAndKeyCode(interp, objc, objv, &hwnd, &iKeyCode))
    {
        return TCL_ERROR;
    }
    
    // ���Ͱ����������Ϣ
    SendKeyUp(hwnd, iKeyCode);
    return TCL_OK;
}

/* ���Ͱ�����һ�ε���Ϣ������ָ��λ�� */
int CmdSendKeyPress(ClientData clientdata, Tcl_Interp* interp,
                    int objc, Tcl_Obj * const objv[])
{
    int iKeyCode;
    HWND hwnd;
    
    if (TCL_OK != ParseHwndAndKeyCode(interp, objc, objv, &hwnd, &iKeyCode))
    {
        return TCL_ERROR;
    }
    
    // ���Ͱ�����һ�ε���Ϣ
    SendKeyPress(hwnd, iKeyCode);
    return TCL_OK;
}

/* ���Ͱ����ַ�������Ϣ������ָ��λ�� */
int CmdSendKeyString(ClientData clientdata, Tcl_Interp* interp,
                    int objc, Tcl_Obj * const objv[])
{
    int iLen;
    HWND hwnd;
    
    if (objc != 3)
    {
        _snprintf(interp->result, TCL_RESULT_MAX_LEN, "Usage: %s hwnd string",
                Tcl_GetStringFromObj(objv[0], &iLen));
        return TCL_ERROR;
    }
    
    if (TCL_OK != ParseHwnd(interp, 2, objv, &hwnd))
    {
        return TCL_ERROR;
    }
    
    // ���Ͱ����ַ�������Ϣ
    SendKeyString(hwnd, Tcl_GetStringFromObj(objv[2], &iLen));
    return TCL_OK;
}

/* �������� */
int CmdKeyDown(ClientData clientdata, Tcl_Interp* interp,
                int objc, Tcl_Obj * const objv[])
{
    int iKeyCode;
    
    if (TCL_OK != ParseKeyCode(interp, objc, objv, &iKeyCode))
    {
        return TCL_ERROR;
    }
    
    // ��������
    KeyDown(iKeyCode);
    return TCL_OK;
}

/* �������� */
int CmdKeyUp(ClientData clientdata, Tcl_Interp* interp,
                int objc, Tcl_Obj * const objv[])
{
    int iKeyCode;
    
    if (TCL_OK != ParseKeyCode(interp, objc, objv, &iKeyCode))
    {
        return TCL_ERROR;
    }
    
    // ��������
    KeyUp(iKeyCode);
    return TCL_OK;
}

/* ������һ�� */
int CmdKeyPress(ClientData clientdata, Tcl_Interp* interp,
                int objc, Tcl_Obj * const objv[])
{
    int iKeyCode;
    
    if (TCL_OK != ParseKeyCode(interp, objc, objv, &iKeyCode))
    {
        return TCL_ERROR;
    }
    
    // ������һ��
    KeyPress(iKeyCode);
    return TCL_OK;
}

/* ��һ���ַ��� */
int CmdKeyString(ClientData clientdata, Tcl_Interp* interp,
                int objc, Tcl_Obj * const objv[])
{
    int iLen;
    char *pszStr = NULL;
    
    if (objc != 2)
    {
        _snprintf(interp->result, TCL_RESULT_MAX_LEN, "Usage: %s string",
                Tcl_GetStringFromObj(objv[0], &iLen));
        return TCL_ERROR;
    }


    pszStr = Tcl_GetStringFromObj(objv[1], &iLen);
    
    if (NULL == pszStr)
    {
        _snprintf(interp->result, TCL_RESULT_MAX_LEN, "%s", "Unexpect error.");
        return TCL_ERROR;
    }
    
    // �����ַ���
    KeyString(pszStr, iLen);
    
    return TCL_OK;
}

/******************************************************************************
���º��������������
******************************************************************************/

/* �������ָ���ƶ���ָ��λ�õ���Ϣ������ָ��λ�� */
int CmdSendMouseMove(ClientData clientdata, Tcl_Interp* interp,
                    int objc, Tcl_Obj * const objv[])
{
    int iX, iY;
    HWND hwnd;
    
    if (TCL_OK != ParseHwndAndXY(interp, objc, objv, &hwnd, &iX, &iY))
    {
        return TCL_ERROR;
    }
    
    // �ƶ���ָ��λ��
    SendMouseMove(hwnd, iX, iY);
    return TCL_OK;
}

/* �������������µ���Ϣ������ָ��λ�� */
int CmdSendMouseLeftDown(ClientData clientdata, Tcl_Interp* interp,
                    int objc, Tcl_Obj * const objv[])
{
    int iX, iY;
    HWND hwnd;
    
    if (TCL_OK != ParseHwndAndXY(interp, objc, objv, &hwnd, &iX, &iY))
    {
        return TCL_ERROR;
    }
    
    // �������
    SendMouseDown(hwnd, MOUSE_LEFT, iX, iY);
    return TCL_OK;
}

/* ��������м����µ���Ϣ������ָ��λ�� */
int CmdSendMouseMiddleDown(ClientData clientdata, Tcl_Interp* interp,
                    int objc, Tcl_Obj * const objv[])
{
    int iX, iY;
    HWND hwnd;
    
    if (TCL_OK != ParseHwndAndXY(interp, objc, objv, &hwnd, &iX, &iY))
    {
        return TCL_ERROR;
    }
    
    // �м�����
    SendMouseDown(hwnd, MOUSE_MIDDLE, iX, iY);
    return TCL_OK;
}

/* ��������Ҽ����µ���Ϣ������ָ��λ�� */
int CmdSendMouseRightDown(ClientData clientdata, Tcl_Interp* interp,
                    int objc, Tcl_Obj * const objv[])
{
    int iX, iY;
    HWND hwnd;
    
    if (TCL_OK != ParseHwndAndXY(interp, objc, objv, &hwnd, &iX, &iY))
    {
        return TCL_ERROR;
    }
    
    // �Ҽ�����
    SendMouseDown(hwnd, MOUSE_RIGHT, iX, iY);
    return TCL_OK;
}

/* �����������������Ϣ������ָ��λ�� */
int CmdSendMouseLeftUp(ClientData clientdata, Tcl_Interp* interp,
                    int objc, Tcl_Obj * const objv[])
{
    HWND hwnd;
    
    if (TCL_OK != ParseHwnd(interp, objc, objv, &hwnd))
    {
        return TCL_ERROR;
    }
    
    // �������
    SendMouseUp(hwnd, MOUSE_LEFT);
    return TCL_OK;
}

/* ��������м��������Ϣ������ָ��λ�� */
int CmdSendMouseMiddleUp(ClientData clientdata, Tcl_Interp* interp,
                    int objc, Tcl_Obj * const objv[])
{
    HWND hwnd;
    
    if (TCL_OK != ParseHwnd(interp, objc, objv, &hwnd))
    {
        return TCL_ERROR;
    }
    
    // �м�����
    SendMouseUp(hwnd, MOUSE_MIDDLE);
    return TCL_OK;
}

/* ��������Ҽ��������Ϣ������ָ��λ�� */
int CmdSendMouseRightUp(ClientData clientdata, Tcl_Interp* interp,
                    int objc, Tcl_Obj * const objv[])
{
    HWND hwnd;
    
    if (TCL_OK != ParseHwnd(interp, objc, objv, &hwnd))
    {
        return TCL_ERROR;
    }
    
    // �Ҽ�����
    SendMouseUp(hwnd, MOUSE_RIGHT);
    return TCL_OK;
}

/* ������������������Ϣ������ָ��λ�� */
int CmdSendMouseLeftClick(ClientData clientdata, Tcl_Interp* interp,
                    int objc, Tcl_Obj * const objv[])
{
    int iX, iY;
    HWND hwnd;
    
    if (TCL_OK != ParseHwndAndXY(interp, objc, objv, &hwnd, &iX, &iY))
    {
        return TCL_ERROR;
    }
    
    // �������
    SendMouseClick(hwnd, MOUSE_LEFT, iX, iY);
    return TCL_OK;
}

/* ��������м���������Ϣ������ָ��λ�� */
int CmdSendMouseMiddleClick(ClientData clientdata, Tcl_Interp* interp,
                        int objc, Tcl_Obj * const objv[])
{
    int iX, iY;
    HWND hwnd;
    
    if (TCL_OK != ParseHwndAndXY(interp, objc, objv, &hwnd, &iX, &iY))
    {
        return TCL_ERROR;
    }
    
    // �м�����
    SendMouseClick(hwnd, MOUSE_MIDDLE, iX, iY);
    return TCL_OK;
}

/* ��������Ҽ���������Ϣ������ָ��λ�� */
int CmdSendMouseRightClick(ClientData clientdata, Tcl_Interp* interp,
                    int objc, Tcl_Obj * const objv[])
{
    int iX, iY;
    HWND hwnd;
    
    if (TCL_OK != ParseHwndAndXY(interp, objc, objv, &hwnd, &iX, &iY))
    {
        return TCL_ERROR;
    }
    
    // �Ҽ�����
    SendMouseClick(hwnd, MOUSE_RIGHT, iX, iY);
    return TCL_OK;
}

/* ����������˫������Ϣ������ָ��λ�� */
int CmdSendMouseLeftDblClick(ClientData clientdata, Tcl_Interp* interp,
                        int objc, Tcl_Obj * const objv[])
{
    int iX, iY;
    HWND hwnd;
    
    if (TCL_OK != ParseHwndAndXY(interp, objc, objv, &hwnd, &iX, &iY))
    {
        return TCL_ERROR;
    }
    
    // ���˫��
    SendMouseDblClick(hwnd, MOUSE_LEFT, iX, iY);
    return TCL_OK;
}

/* ��������м�˫������Ϣ������ָ��λ�� */
int CmdSendMouseMiddleDblClick(ClientData clientdata, Tcl_Interp* interp,
                    int objc, Tcl_Obj * const objv[])
{
    int iX, iY;
    HWND hwnd;
    
    if (TCL_OK != ParseHwndAndXY(interp, objc, objv, &hwnd, &iX, &iY))
    {
        return TCL_ERROR;
    }
    
    // �м�˫��
    SendMouseDblClick(hwnd, MOUSE_MIDDLE, iX, iY);
    return TCL_OK;
}

/* ��������Ҽ�˫������Ϣ������ָ��λ�� */
int CmdSendMouseRightDblClick(ClientData clientdata, Tcl_Interp* interp,
                    int objc, Tcl_Obj * const objv[])
{
    int iX, iY;
    HWND hwnd;
    
    if (TCL_OK != ParseHwndAndXY(interp, objc, objv, &hwnd, &iX, &iY))
    {
        return TCL_ERROR;
    }
    
    // �Ҽ�˫��
    SendMouseDblClick(hwnd, MOUSE_RIGHT, iX, iY);
    return TCL_OK;
}

/* ���ָ���ƶ�������ָ��λ�� */
int CmdMouseMoveR(ClientData clientdata, Tcl_Interp* interp, int objc, 
                    Tcl_Obj * const objv[])
{
    int iX, iY;
    HWND hwnd;
    
    if (TCL_OK != ParseHwndAndXY(interp, objc, objv, &hwnd, &iX, &iY))
    {
        return TCL_ERROR;
    }
    
    // �ƶ���ָ��λ��
    MouseMoveR(hwnd, iX, iY);
    return TCL_OK;
}

/* ���ָ���ƶ�����Ļָ��λ�� */
int CmdMouseMove(ClientData clientdata, Tcl_Interp* interp, int objc, 
                    Tcl_Obj * const objv[])
{
    int iX, iY;
    
    if (TCL_OK != ParseXY(interp, objc, objv, &iX, &iY))
    {
        return TCL_ERROR;
    }
    
    // �ƶ���ָ��λ��
    MouseMove(iX, iY);
    return TCL_OK;
}

/* ���������� */
int CmdMouseLeftDown(ClientData clientdata, Tcl_Interp* interp,
                    int argc, CONST84 char * argv[])
{
    MouseDown(MOUSE_LEFT);
    return TCL_OK;
}

/* ����м����� */
int CmdMouseMiddleDown(ClientData clientdata, Tcl_Interp* interp,
                    int argc, CONST84 char * argv[])
{
    MouseDown(MOUSE_MIDDLE);
    return TCL_OK;
}

/* ����Ҽ����� */
int CmdMouseRightDown(ClientData clientdata, Tcl_Interp* interp,
                    int argc, CONST84 char * argv[])
{
    MouseDown(MOUSE_RIGHT);
    return TCL_OK;
}

/* ���������� */
int CmdMouseLeftUp(ClientData clientdata, Tcl_Interp* interp,
                int argc, CONST84 char * argv[])
{
    MouseUp(MOUSE_LEFT);
    return TCL_OK;
}

/* ����м����� */
int CmdMouseMiddleUp(ClientData clientdata, Tcl_Interp* interp,
                    int argc, CONST84 char * argv[])
{
    MouseUp(MOUSE_MIDDLE);
    return TCL_OK;
}

/* ����Ҽ����� */
int CmdMouseRightUp(ClientData clientdata, Tcl_Interp* interp,
                    int argc, CONST84 char * argv[])
{
    MouseUp(MOUSE_RIGHT);
    return TCL_OK;
}

/* ���������� */
int CmdMouseLeftClick(ClientData clientdata, Tcl_Interp* interp,
                    int argc, CONST84 char * argv[])
{
    MouseClick(MOUSE_LEFT);
    return TCL_OK;
}

/* ����м����� */
int CmdMouseMiddleClick(ClientData clientdata, Tcl_Interp* interp,
                        int argc, CONST84 char * argv[])
{
    MouseClick(MOUSE_MIDDLE);
    return TCL_OK;
}

/* ����Ҽ����� */
int CmdMouseRightClick(ClientData clientdata, Tcl_Interp* interp,
                    int argc, CONST84 char * argv[])
{
    MouseClick(MOUSE_RIGHT);
    return TCL_OK;
}

/* ������˫�� */
int CmdMouseLeftDblClick(ClientData clientdata, Tcl_Interp* interp,
                        int argc, CONST84 char * argv[])
{
    MouseDblClick(MOUSE_LEFT);
    return TCL_OK;
}

/* ����м�˫�� */
int CmdMouseMiddleDblClick(ClientData clientdata, Tcl_Interp* interp,
                        int argc, CONST84 char * argv[])
{
    MouseDblClick(MOUSE_MIDDLE);
    return TCL_OK;
}

/* ����Ҽ�˫�� */
int CmdMouseRightDblClick(ClientData clientdata, Tcl_Interp* interp,
                        int argc, CONST84 char * argv[])
{
    MouseDblClick(MOUSE_RIGHT);
    return TCL_OK;
}

/******************************************************************************
��������: Tcl_AppInit
����˵��: tcl ����������չָ��ĳ�ʼ������
�������: ��
�������: ��
�� �� ֵ: ��
˵    ��: ��
******************************************************************************/
int Tcl_AppInit(Tcl_Interp *interp)
{ 
    /* ʱ��������� */
    Tcl_CreateObjCommand(interp, "sleep", CmdSleep,
                        NULL, NULL);
    Tcl_CreateCommand(interp, "exec", CmdExec,
                        NULL, NULL);
    Tcl_CreateCommand(interp, "split", CmdSplit,
                        NULL, NULL);
    Tcl_CreateCommand(interp, "rand", CmdRand,
                        NULL, NULL);
                        
    /* ����������� */
    Tcl_CreateCommand(interp, "win_find", CmdWindowFind,
                        NULL, NULL);
    Tcl_CreateCommand(interp, "win_rect", CmdWindowRect,
                        NULL, NULL);
    Tcl_CreateCommand(interp, "win_parent", CmdWindowParent,
                        NULL, NULL);
    Tcl_CreateCommand(interp, "win_root", CmdWindowRoot,
                        NULL, NULL);
    Tcl_CreateCommand(interp, "win_owner", CmdWindowOwner,
                        NULL, NULL);
    Tcl_CreateObjCommand(interp, "win_activate", CmdWindowActivate,
                        NULL, NULL);
    Tcl_CreateObjCommand(interp, "win_close", CmdWindowClose,
                        NULL, NULL);
    Tcl_CreateObjCommand(interp, "win_min", CmdWindowMinimize,
                        NULL, NULL);
    Tcl_CreateObjCommand(interp, "win_max", CmdWindowMaximize,
                        NULL, NULL);
    Tcl_CreateObjCommand(interp, "win_hide", CmdWindowHide,
                        NULL, NULL);
    Tcl_CreateObjCommand(interp, "win_show", CmdWindowShow,
                        NULL, NULL);
    Tcl_CreateObjCommand(interp, "win_restore", CmdWindowRestore,
                        NULL, NULL);
    
    Tcl_CreateObjCommand(interp, "get_exe_path", CmdGetExePath,
                        NULL, NULL);
    Tcl_CreateObjCommand(interp, "get_lv_row", CmdGetLVItemNum,
                        NULL, NULL);
    Tcl_CreateObjCommand(interp, "get_lv_content", CmdGetLVItemContent,
                        NULL, NULL);
                        
    /* ����������� */
    Tcl_CreateObjCommand(interp, "send_key_down", CmdSendKeyDown,
                        NULL, NULL);
    Tcl_CreateObjCommand(interp, "send_key_up", CmdSendKeyUp,
                        NULL, NULL);
    Tcl_CreateObjCommand(interp, "send_key_press", CmdSendKeyPress,
                        NULL, NULL);
    Tcl_CreateObjCommand(interp, "send_key_string", CmdSendKeyString,
                        NULL, NULL);
    Tcl_CreateObjCommand(interp, "key_down", CmdKeyDown,
                        NULL, NULL);
    Tcl_CreateObjCommand(interp, "key_up", CmdKeyUp,
                        NULL, NULL);
    Tcl_CreateObjCommand(interp, "key_press", CmdKeyPress,
                        NULL, NULL);
    Tcl_CreateObjCommand(interp, "key_string", CmdKeyString,
                        NULL, NULL);
    
    /* ���������� */
    Tcl_CreateObjCommand(interp, "send_mouse_mv", CmdSendMouseMove,
                        NULL, NULL);
    Tcl_CreateObjCommand(interp, "send_mouse_ld", CmdSendMouseLeftDown,
                        NULL, NULL);
    Tcl_CreateObjCommand(interp, "send_mouse_md", CmdSendMouseMiddleDown,
                        NULL, NULL);
    Tcl_CreateObjCommand(interp, "send_mouse_rd", CmdSendMouseRightDown,
                        NULL, NULL);
    Tcl_CreateObjCommand(interp, "send_mouse_lu", CmdSendMouseLeftUp,
                        NULL, NULL);
    Tcl_CreateObjCommand(interp, "send_mouse_mu", CmdSendMouseMiddleUp,
                        NULL, NULL);
    Tcl_CreateObjCommand(interp, "send_mouse_ru", CmdSendMouseRightUp,
                        NULL, NULL);
    Tcl_CreateObjCommand(interp, "send_mouse_lclk", CmdSendMouseLeftClick,
                        NULL, NULL);
    Tcl_CreateObjCommand(interp, "send_mouse_mclk", CmdSendMouseMiddleClick,
                        NULL, NULL);
    Tcl_CreateObjCommand(interp, "send_mouse_rclk", CmdSendMouseRightClick,
                        NULL, NULL);
    Tcl_CreateObjCommand(interp, "send_mouse_ldclk", CmdSendMouseLeftDblClick,
                        NULL, NULL);
    Tcl_CreateObjCommand(interp, "send_mouse_mdclk", CmdSendMouseMiddleDblClick,
                        NULL, NULL);
    Tcl_CreateObjCommand(interp, "send_mouse_rdclk", CmdSendMouseRightDblClick,
                        NULL, NULL);
                        
    Tcl_CreateObjCommand(interp, "mouse_mvr", CmdMouseMoveR,
                        NULL, NULL);
    Tcl_CreateObjCommand(interp, "mouse_mv", CmdMouseMove,
                        NULL, NULL);
    Tcl_CreateCommand(interp, "mouse_ld", CmdMouseLeftDown, NULL, NULL);
    Tcl_CreateCommand(interp, "mouse_md", CmdMouseMiddleDown, NULL, NULL);
    Tcl_CreateCommand(interp, "mouse_rd", CmdMouseRightDown, NULL, NULL);
    Tcl_CreateCommand(interp, "mouse_lu", CmdMouseLeftUp, NULL, NULL);
    Tcl_CreateCommand(interp, "mouse_mu", CmdMouseMiddleUp, NULL, NULL);
    Tcl_CreateCommand(interp, "mouse_ru", CmdMouseRightUp, NULL, NULL);
    Tcl_CreateCommand(interp, "mouse_lclk", CmdMouseLeftClick, NULL, NULL);
    Tcl_CreateCommand(interp, "mouse_mclk", CmdMouseMiddleClick, NULL, NULL);
    Tcl_CreateCommand(interp, "mouse_rclk", CmdMouseRightClick, NULL, NULL);
    Tcl_CreateCommand(interp, "mouse_ldclk", CmdMouseLeftDblClick, NULL, NULL);
    Tcl_CreateCommand(interp, "mouse_mdclk", CmdMouseMiddleDblClick, NULL, NULL);
    Tcl_CreateCommand(interp, "mouse_rdclk", CmdMouseRightDblClick, NULL, NULL);
    
    return TCL_OK;
}

/******************************************************************************
��������: ExecuteScriptAndFile
����˵��: ִ���ⲿ��tcl�ļ�
�������: ��
�������: ��
�� �� ֵ: ��
˵    ��: ��
******************************************************************************/
int ExecuteScriptAndFile(const char *script, const char *fileName)
{
    Tcl_Interp* interp = Tcl_CreateInterp();
    
    /* ��ʼ�� */
    if (Tcl_AppInit(interp))
    {
        printf("TCL initialize Error!\n");
        return -1;
    }

    if (NULL != script)
    { /* evaluate the TCL script */
        if (TCL_OK != Tcl_Eval(interp, script))
        {
            printf("There are errors in your script\n");    
            Tcl_DeleteInterp(interp);
            return -2;
        }
    }

    #if 1
    if (NULL != fileName)
    { /* evaluate the TCL description file */
        if (TCL_OK != Tcl_EvalFile(interp, fileName))
        {
            printf("There are errors in your Tcl File\n");    
            Tcl_DeleteInterp(interp);
            return -3;
        }
    }
    #else
    if (NULL != fileName)
    { /* evaluate the TCL description file */
        char szCmd[CMD_MAX_LEN];
        
        _snprintf(szCmd, CMD_MAX_LEN, "source %s\n", fileName);
        
        if (TCL_OK != Tcl_Eval(interp, szCmd))
        {
            printf("There are errors in your Tcl File\n");    
            Tcl_DeleteInterp(interp);
            return -3;
        }
    }
    #endif
    
    Tcl_DeleteInterp(interp);
    return 0;
}

