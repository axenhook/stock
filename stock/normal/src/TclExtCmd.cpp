/*******************************************************************************
版权所有, 2010~2013, 曾华荣 (zeng_hr@163.com)
文件名: TCLEXTCMD.CPP
作者: 曾华荣 (zeng_hr@163.com)  日期: 2010年4月13日
功能描述: 
函数列表: 
  1. ...: 
  
修改历史: 
  作者: 曾华荣 (zeng_hr@163.com)  日期: 2010年4月13日
  ------------------------------------------------------------------------------
  初始版本
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
  interp->result的预分配空间只有200个字节，如果命令的结果的长度超过200个字节，
  那么请使用函数Tcl_SetResult或Tcl_AppendResult来管理命令结果。
*/
#define TCL_RESULT_MAX_LEN    200

/******************************************************************************
以下是参数解析相关函数
******************************************************************************/

/* 解析出按键码参数 */
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

/* 解析出时间 */
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

/* 解析出坐标参数 */
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

/* 解析出窗口句柄参数 */
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

/* 解析出窗口句柄和坐标参数 */
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

/* 解析出窗口句柄和坐标参数 */
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
以下函数与时间操作相关
******************************************************************************/
/* 等待一段时间 */
int CmdSleep(ClientData clientdata, Tcl_Interp* interp,
                int objc, Tcl_Obj * const objv[])
{
    int tm;
    
    if (TCL_OK != ParseTime(interp, objc, objv, &tm))
    {
        return TCL_ERROR;
    }
    
    // 等待一段时间, 单位ms
    Sleep(tm);
    
    return TCL_OK;
}

/* 字符串分离 */
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

/* 随机数 */
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

/* 执行外部命令 */
int CmdExec(ClientData clientdata, Tcl_Interp* interp,
                    int argc, CONST84 char * argv[])
{
    if (argc != 2)
    {
        _snprintf(interp->result, TCL_RESULT_MAX_LEN, "Usage: %s path", argv[0]);
        return TCL_ERROR;
    }

    #ifdef USING_UNICODE
    
    /* 将宽字符转换为ASCII */
    char *ascii = Convert(argv[1], CP_UTF8, CP_ACP);
    WinExec(ascii, SW_SHOW);
    delete ascii;
    
    #else
    
    WinExec(argv[1], SW_SHOW);
    
    #endif
    
    return TCL_OK;
}

/******************************************************************************
以下函数与窗口操作相关
******************************************************************************/
/* 查找标题中包含指定字符串的第一个窗口 */
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
    
    /* 将宽字符转换为ASCII */
    char *ascii = Convert(caption, CP_UTF8, CP_ACP);
    hwnd = WindowFind(hwnd, className, ascii);
	delete ascii;
    
    #else
    
    hwnd = WindowFind(hwnd, className, caption);

    #endif
    
    _snprintf(interp->result, TCL_RESULT_MAX_LEN, "%ld", hwnd);
    return TCL_OK;
}

/* 获取窗口的根窗口 */
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
    
    // 获取root
    hwnd = GetAncestor(hwnd, GA_ROOT);
    
    _snprintf(interp->result, TCL_RESULT_MAX_LEN, "%ld", hwnd);
    return TCL_OK;
}

/* 获取窗口的拥有者, 也就是创建者 */
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
    
    // 获取owner
    hwnd = GetAncestor(hwnd, GA_ROOTOWNER);
    
    _snprintf(interp->result, TCL_RESULT_MAX_LEN, "%ld", hwnd);
    return TCL_OK;
}

/* 获取窗口的父窗口 */
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
    
    // 获取parent
    hwnd = GetParent(hwnd);
    
    _snprintf(interp->result, TCL_RESULT_MAX_LEN, "%ld", hwnd);
    return TCL_OK;
}

/* 获取窗口的位置 */
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
    
    // 获取窗口位置
	RECT tRect;
	GetWindowRect(hwnd, &tRect);

    _snprintf(interp->result, TCL_RESULT_MAX_LEN, "%d %d %d %d",
        tRect.left, tRect.top, tRect.right-tRect.left, tRect.bottom-tRect.top);
    return TCL_OK;
}

