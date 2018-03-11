/*******************************************************************************
版权所有, 2010~2013, 曾华荣 (zeng_hr@163.com)
文件名: KBDMOUSELIB.CPP
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

#include "KbdMouseLib.h"

/* 此处keycode必须为控制按键, 如果被控制窗口未激活, 等同于SendKeyPress()功能 */
/* 例如: VK_RETURN, VK_LEFT, VK_RIGHT等 */
void SendKeyDown(HWND hwnd, int keycode)
{
    SendMessage(hwnd, WM_KEYDOWN, keycode, 0);
}

/* 此处keycode必须为控制按键 */
/* 例如: VK_RETURN, VK_LEFT, VK_RIGHT等 */
void SendKeyUp(HWND hwnd, int keycode)
{
    SendMessage(hwnd, WM_KEYUP, keycode, 0);
}

/* 此处keycode必须为控制按键 */
/* 例如: VK_RETURN, VK_LEFT, VK_RIGHT等 */
void SendKeyPress(HWND hwnd, int keycode)
{
    SendKeyDown(hwnd, keycode);
    SendKeyUp(hwnd, keycode);
}

/* 此处keystr是字符串 */
void SendKeyString(HWND hwnd, const char *keystr)
{
    while (*keystr != 0)
    {
        SendMessage(hwnd, WM_CHAR, *keystr, 0);
        keystr++;
    }
}

/* 按键, 需要使被操作窗口处于活动状态 */
void KeyDown(int keycode)
{
    keybd_event(keycode, 0, 0, 0);
}

void KeyUp(int keycode)
{
    keybd_event(keycode, 0, KEYEVENTF_KEYUP, 0);
}

void KeyPress(int keycode)
{
    KeyDown(keycode);
    KeyUp(keycode);
}

/* 鼠标操作 */
void SendMouseMove(HWND hwnd, int x, int y)
{
    SendMessage(hwnd, WM_MOUSEMOVE, 0, MAKELPARAM(x, y));
}

int SendMouseDown(HWND hwnd, int lrm, int x, int y)
{
    switch (lrm)
    {
        case MOUSE_LEFT:
        {
            SendMessage(hwnd, WM_LBUTTONDOWN, 0, MAKELPARAM(x, y));
            break;
        }

        case MOUSE_RIGHT:
        {
            SendMessage(hwnd, WM_RBUTTONDOWN, 0, MAKELPARAM(x, y));
            break;
        }

        case MOUSE_MIDDLE:
        {
            SendMessage(hwnd, WM_MBUTTONDOWN, 0, MAKELPARAM(x, y));
            break;
        }

        default:
        {
            return -1;
            break;
        }
    }

    return 0;
}

int SendMouseUp(HWND hwnd, int lrm)
{
    switch (lrm)
    {
        case MOUSE_LEFT:
        {
            SendMessage(hwnd, WM_LBUTTONUP, 0, 0);
            break;
        }

        case MOUSE_RIGHT:
        {
            SendMessage(hwnd, WM_RBUTTONUP, 0, 0);
            break;
        }

        case MOUSE_MIDDLE:
        {
            SendMessage(hwnd, WM_MBUTTONUP, 0, 0);
            break;
        }

        default:
        {
            return -1;
            break;
        }
    }

    return 0;
}

int SendMouseClick(HWND hwnd, int lrm, int x, int y)
{
    switch (lrm)
    {
        case MOUSE_LEFT:
        {
            SendMessage(hwnd, WM_LBUTTONDOWN, 0, MAKELPARAM(x, y));
            SendMessage(hwnd, WM_LBUTTONUP, 0, MAKELPARAM(x, y));
            break;
        }

        case MOUSE_RIGHT:
        {
            SendMessage(hwnd, WM_RBUTTONDOWN, 0, MAKELPARAM(x, y));
            SendMessage(hwnd, WM_RBUTTONUP, 0, MAKELPARAM(x, y));
            break;
        }

        case MOUSE_MIDDLE:
        {
            SendMessage(hwnd, WM_MBUTTONDOWN, 0, MAKELPARAM(x, y));
            SendMessage(hwnd, WM_MBUTTONUP, 0, MAKELPARAM(x, y));
            break;
        }

        default:
        {
            return -1;
            break;
        }
    }

    return 0;
}

int SendMouseDblClick(HWND hwnd, int lrm, int x, int y)
{
    switch (lrm)
    {
        case MOUSE_LEFT:
        {
            SendMessage(hwnd, WM_LBUTTONDBLCLK, 0, MAKELPARAM(x, y));
            break;
        }

        case MOUSE_RIGHT:
        {
            SendMessage(hwnd, WM_RBUTTONDBLCLK, 0, MAKELPARAM(x, y));
            break;
        }

        case MOUSE_MIDDLE:
        {
            SendMessage(hwnd, WM_MBUTTONDBLCLK, 0, MAKELPARAM(x, y));
            break;
        }

        default:
        {
            return -1;
            break;
        }
    }

    return 0;
}

