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

extern long LVGetRowNum(HWND hwnd);
extern long LVGetColumnNum(HWND hwnd);
extern HWND MyFindWind(HWND hParent, WCHAR *className, WCHAR *caption);
extern int GetExePath(HWND hwnd, WCHAR *path, int len);
extern void GetModulePath(HWND hwnd, WCHAR *path, int len);
extern void SetWindowActive(HWND hwnd);
extern int LVGetItemContent(HWND hwnd, long row, long column,
                            char *content, int len);


#endif