/* 获取指定窗口的可执行程序名称 */
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
    
    /* 获取执行路径 */
    if (GetExePath(hwnd, szPath, MAX_LENGTH))
    { // 失败了
        _snprintf(interp->result, TCL_RESULT_MAX_LEN, "%s", "Get exe path failed");
        return TCL_ERROR;
    }
    
    _snprintf(interp->result, TCL_RESULT_MAX_LEN, "%s", szPath);
    return TCL_OK;
}

/* 将窗口激活 */
int CmdWindowActivate(ClientData clientdata, Tcl_Interp* interp,
                int objc, Tcl_Obj * const objv[])
{
    HWND hwnd;
    
    if (TCL_OK != ParseHwnd(interp, objc, objv, &hwnd))
    {
        return TCL_ERROR;
    }
    
    /* 激活窗口 */
    WindowActivate(hwnd);
    return TCL_OK;
}

/* 将窗口关闭 */
int CmdWindowClose(ClientData clientdata, Tcl_Interp* interp,
                int objc, Tcl_Obj * const objv[])
{
    HWND hwnd;
    
    if (TCL_OK != ParseHwnd(interp, objc, objv, &hwnd))
    {
        return TCL_ERROR;
    }
    
    /* 关闭窗口 */
    WindowClose(hwnd);
    return TCL_OK;
}

/* 将窗口最小化 */
int CmdWindowMinimize(ClientData clientdata, Tcl_Interp* interp,
                int objc, Tcl_Obj * const objv[])
{
    HWND hwnd;
    
    if (TCL_OK != ParseHwnd(interp, objc, objv, &hwnd))
    {
        return TCL_ERROR;
    }
    
    /* 最小化窗口 */
    WindowMinimize(hwnd);
    return TCL_OK;
}

/* 将窗口最大化 */
int CmdWindowMaximize(ClientData clientdata, Tcl_Interp* interp,
                int objc, Tcl_Obj * const objv[])
{
    HWND hwnd;
    
    if (TCL_OK != ParseHwnd(interp, objc, objv, &hwnd))
    {
        return TCL_ERROR;
    }
    
    /* 最大化窗口 */
    WindowMaximize(hwnd);
    return TCL_OK;
}

/* 将窗口隐藏 */
int CmdWindowHide(ClientData clientdata, Tcl_Interp* interp,
                int objc, Tcl_Obj * const objv[])
{
    HWND hwnd;
    
    if (TCL_OK != ParseHwnd(interp, objc, objv, &hwnd))
    {
        return TCL_ERROR;
    }
    
    /* 隐藏窗口 */
    WindowHide(hwnd);
    return TCL_OK;
}

/* 将窗口显示 */
int CmdWindowShow(ClientData clientdata, Tcl_Interp* interp,
                int objc, Tcl_Obj * const objv[])
{
    HWND hwnd;
    
    if (TCL_OK != ParseHwnd(interp, objc, objv, &hwnd))
    {
        return TCL_ERROR;
    }
    
    /* 显示窗口 */
    WindowShow(hwnd);
    return TCL_OK;
}

/* 将窗口还原 */
int CmdWindowRestore(ClientData clientdata, Tcl_Interp* interp,
                int objc, Tcl_Obj * const objv[])
{
    HWND hwnd;
    
    if (TCL_OK != ParseHwnd(interp, objc, objv, &hwnd))
    {
        return TCL_ERROR;
    }
    
    /* 还原窗口 */
    WindowRestore(hwnd);
    return TCL_OK;
}

/* 获取ListView窗口的条目数目 */
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

/* 获取ListView窗口中指定位置的内容 */
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
    
    content[0] = 0; /* 设置字符串为空 */
    LVGetItemContent(hwnd, row, column, content, MAX_LENGTH);
    
    _snprintf(interp->result, TCL_RESULT_MAX_LEN, "%s", content);
    return TCL_OK;
}

