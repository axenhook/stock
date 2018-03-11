/*******************************************************************************
��Ȩ����, 2010~2013, ������ (zeng_hr@163.com)
�ļ���: WNDLIB.H
����: ������ (zeng_hr@163.com)  ����: 2010��4��13��
��������: 
�����б�: 
  1. ...: 
  
�޸���ʷ: 
  ����: ������ (zeng_hr@163.com)  ����: 2010��4��13��
  ------------------------------------------------------------------------------
  ��ʼ�汾
*******************************************************************************/
#ifndef _WND_LIB_H_
#define _WND_LIB_H_

#define MAX_LENGTH  256

#define COLUMN_SIGNATURE  '#'

extern HWND WindowFind(HWND hParent, const char *className, const char *caption);
extern int GetExePath(HWND hwnd, char *path, int len);
extern void GetModulePath(HWND hwnd, char *path, int len);
extern void WindowActivate(HWND hwnd);
extern void WindowClose(HWND hwnd);
extern void WindowMinimize(HWND hwnd);
extern void WindowMaximize(HWND hwnd);
extern void WindowHide(HWND hwnd);
extern void WindowShow(HWND hwnd);
extern void WindowRestore(HWND hwnd);

extern long LVGetRowNum(HWND hwnd);
extern long LVGetColumnNum(HWND hwnd);
extern int LVGetRowContent(HWND hwnd, long row, char *content, int len);
extern int LVGetItemContent(HWND hwnd, long row, long column,
                            char *content, int len);

extern bool EnableDebugPrivilege(void);
extern LPSTR Convert(LPCSTR str, int sourceCodepage, int targetCodepage);

#endif
