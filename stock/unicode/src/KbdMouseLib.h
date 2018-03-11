/*******************************************************************************
��Ȩ����, 2010~2013, ������ (zeng_hr@163.com)
�ļ���: KBDMOUSELIB.H
����: ������ (zeng_hr@163.com)  ����: 2010��4��13��
��������: 
�����б�: 
  1. ...: 
  
�޸���ʷ: 
  ����: ������ (zeng_hr@163.com)  ����: 2010��4��13��
  ------------------------------------------------------------------------------
  ��ʼ�汾
*******************************************************************************/
#ifndef _KBD_MOUSE_LIB_H_
#define _KBD_MOUSE_LIB_H_

// �˴�keycode����Ϊ���ư���, ��������ƴ���δ����, ��ͬ��SendKeyPress()����
// ����: VK_RETURN, VK_LEFT, VK_RIGHT��
extern void SendKeyDown(HWND hwnd, int keycode);
// �˴�keycode����Ϊ���ư���
// ����: VK_RETURN, VK_LEFT, VK_RIGHT��
extern void SendKeyUp(HWND hwnd, int keycode);
// �˴�keycode����Ϊ���ư���
// ����: VK_RETURN, VK_LEFT, VK_RIGHT��
extern void SendKeyPress(HWND hwnd, int keycode);
// �˴�keystr���ַ���
extern void SendKeyString(HWND hwnd, const char *keystr);
// ����, ��Ҫʹ���������ڴ��ڻ״̬
extern void KeyDown(int keycode);
extern void KeyUp(int keycode);
extern void KeyPress(int keycode);

#define MOUSE_LEFT   0
#define MOUSE_RIGHT  1
#define MOUSE_MIDDLE 2

// �����Ϣ����
extern void SendMouseMove(HWND hwnd, int x, int y);
extern int SendMouseDown(HWND hwnd, int lrm, int x, int y);
extern int SendMouseUp(HWND hwnd, int lrm);
extern int SendMouseClick(HWND hwnd, int lrm, int x, int y);
extern int SendMouseDblClick(HWND hwnd, int lrm, int x, int y);
// ���ָ���ƶ������ڵ����λ��
extern void MouseMoveR(HWND hwnd, int x, int y);
// ���ָ���ƶ�������λ��
extern void MouseMove(int x, int y);
// ��������, ����
extern int MouseDown(int lrm);
extern int MouseUp(int lrm);
// ��������, ˫��
extern int MouseClick(int lrm);
extern int MouseDblClick(int lrm);

#endif