/******************************************************************************
以下函数与按键操作相关
******************************************************************************/

/* 发送按键按下的消息到窗口指定位置 */
int CmdSendKeyDown(ClientData clientdata, Tcl_Interp* interp,
                    int objc, Tcl_Obj * const objv[])
{
    int iKeyCode;
    HWND hwnd;
    
    if (TCL_OK != ParseHwndAndKeyCode(interp, objc, objv, &hwnd, &iKeyCode))
    {
        return TCL_ERROR;
    }
    
    // 发送按键按下的消息
    SendKeyDown(hwnd, iKeyCode);
    return TCL_OK;
}

/* 发送按键弹起的消息到窗口指定位置 */
int CmdSendKeyUp(ClientData clientdata, Tcl_Interp* interp,
                    int objc, Tcl_Obj * const objv[])
{
    int iKeyCode;
    HWND hwnd;
    
    if (TCL_OK != ParseHwndAndKeyCode(interp, objc, objv, &hwnd, &iKeyCode))
    {
        return TCL_ERROR;
    }
    
    // 发送按键弹起的消息
    SendKeyUp(hwnd, iKeyCode);
    return TCL_OK;
}

/* 发送按键按一次的消息到窗口指定位置 */
int CmdSendKeyPress(ClientData clientdata, Tcl_Interp* interp,
                    int objc, Tcl_Obj * const objv[])
{
    int iKeyCode;
    HWND hwnd;
    
    if (TCL_OK != ParseHwndAndKeyCode(interp, objc, objv, &hwnd, &iKeyCode))
    {
        return TCL_ERROR;
    }
    
    // 发送按键按一次的消息
    SendKeyPress(hwnd, iKeyCode);
    return TCL_OK;
}

/* 发送按键字符串的消息到窗口指定位置 */
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
    
    // 发送按键字符串的消息
    SendKeyString(hwnd, Tcl_GetStringFromObj(objv[2], &iLen));
    return TCL_OK;
}

/* 按键按下 */
int CmdKeyDown(ClientData clientdata, Tcl_Interp* interp,
                int objc, Tcl_Obj * const objv[])
{
    int iKeyCode;
    
    if (TCL_OK != ParseKeyCode(interp, objc, objv, &iKeyCode))
    {
        return TCL_ERROR;
    }
    
    // 按键按下
    KeyDown(iKeyCode);
    return TCL_OK;
}

/* 按键弹起 */
int CmdKeyUp(ClientData clientdata, Tcl_Interp* interp,
                int objc, Tcl_Obj * const objv[])
{
    int iKeyCode;
    
    if (TCL_OK != ParseKeyCode(interp, objc, objv, &iKeyCode))
    {
        return TCL_ERROR;
    }
    
    // 按键弹起
    KeyUp(iKeyCode);
    return TCL_OK;
}

/* 按键按一次 */
int CmdKeyPress(ClientData clientdata, Tcl_Interp* interp,
                int objc, Tcl_Obj * const objv[])
{
    int iKeyCode;
    
    if (TCL_OK != ParseKeyCode(interp, objc, objv, &iKeyCode))
    {
        return TCL_ERROR;
    }
    
    // 按键按一次
    KeyPress(iKeyCode);
    return TCL_OK;
}

/* 按一串字符串 */
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
    
    // 按键字符串
    KeyString(pszStr, iLen);
    
    return TCL_OK;
}

/******************************************************************************
以下函数与鼠标操作相关
******************************************************************************/

/* 发送鼠标指针移动到指定位置的消息到窗口指定位置 */
int CmdSendMouseMove(ClientData clientdata, Tcl_Interp* interp,
                    int objc, Tcl_Obj * const objv[])
{
    int iX, iY;
    HWND hwnd;
    
    if (TCL_OK != ParseHwndAndXY(interp, objc, objv, &hwnd, &iX, &iY))
    {
        return TCL_ERROR;
    }
    
    // 移动到指定位置
    SendMouseMove(hwnd, iX, iY);
    return TCL_OK;
}

