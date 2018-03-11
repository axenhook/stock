/*******************************************************************************
��Ȩ����, 2010~2013, ������ (zeng_hr@163.com)
�ļ���: KBDMOUSELIB.CPP
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

#include "KbdMouseLib.h"

/* �˴�keycode����Ϊ���ư���, ��������ƴ���δ����, ��ͬ��SendKeyPress()���� */
/* ����: VK_RETURN, VK_LEFT, VK_RIGHT�� */
void SendKeyDown(HWND hwnd, int keycode)
{
    SendMessage(hwnd, WM_KEYDOWN, keycode, 0);
}

/* �˴�keycode����Ϊ���ư��� */
/* ����: VK_RETURN, VK_LEFT, VK_RIGHT�� */
void SendKeyUp(HWND hwnd, int keycode)
{
    SendMessage(hwnd, WM_KEYUP, keycode, 0);
}

/* �˴�keycode����Ϊ���ư��� */
/* ����: VK_RETURN, VK_LEFT, VK_RIGHT�� */
void SendKeyPress(HWND hwnd, int keycode)
{
    SendKeyDown(hwnd, keycode);
    SendKeyUp(hwnd, keycode);
}

/* �˴�keystr���ַ��� */
void SendKeyString(HWND hwnd, const char *keystr)
{
    while (*keystr != 0)
    {
        SendMessage(hwnd, WM_CHAR, *keystr, 0);
        keystr++;
    }
}

/* ����, ��Ҫʹ���������ڴ��ڻ״̬ */
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

/* ������ */
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

/* �ƶ������ڵ����λ�� */
void MouseMoveR(HWND hwnd, int x, int y)
{
    RECT rect;

    GetWindowRect(hwnd, &rect);
    /*printf("x = %d, y = %d\n", rect.left, rect.top); */

    /*mouse_event(MOUSEEVENTF_ABSOLUTE | MOUSEEVENTF_MOVE, */
    /*    rect.left + x, rect.top + y, 0, 0); // ���ɹ� */
    SetCursorPos(rect.left + x, rect.top + y);
}

/* �ƶ�������λ�� */
void MouseMove(int x, int y)
{
    /*mouse_event(MOUSEEVENTF_ABSOLUTE | MOUSEEVENTF_MOVE, x, y, 0, 0); // ���ɹ� */
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
    hwnd = MyFindWind(NULL, NULL, L"��ͼ");
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
    MessageBox(NULL, str, L"��Ϣ�����", MB_OKCANCEL);
#endif

#if 0 /* ���԰�����Ϊģ��, Ŀǰ��ϰ�������ģ�ⲻ�ɹ� */
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

        /* ģ�ⰴ��ctrl+a�����, Ŀǰ��������ɹ� */
        KeyDown(VK_CONTROL); /* ����ctrl�� */
        Sleep(500);
        KeyDown('a'); /* ����a */
        Sleep(500);
        KeyUp('a'); /* �ɿ�a */
        Sleep(500);
        KeyUp(VK_CONTROL);  /* �ɿ�ctrl�� */
        Sleep(500);
    }
#endif

#if 0 /* ���Է��Ͱ�����Ϣ, Ŀǰ��ϰ�����Ϣ�����ɹ� */
    /* ��һ��TEdit֮��Ŀؼ���ʵ�� */
    hwnd = (HWND)5047570;
    SendKeyString(hwnd, "1234567890");

    SendKeyPress(hwnd, VK_RETURN);
    SendKeyPress(hwnd, VK_RETURN);
    SendKeyPress(hwnd, VK_RETURN);
    SendKeyString(hwnd, "0987654321");

    /* ģ�ⰴ��ctrl+a�����, Ŀǰ��������ɹ� */
    SetWindowActive(hwnd);
    Sleep(2000);
    SendKeyDown(hwnd, VK_LCONTROL); /* ����ctrl��, Ŀǰ���ֲ�ס */
    SendKeyPress(hwnd, 'a'); /* ����ctrl��, Ŀǰ���ֲ�ס */
    /*SendKeyString(hwnd, "a"); */
    SendKeyUp(hwnd, VK_LCONTROL);  /* �ɿ�ctrl�� */
#endif

#if 1 /* ���Է��������Ϣ����̨���� */
    /* �򿪸���/��ͼ����, Ȼ��ץȡ��ͼ����, ��ѡ��Ǧ�ʹ��� */
    /* �ử������ͬ�ĵ������� */
    hwnd = (HWND)68614;
    /*SetWindowActive(hwnd); // �˾䲻��Ҫ��Ҳ���Ի����� */
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

#if 0 /* ����������λ���ƶ� */
    /* �򿪸���/��ͼ����, ��ѡ��Ǧ�ʹ��� */
    /* �ử������ͬ�ĵ������� */
    hwnd = MyFindWind(NULL, NULL, L"��ͼ");
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

#if 0 /* ����������λ���ƶ� */
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

    MessageBox(NULL, str, "��Ϣ�����", MB_OKCANCEL);
#endif

    system("pause");
}

#endif