/* 移动到窗口的相对位置 */
void MouseMoveR(HWND hwnd, int x, int y)
{
    RECT rect;

    GetWindowRect(hwnd, &rect);
    /*printf("x = %d, y = %d\n", rect.left, rect.top); */

    /*mouse_event(MOUSEEVENTF_ABSOLUTE | MOUSEEVENTF_MOVE, */
    /*    rect.left + x, rect.top + y, 0, 0); // 不成功 */
    SetCursorPos(rect.left + x, rect.top + y);
}

/* 移动到绝对位置 */
void MouseMove(int x, int y)
{
    /*mouse_event(MOUSEEVENTF_ABSOLUTE | MOUSEEVENTF_MOVE, x, y, 0, 0); // 不成功 */
    SetCursorPos(x, y);
}

int MouseDown(int lrm)
{
    switch (lrm)
    {
        case MOUSE_LEFT:
        {
            mouse_event(MOUSEEVENTF_LEFTDOWN, 0, 0, 0, 0);
            break;
        }

        case MOUSE_RIGHT:
        {
            mouse_event(MOUSEEVENTF_RIGHTDOWN, 0, 0, 0, 0);
            break;
        }

        case MOUSE_MIDDLE:
        {
            mouse_event(MOUSEEVENTF_MIDDLEDOWN, 0, 0, 0, 0);
            break;
        }

        default:
        {
            return -1;
            break;
        }
    }

    return 0;
}

int MouseUp(int lrm)
{
    switch (lrm)
    {
        case MOUSE_LEFT:
        {
            mouse_event(MOUSEEVENTF_LEFTUP, 0, 0, 0, 0);
            break;
        }

        case MOUSE_RIGHT:
        {
            mouse_event(MOUSEEVENTF_RIGHTUP, 0, 0, 0, 0);
            break;
        }

        case MOUSE_MIDDLE:
        {
            mouse_event(MOUSEEVENTF_MIDDLEUP, 0, 0, 0, 0);
            break;
        }

        default:
        {
            return -1;
            break;
        }
    }

    return 0;
}

int MouseClick(int lrm)
{
    switch (lrm)
    {
        case MOUSE_LEFT:
        {
            mouse_event(MOUSEEVENTF_LEFTDOWN | MOUSEEVENTF_LEFTUP, 0, 0, 0, 0);
            break;
        }

        case MOUSE_RIGHT:
        {
            mouse_event(MOUSEEVENTF_RIGHTDOWN | MOUSEEVENTF_RIGHTUP, 0, 0, 0, 0);
            break;
        }

        case MOUSE_MIDDLE:
        {
            mouse_event(MOUSEEVENTF_MIDDLEDOWN | MOUSEEVENTF_MIDDLEUP, 0, 0, 0, 0);
            break;
        }

        default:
        {
            return -1;
            break;
        }
    }

    return 0;
}

int MouseDblClick(int lrm)
{
    int ret;

    ret = MouseClick(lrm);
    if (ret)
    {
        return ret;
    }

    return MouseClick(lrm);
}

#if 1

#include "WndLib.h"