/* 发送鼠标左键按下的消息到窗口指定位置 */
int CmdSendMouseLeftDown(ClientData clientdata, Tcl_Interp* interp,
                    int objc, Tcl_Obj * const objv[])
{
    int iX, iY;
    HWND hwnd;
    
    if (TCL_OK != ParseHwndAndXY(interp, objc, objv, &hwnd, &iX, &iY))
    {
        return TCL_ERROR;
    }
    
    // 左键按下
    SendMouseDown(hwnd, MOUSE_LEFT, iX, iY);
    return TCL_OK;
}

/* 发送鼠标中键按下的消息到窗口指定位置 */
int CmdSendMouseMiddleDown(ClientData clientdata, Tcl_Interp* interp,
                    int objc, Tcl_Obj * const objv[])
{
    int iX, iY;
    HWND hwnd;
    
    if (TCL_OK != ParseHwndAndXY(interp, objc, objv, &hwnd, &iX, &iY))
    {
        return TCL_ERROR;
    }
    
    // 中键按下
    SendMouseDown(hwnd, MOUSE_MIDDLE, iX, iY);
    return TCL_OK;
}

/* 发送鼠标右键按下的消息到窗口指定位置 */
int CmdSendMouseRightDown(ClientData clientdata, Tcl_Interp* interp,
                    int objc, Tcl_Obj * const objv[])
{
    int iX, iY;
    HWND hwnd;
    
    if (TCL_OK != ParseHwndAndXY(interp, objc, objv, &hwnd, &iX, &iY))
    {
        return TCL_ERROR;
    }
    
    // 右键按下
    SendMouseDown(hwnd, MOUSE_RIGHT, iX, iY);
    return TCL_OK;
}

/* 发送鼠标左键弹起的消息到窗口指定位置 */
int CmdSendMouseLeftUp(ClientData clientdata, Tcl_Interp* interp,
                    int objc, Tcl_Obj * const objv[])
{
    HWND hwnd;
    
    if (TCL_OK != ParseHwnd(interp, objc, objv, &hwnd))
    {
        return TCL_ERROR;
    }
    
    // 左键弹起
    SendMouseUp(hwnd, MOUSE_LEFT);
    return TCL_OK;
}

/* 发送鼠标中键弹起的消息到窗口指定位置 */
int CmdSendMouseMiddleUp(ClientData clientdata, Tcl_Interp* interp,
                    int objc, Tcl_Obj * const objv[])
{
    HWND hwnd;
    
    if (TCL_OK != ParseHwnd(interp, objc, objv, &hwnd))
    {
        return TCL_ERROR;
    }
    
    // 中键弹起
    SendMouseUp(hwnd, MOUSE_MIDDLE);
    return TCL_OK;
}

/* 发送鼠标右键弹起的消息到窗口指定位置 */
int CmdSendMouseRightUp(ClientData clientdata, Tcl_Interp* interp,
                    int objc, Tcl_Obj * const objv[])
{
    HWND hwnd;
    
    if (TCL_OK != ParseHwnd(interp, objc, objv, &hwnd))
    {
        return TCL_ERROR;
    }
    
    // 右键弹起
    SendMouseUp(hwnd, MOUSE_RIGHT);
    return TCL_OK;
}

/* 发送鼠标左键单击的消息到窗口指定位置 */
int CmdSendMouseLeftClick(ClientData clientdata, Tcl_Interp* interp,
                    int objc, Tcl_Obj * const objv[])
{
    int iX, iY;
    HWND hwnd;
    
    if (TCL_OK != ParseHwndAndXY(interp, objc, objv, &hwnd, &iX, &iY))
    {
        return TCL_ERROR;
    }
    
    // 左键单击
    SendMouseClick(hwnd, MOUSE_LEFT, iX, iY);
    return TCL_OK;
}

