/*******************************************************************************
版权所有, 2010~2013, 曾华荣 (zeng_hr@163.com)
文件名: WNDLIB.H
作者: 曾华荣 (zeng_hr@163.com)  日期: 2010年4月13日
功能描述: 
函数列表: 
  1. ...: 
  
修改历史: 
  作者: 曾华荣 (zeng_hr@163.com)  日期: 2010年4月13日
  ------------------------------------------------------------------------------
  初始版本
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
