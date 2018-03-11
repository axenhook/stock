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

#pragma comment(lib, "tcl86.lib")

/*
  interp->result��Ԥ����ռ�ֻ��200���ֽڣ��������Ľ���ĳ��ȳ���200���ֽڣ�
  ��ô��ʹ�ú���Tcl_SetResult��Tcl_AppendResult��������������
*/
#define CMD_MAX_LEN    200

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
        _snprintf(interp->result, CMD_MAX_LEN, "Usage: %s keyCode",
                Tcl_GetStringFromObj(objv[0], &iLen));
        return TCL_ERROR;
    }
    
    if (TCL_OK != Tcl_GetIntFromObj(interp, objv[1], piKeyCode))
    {
        _snprintf(interp->result, CMD_MAX_LEN, "Expect integer but got %s", 
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
        _snprintf(interp->result, CMD_MAX_LEN, "Usage: %s time_ms",
                Tcl_GetStringFromObj(objv[0], &iLen));
        return TCL_ERROR;
    }
    
    if (TCL_OK != Tcl_GetIntFromObj(interp, objv[1], piTime))
    {
        _snprintf(interp->result, CMD_MAX_LEN, "Expect integer but got %s", 
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
        _snprintf(interp->result, CMD_MAX_LEN, "Usage: %s x y",
                Tcl_GetStringFromObj(objv[0], &iLen));
        return TCL_ERROR;
    }
    
    if (TCL_OK != Tcl_GetIntFromObj(interp, objv[1], piX))
    {
        _snprintf(interp->result, CMD_MAX_LEN, "Expect integer but got %s", 
            Tcl_GetStringFromObj(objv[1], &iLen));
        return TCL_ERROR;
    }
    
    if (TCL_OK != Tcl_GetIntFromObj(interp, objv[2], piY))
    {
        _snprintf(interp->result, CMD_MAX_LEN, "Expect integer but got %s", 
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
        _snprintf(interp->result, CMD_MAX_LEN, "Usage: %s hwnd",
                Tcl_GetStringFromObj(objv[0], &iLen));
        return TCL_ERROR;
    }
    
    if (TCL_OK != Tcl_GetLongFromObj(interp, objv[1], (long *)pHwnd))
    {
        _snprintf(interp->result, CMD_MAX_LEN, "Expect integer but got %s", 
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
        
        _snprintf(interp->result, CMD_MAX_LEN, "Usage: %s hwnd keyCode",
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
        
        _snprintf(interp->result, CMD_MAX_LEN, "Usage: %s hwnd x y",
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
    
    // �ȴ�һ��ʱ��
    Sleep(tm);
    
    return TCL_OK;
}

/******************************************************************************
���º����봰�ڲ������
******************************************************************************/
/* ���ұ����а���ָ���ַ����ĵ�һ������ */
int CmdFindWindow(ClientData clientdata, Tcl_Interp* interp,
                int objc, Tcl_Obj * const objv[])
{
    HWND hwnd;
    char *pszCaption;
    int iLen;
    WCHAR szCaptionW[MAX_LENGTH]; 
    int iLenW;
    
    if (objc != 2)
    {
        _snprintf(interp->result, CMD_MAX_LEN, "Usage: %s caption",
                Tcl_GetStringFromObj(objv[0], &iLen));
        return TCL_ERROR;
    }
    
    // ģ�����Ҵ���
    pszCaption = Tcl_GetStringFromObj(objv[1], &iLen);
    iLenW = MultiByteToWideChar(CP_ACP, 0, pszCaption, iLen, NULL, 0);
    MultiByteToWideChar(CP_ACP, 0, pszCaption, -1, szCaptionW, iLenW);
    szCaptionW[iLenW] = 0;
    hwnd = MyFindWind(NULL, NULL, szCaptionW);
    
    _snprintf(interp->result, CMD_MAX_LEN, "%ld", hwnd);
    return TCL_OK;
}

/* ��ȡָ�����ڵĿ�ִ�г������� */
int CmdGetExePath(ClientData clientdata, Tcl_Interp* interp,
                int objc, Tcl_Obj * const objv[])
{
    HWND hwnd;
    WCHAR szPathW[MAX_LENGTH];
    int iLen;
    char szPath[MAX_LENGTH];
    
    if (TCL_OK != ParseHwnd(interp, objc, objv, &hwnd))
    {
        return TCL_ERROR;
    }
    
    /* ��ȡִ��·�� */
    if (GetExePath(hwnd, szPathW, MAX_LENGTH))
    { // ʧ����
        _snprintf(interp->result, CMD_MAX_LEN, "%s", "Get exe path failed");
        return TCL_ERROR;
    }
    
    /* ��unicodeת���ɶ��ֽڸ�ʽ */
    iLen = WideCharToMultiByte(CP_ACP, 0, szPathW, -1, NULL, 0, NULL, NULL); 
    WideCharToMultiByte(CP_ACP, 0, szPathW, -1, szPath, iLen, NULL, NULL); 
    szPath[iLen] = 0;
    
    _snprintf(interp->result, CMD_MAX_LEN, "%s", szPath);
    return TCL_OK;
}

/* �����ڼ��� */
int CmdSetWindowActive(ClientData clientdata, Tcl_Interp* interp,
                int objc, Tcl_Obj * const objv[])
{
    HWND hwnd;
    
    if (TCL_OK != ParseHwnd(interp, objc, objv, &hwnd))
    {
        return TCL_ERROR;
    }
    
    /* ����� */
    SetWindowActive(hwnd);
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
    
    _snprintf(interp->result, CMD_MAX_LEN, "%ld", LVGetRowNum(hwnd));
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
    
    _snprintf(interp->result, CMD_MAX_LEN, "%s", content);
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
        _snprintf(interp->result, CMD_MAX_LEN, "Usage: %s hwnd string",
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
                        
    /* ����������� */
    Tcl_CreateObjCommand(interp, "findw", CmdFindWindow,
                        NULL, NULL);
    Tcl_CreateObjCommand(interp, "get_exe_path", CmdGetExePath,
                        NULL, NULL);
    Tcl_CreateObjCommand(interp, "set_win_active", CmdSetWindowActive,
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
    
    if (NULL != fileName)
    { /* evaluate the TCL description file */
        if (TCL_OK != Tcl_EvalFile(interp, fileName))
        {
            printf("There are errors in your Tcl File\n");    
            Tcl_DeleteInterp(interp);
            return -3;
        }
    }
    
    Tcl_DeleteInterp(interp);
    return 0;
}