/* 发送鼠标中键单击的消息到窗口指定位置 */
int CmdSendMouseMiddleClick(ClientData clientdata, Tcl_Interp* interp,
                        int objc, Tcl_Obj * const objv[])
{
    int iX, iY;
    HWND hwnd;
    
    if (TCL_OK != ParseHwndAndXY(interp, objc, objv, &hwnd, &iX, &iY))
    {
        return TCL_ERROR;
    }
    
    // 中键单击
    SendMouseClick(hwnd, MOUSE_MIDDLE, iX, iY);
    return TCL_OK;
}

/* 发送鼠标右键单击的消息到窗口指定位置 */
int CmdSendMouseRightClick(ClientData clientdata, Tcl_Interp* interp,
                    int objc, Tcl_Obj * const objv[])
{
    int iX, iY;
    HWND hwnd;
    
    if (TCL_OK != ParseHwndAndXY(interp, objc, objv, &hwnd, &iX, &iY))
    {
        return TCL_ERROR;
    }
    
    // 右键单击
    SendMouseClick(hwnd, MOUSE_RIGHT, iX, iY);
    return TCL_OK;
}

/* 发送鼠标左键双击的消息到窗口指定位置 */
int CmdSendMouseLeftDblClick(ClientData clientdata, Tcl_Interp* interp,
                        int objc, Tcl_Obj * const objv[])
{
    int iX, iY;
    HWND hwnd;
    
    if (TCL_OK != ParseHwndAndXY(interp, objc, objv, &hwnd, &iX, &iY))
    {
        return TCL_ERROR;
    }
    
    // 左键双击
    SendMouseDblClick(hwnd, MOUSE_LEFT, iX, iY);
    return TCL_OK;
}

/* 发送鼠标中键双击的消息到窗口指定位置 */
int CmdSendMouseMiddleDblClick(ClientData clientdata, Tcl_Interp* interp,
                    int objc, Tcl_Obj * const objv[])
{
    int iX, iY;
    HWND hwnd;
    
    if (TCL_OK != ParseHwndAndXY(interp, objc, objv, &hwnd, &iX, &iY))
    {
        return TCL_ERROR;
    }
    
    // 中键双击
    SendMouseDblClick(hwnd, MOUSE_MIDDLE, iX, iY);
    return TCL_OK;
}

/* 发送鼠标右键双击的消息到窗口指定位置 */
int CmdSendMouseRightDblClick(ClientData clientdata, Tcl_Interp* interp,
                    int objc, Tcl_Obj * const objv[])
{
    int iX, iY;
    HWND hwnd;
    
    if (TCL_OK != ParseHwndAndXY(interp, objc, objv, &hwnd, &iX, &iY))
    {
        return TCL_ERROR;
    }
    
    // 右键双击
    SendMouseDblClick(hwnd, MOUSE_RIGHT, iX, iY);
    return TCL_OK;
}

/* 鼠标指针移动到窗口指定位置 */
int CmdMouseMoveR(ClientData clientdata, Tcl_Interp* interp, int objc, 
                    Tcl_Obj * const objv[])
{
    int iX, iY;
    HWND hwnd;
    
    if (TCL_OK != ParseHwndAndXY(interp, objc, objv, &hwnd, &iX, &iY))
    {
        return TCL_ERROR;
    }
    
    // 移动到指定位置
    MouseMoveR(hwnd, iX, iY);
    return TCL_OK;
}

/* 鼠标指针移动到屏幕指定位置 */
int CmdMouseMove(ClientData clientdata, Tcl_Interp* interp, int objc, 
                    Tcl_Obj * const objv[])
{
    int iX, iY;
    
    if (TCL_OK != ParseXY(interp, objc, objv, &iX, &iY))
    {
        return TCL_ERROR;
    }
    
    // 移动到指定位置
    MouseMove(iX, iY);
    return TCL_OK;
}

/* 鼠标左键按下 */
int CmdMouseLeftDown(ClientData clientdata, Tcl_Interp* interp,
                    int argc, CONST84 char * argv[])
{
    MouseDown(MOUSE_LEFT);
    return TCL_OK;
}

