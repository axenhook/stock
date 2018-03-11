/*******************************************************************************
版权所有, 2010~2013, 曾华荣 (zeng_hr@163.com)
文件名: KBDMOUSELIB.H
作者: 曾华荣 (zeng_hr@163.com)  日期: 2010年4月13日
功能描述: 
函数列表: 
  1. ...: 
  
修改历史: 
  作者: 曾华荣 (zeng_hr@163.com)  日期: 2010年4月13日
  ------------------------------------------------------------------------------
  初始版本
*******************************************************************************/
#ifndef _KBD_MOUSE_LIB_H_
#define _KBD_MOUSE_LIB_H_

// 此处keycode必须为控制按键, 如果被控制窗口未激活, 等同于SendKeyPress()功能
// 例如: VK_RETURN, VK_LEFT, VK_RIGHT等
extern void SendKeyDown(HWND hwnd, int keycode);
// 此处keycode必须为控制按键
// 例如: VK_RETURN, VK_LEFT, VK_RIGHT等
extern void SendKeyUp(HWND hwnd, int keycode);
// 此处keycode必须为控制按键
// 例如: VK_RETURN, VK_LEFT, VK_RIGHT等
extern void SendKeyPress(HWND hwnd, int keycode);
// 此处keystr是字符串
extern void SendKeyString(HWND hwnd, const char *keystr);
// 按键, 需要使被操作窗口处于活动状态
extern void KeyDown(int keycode);
extern void KeyUp(int keycode);
extern void KeyPress(int keycode);

#define MOUSE_LEFT   0
#define MOUSE_RIGHT  1
#define MOUSE_MIDDLE 2

// 鼠标消息操作
extern void SendMouseMove(HWND hwnd, int x, int y);
extern int SendMouseDown(HWND hwnd, int lrm, int x, int y);
extern int SendMouseUp(HWND hwnd, int lrm);
extern int SendMouseClick(HWND hwnd, int lrm, int x, int y);
extern int SendMouseDblClick(HWND hwnd, int lrm, int x, int y);
// 鼠标指针移动到窗口的相对位置
extern void MouseMoveR(HWND hwnd, int x, int y);
// 鼠标指针移动到绝对位置
extern void MouseMove(int x, int y);
// 鼠标键按下, 弹起
extern int MouseDown(int lrm);
extern int MouseUp(int lrm);
// 鼠标键单击, 双击
extern int MouseClick(int lrm);
extern int MouseDblClick(int lrm);

#endif