void DebugKbdMouseLib(void)
{
    CString str;
    HWND hwnd;

#if 0
    hwnd = MyFindWind(NULL, NULL, L"画图");
    if (hwnd != NULL)
    {
        WCHAR caption[MAX_LENGTH];
        WCHAR exePath[MAX_LENGTH], modulePath[MAX_LENGTH];
        GetWindowText(hwnd, caption, MAX_LENGTH);
        GetExePath(hwnd, exePath, sizeof(exePath));
        GetModulePath(hwnd, modulePath, sizeof(modulePath));
        str.Format(L"hwnd = %d, caption = %s, exepath = %s, modulepath = %s",
            hwnd, caption, exePath, modulePath);
    }
    else
    {
        str.Format(L"%s", L"Can't find the windows");
    }
    MessageBox(NULL, str, L"消息框标题", MB_OKCANCEL);
#endif

#if 0 /* 测试按键行为模拟, 目前组合按键按键模拟不成功 */
    hwnd = MyFindWind(NULL, NULL, L"Notepad2");
    if (hwnd != NULL)
    {
        SetWindowActive(hwnd);
        KeyPress('1');
        Sleep(100);
        KeyPress('2');
        Sleep(100);
        KeyPress('3');
        Sleep(100);
        KeyPress('4');
        Sleep(100);
        KeyPress('5');
        Sleep(100);
        KeyPress(VK_RETURN);
        Sleep(1000);

        /* 模拟按下ctrl+a的情况, 目前这个发不成功 */
        KeyDown(VK_CONTROL); /* 按下ctrl键 */
        Sleep(500);
        KeyDown('a'); /* 按下a */
        Sleep(500);
        KeyUp('a'); /* 松开a */
        Sleep(500);
        KeyUp(VK_CONTROL);  /* 松开ctrl键 */
        Sleep(500);
    }
#endif

#if 0 /* 测试发送按键消息, 目前组合按键消息发不成功 */
    /* 找一个TEdit之类的控件做实验 */
    hwnd = (HWND)5047570;
    SendKeyString(hwnd, "1234567890");

    SendKeyPress(hwnd, VK_RETURN);
    SendKeyPress(hwnd, VK_RETURN);
    SendKeyPress(hwnd, VK_RETURN);
    SendKeyString(hwnd, "0987654321");

    /* 模拟按下ctrl+a的情况, 目前这个发不成功 */
    SetWindowActive(hwnd);
    Sleep(2000);
    SendKeyDown(hwnd, VK_LCONTROL); /* 按下ctrl键, 目前保持不住 */
    SendKeyPress(hwnd, 'a'); /* 按下ctrl键, 目前保持不住 */
    /*SendKeyString(hwnd, "a"); */
    SendKeyUp(hwnd, VK_LCONTROL);  /* 松开ctrl键 */
#endif

#if 1 /* 测试发送鼠标信息给后台程序 */
    /* 打开附件/画图工具, 然后抓取画图框句柄, 并选择铅笔工具 */
    /* 会画出两个同心的正方形 */
    hwnd = (HWND)68614;
    /*SetWindowActive(hwnd); // 此句不需要打开也可以划出来 */
    SendMouseDown(hwnd, MOUSE_LEFT, 100, 100);
    Sleep(100);
    SendMouseMove(hwnd, 100, 400);
    Sleep(100);
    SendMouseMove(hwnd, 400, 400);
    Sleep(100);
    SendMouseMove(hwnd, 400, 100);
    Sleep(100);
    SendMouseMove(hwnd, 100, 100);
    Sleep(100);
    SendMouseUp(hwnd, MOUSE_LEFT);
    Sleep(100);

    SendMouseDown(hwnd, MOUSE_LEFT, 150, 150);
    Sleep(100);
    SendMouseMove(hwnd, 150, 350);
    Sleep(100);
    SendMouseMove(hwnd, 350, 350);
    Sleep(100);
    SendMouseMove(hwnd, 350, 150);
    Sleep(100);
    SendMouseMove(hwnd, 150, 150);
    Sleep(100);
    SendMouseUp(hwnd, MOUSE_LEFT);
    Sleep(100);

#endif

#if 0 /* 测试鼠标相对位置移动 */
    /* 打开附件/画图工具, 并选择铅笔工具 */
    /* 会画出两个同心的正方形 */
    hwnd = MyFindWind(NULL, NULL, L"画图");
    if (hwnd != NULL)
    {
        SetWindowActive(hwnd);
        MouseMoveR(hwnd, 100, 100);
        MouseDown(MOUSE_LEFT);
        Sleep(100);
        MouseMoveR(hwnd, 100, 400);
        Sleep(100);
        MouseMoveR(hwnd, 400, 400);
        Sleep(100);
        MouseMoveR(hwnd, 400, 100);
        Sleep(100);
        MouseMoveR(hwnd, 100, 100);
        Sleep(100);
        MouseUp(MOUSE_LEFT);
        Sleep(100);

        MouseMoveR(hwnd, 150, 150);
        MouseDown(MOUSE_LEFT);
        Sleep(100);
        MouseMoveR(hwnd, 150, 350);
        Sleep(100);
        MouseMoveR(hwnd, 350, 350);
        Sleep(100);
        MouseMoveR(hwnd, 350, 150);
        Sleep(100);
        MouseMoveR(hwnd, 150, 150);
        Sleep(100);
        MouseUp(MOUSE_LEFT);
        Sleep(100);
    }
#endif

#if 0 /* 测试鼠标绝对位置移动 */
    MouseMove(0, 0);
    Sleep(1000);
    MouseMove(800, 0);
    Sleep(1000);
    MouseMove(800, 600);
    Sleep(1000);
    MouseMove(0, 600);
    Sleep(1000);
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

    MessageBox(NULL, str, "消息框标题", MB_OKCANCEL);
#endif

    system("pause");
}

#endif