/* 鼠标中键按下 */
int CmdMouseMiddleDown(ClientData clientdata, Tcl_Interp* interp,
                    int argc, CONST84 char * argv[])
{
    MouseDown(MOUSE_MIDDLE);
    return TCL_OK;
}

/* 鼠标右键按下 */
int CmdMouseRightDown(ClientData clientdata, Tcl_Interp* interp,
                    int argc, CONST84 char * argv[])
{
    MouseDown(MOUSE_RIGHT);
    return TCL_OK;
}

/* 鼠标左键弹起 */
int CmdMouseLeftUp(ClientData clientdata, Tcl_Interp* interp,
                int argc, CONST84 char * argv[])
{
    MouseUp(MOUSE_LEFT);
    return TCL_OK;
}

/* 鼠标中键弹起 */
int CmdMouseMiddleUp(ClientData clientdata, Tcl_Interp* interp,
                    int argc, CONST84 char * argv[])
{
    MouseUp(MOUSE_MIDDLE);
    return TCL_OK;
}

/* 鼠标右键弹起 */
int CmdMouseRightUp(ClientData clientdata, Tcl_Interp* interp,
                    int argc, CONST84 char * argv[])
{
    MouseUp(MOUSE_RIGHT);
    return TCL_OK;
}

/* 鼠标左键单击 */
int CmdMouseLeftClick(ClientData clientdata, Tcl_Interp* interp,
                    int argc, CONST84 char * argv[])
{
    MouseClick(MOUSE_LEFT);
    return TCL_OK;
}

/* 鼠标中键单击 */
int CmdMouseMiddleClick(ClientData clientdata, Tcl_Interp* interp,
                        int argc, CONST84 char * argv[])
{
    MouseClick(MOUSE_MIDDLE);
    return TCL_OK;
}

/* 鼠标右键单击 */
int CmdMouseRightClick(ClientData clientdata, Tcl_Interp* interp,
                    int argc, CONST84 char * argv[])
{
    MouseClick(MOUSE_RIGHT);
    return TCL_OK;
}

/* 鼠标左键双击 */
int CmdMouseLeftDblClick(ClientData clientdata, Tcl_Interp* interp,
                        int argc, CONST84 char * argv[])
{
    MouseDblClick(MOUSE_LEFT);
    return TCL_OK;
}

/* 鼠标中键双击 */
int CmdMouseMiddleDblClick(ClientData clientdata, Tcl_Interp* interp,
                        int argc, CONST84 char * argv[])
{
    MouseDblClick(MOUSE_MIDDLE);
    return TCL_OK;
}

/* 鼠标右键双击 */
int CmdMouseRightDblClick(ClientData clientdata, Tcl_Interp* interp,
                        int argc, CONST84 char * argv[])
{
    MouseDblClick(MOUSE_RIGHT);
    return TCL_OK;
}

/******************************************************************************
函数名称: Tcl_AppInit
功能说明: tcl 解释器和扩展指令的初始化函数
输入参数: 无
输出参数: 无
返 回 值: 无
说    明: 无
******************************************************************************/
int Tcl_AppInit(Tcl_Interp *interp)
{ 
    /* 时间相关命令 */
    Tcl_CreateObjCommand(interp, "sleep", CmdSleep,
                        NULL, NULL);
    Tcl_CreateCommand(interp, "exec", CmdExec,
                        NULL, NULL);
    Tcl_CreateCommand(interp, "split", CmdSplit,
                        NULL, NULL);
    Tcl_CreateCommand(interp, "rand", CmdRand,
                        NULL, NULL);
                        
    /* 窗口相关命令 */
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
                        
    /* 按键相关命令 */
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
    
    /* 鼠标相关命令 */
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
函数名称: ExecuteScriptAndFile
功能说明: 执行外部的tcl文件
输入参数: 无
输出参数: 无
返 回 值: 无
说    明: 无
******************************************************************************/
int ExecuteScriptAndFile(const char *script, const char *fileName)
{
    Tcl_Interp* interp = Tcl_CreateInterp();
    
    /* 初始化 */
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

