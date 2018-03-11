/******************************************************************************
��Ȩ����, 2010~2013, ������ (zeng_hr@163.com)
�ļ���: STOCKKEEPALIVE.CPP
����: ������ (zeng_hr@163.com)  ����: 2010��5��6��
��������: 
�����б�: 
  1. ...: 
  
�޸���ʷ: 
  ����: ������ (zeng_hr@163.com)  ����: 2010��5��6��
  -----------------------------------------------------------------------------
  ��ʼ�汾
******************************************************************************/
#include <stdio.h>

#include "osp_globals.h"
#include "Stock.h"
#include "Log.h"
#include "TclExtCmd.h"

/******************************************************************************
��������: KeepAliveWatch
����˵��: ��
�������: ��
�������: ��
�� �� ֵ: ��
˵    ��: ��
******************************************************************************/
OSP_S32 KeepAliveWatch(HWND v_hwnd)
{
    OSP_S8 szCmd[STOCK_CMD_LEN];

    LogTraceEvent(g_Log, "Keep Alive watch window. [%d]\n", v_hwnd);

    _snprintf(szCmd, STOCK_CMD_LEN, "set hwnd %d\n", v_hwnd);

    LogTraceEvent(g_Log, "Execute cmd. [%s, %s]\n", szCmd, STOCK_FLUSH_WATCH);
    
    if (0 != ExecuteScriptAndFile(szCmd, STOCK_FLUSH_WATCH))
    {
        LogTraceError(g_Log, "Execute cmd failed. [%s, %s]\n",
            szCmd, STOCK_FLUSH_WATCH);
        return -ERR_WATCH_EXEC_KEEP_ALIVE_WATCH_SCRIPT;
    }
    
    return SUCCESS;
} /* End of KeepAliveWatch */

/******************************************************************************
��������: KeepAliveChip
����˵��: ��
�������: ��
�������: ��
�� �� ֵ: ��
˵    ��: ��
******************************************************************************/
OSP_S32 KeepAliveChip(HWND v_hwnd)
{
    OSP_S8 szCmd[STOCK_CMD_LEN];
        
    LogTraceEvent(g_Log, "Keep Alive chip window. [%d]\n", v_hwnd);

    _snprintf(szCmd, STOCK_CMD_LEN, "set hwnd %d\n", v_hwnd);

    LogTraceEvent(g_Log, "Execute cmd. [%s, %s]\n", szCmd, STOCK_FLUSH_CHIP);
    
    if (0 != ExecuteScriptAndFile(szCmd, STOCK_FLUSH_CHIP))
    {
        LogTraceError(g_Log, "Execute cmd failed. [%s, %s]\n",
            szCmd, STOCK_FLUSH_CHIP);
        return -ERR_WATCH_EXEC_KEEP_ALIVE_CHIP_SCRIPT;
    }
    
    return SUCCESS;
} /* End of KeepAliveChip */

