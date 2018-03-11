/******************************************************************************
版权所有, 2010~2013, 曾华荣 (zeng_hr@163.com)
文件名: STOCKKEEPALIVE.CPP
作者: 曾华荣 (zeng_hr@163.com)  日期: 2010年5月6日
功能描述: 
函数列表: 
  1. ...: 
  
修改历史: 
  作者: 曾华荣 (zeng_hr@163.com)  日期: 2010年5月6日
  -----------------------------------------------------------------------------
  初始版本
******************************************************************************/
#include <stdio.h>

#include "osp_globals.h"
#include "Stock.h"
#include "Log.h"
#include "TclExtCmd.h"

/******************************************************************************
函数名称: KeepAliveWatch
功能说明: 无
输入参数: 无
输出参数: 无
返 回 值: 无
说    明: 无
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
函数名称: KeepAliveChip
功能说明: 无
输入参数: 无
输出参数: 无
返 回 值: 无
说    明: 无
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

