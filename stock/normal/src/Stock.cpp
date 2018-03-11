/******************************************************************************
版权所有, 2010~2013, 曾华荣 (zeng_hr@163.com)
文件名: STOCK.CPP
作者: 曾华荣 (zeng_hr@163.com)  日期: 2010年5月3日
功能描述: 
函数列表: 
  1. ...: 
  
修改历史: 
  作者: 曾华荣 (zeng_hr@163.com)  日期: 2010年5月3日
  -----------------------------------------------------------------------------
  初始版本
******************************************************************************/

#include "osp_globals.h"

#include "Stock.h"
#include "Log.h"
#include "WndLib.h"
#include "inifile.h"

STOCK_CONFIG_S g_stCfg;

/*******************************************************************************
函数名称: ConfigInit
功能说明: 无
输入参数: 无
输出参数: 无
返 回 值:
    >=0: 成功
    < 0: 错误代码
说    明: 无
*******************************************************************************/
OSP_S32 ConfigInit(const OSP_S8 *v_pszConfigFile)
{
    memset(&g_stCfg, 0, sizeof(STOCK_CONFIG_S));

    /* 读取预警主窗口名称 */
    if (0 != IniReadString(STOCK_CONFIG_FILE, "SoftWare",
        "WATCH_MAIN_WND", g_stCfg.szWatchMainWndName, STOCK_MAX_LEN, NULL))
    {
        LogTraceError(g_Log, "Read config file failed. [%s, %s, %s]\n",
            STOCK_CONFIG_FILE, "SoftWare", "WATCH_MAIN_WND");
        return -ERR_STOCK_READ_CONFIG;
    }
    
    LogTraceEvent(g_Log, "Read config file success. [%s, %s, %s, %s]\n",
        STOCK_CONFIG_FILE, "SoftWare", "WATCH_MAIN_WND",
        g_stCfg.szWatchMainWndName);

    /* 读取预警信息窗口名称 */
    if (0 != IniReadString(STOCK_CONFIG_FILE, "SoftWare",
        "WATCH_INFO_WND", g_stCfg.szWatchInfoWndName, STOCK_MAX_LEN, NULL))
    {
        LogTraceError(g_Log, "Read config file failed. [%s, %s, %s]\n",
            STOCK_CONFIG_FILE, "SoftWare", "WATCH_INFO_WND");
        return -ERR_STOCK_READ_CONFIG;
    }
    
    LogTraceEvent(g_Log, "Read config file success. [%s, %s, %s, %s]\n",
        STOCK_CONFIG_FILE, "SoftWare", "WATCH_INFO_WND",
        g_stCfg.szWatchInfoWndName);

    /* 读取预警信息窗口控件名称 */
    if (0 != IniReadString(STOCK_CONFIG_FILE, "SoftWare",
        "WATCH_INFO_WND_CLASS", g_stCfg.szWatchInfoWndClass, STOCK_MAX_LEN, NULL))
    {
        LogTraceError(g_Log, "Read config file failed. [%s, %s, %s]\n",
            STOCK_CONFIG_FILE, "SoftWare", "WATCH_INFO_WND_CLASS");
        return -ERR_STOCK_READ_CONFIG;
    }
    
    LogTraceEvent(g_Log, "Read config file success. [%s, %s, %s, %s]\n",
        STOCK_CONFIG_FILE, "SoftWare", "WATCH_INFO_WND_CLASS",
        g_stCfg.szWatchInfoWndClass);

    /* 读取下单主窗口名称 */
    if (0 != IniReadString(STOCK_CONFIG_FILE, "SoftWare",
        "CHIP_MAIN_WND", g_stCfg.szChipMainWndName, STOCK_MAX_LEN, NULL))
    {
        LogTraceError(g_Log, "Read config file failed. [%s, %s, %s]\n",
            STOCK_CONFIG_FILE, "SoftWare", "CHIP_MAIN_WND");
        return -ERR_STOCK_READ_CONFIG;
    }
    
    LogTraceEvent(g_Log, "Read config file success. [%s, %s, %s, %s]\n",
        STOCK_CONFIG_FILE, "SoftWare", "CHIP_MAIN_WND",
        g_stCfg.szChipMainWndName);

    /* 读取预警方式 */
    g_stCfg.iWatchMethod = IniReadInt(STOCK_CONFIG_FILE, "SoftWare",
        "WATCH_METHOD", WATCH_NAME);
    LogTraceEvent(g_Log, "Read config file success. [%s, %s, %s, %d]\n",
        STOCK_CONFIG_FILE, "SoftWare", "WATCH_METHOD",
        g_stCfg.iWatchMethod);

    /* 读取刷新预警软件的时间间隔 */
    g_stCfg.iWatchFlushTime = IniReadInt(STOCK_CONFIG_FILE, "SoftWare",
        "WATCH_FLUSH_TIME", 5000);
    LogTraceEvent(g_Log, "Read config file success. [%s, %s, %s, %d]\n",
        STOCK_CONFIG_FILE, "SoftWare", "WATCH_FLUSH_TIME",
        g_stCfg.iWatchFlushTime);

    /* 读取刷新下单软件的时间间隔 */
    g_stCfg.iChipFlushTime = IniReadInt(STOCK_CONFIG_FILE, "SoftWare",
        "CHIP_FLUSH_TIME", 5000);
    LogTraceEvent(g_Log, "Read config file success. [%s, %s, %s, %d]\n",
        STOCK_CONFIG_FILE, "SoftWare", "CHIP_FLUSH_TIME",
        g_stCfg.iChipFlushTime);

    /* 读取软件检查的时间间隔 */
    g_stCfg.iCheckWndTime = IniReadInt(STOCK_CONFIG_FILE, "SoftWare",
        "CHECK_WND_TIME", 5000);
    LogTraceEvent(g_Log, "Read config file success. [%s, %s, %s, %d]\n",
        STOCK_CONFIG_FILE, "SoftWare", "CHECK_WND_TIME",
        g_stCfg.iCheckWndTime);

    /* 读取是否启动交易 */
    g_stCfg.iChipTimes = IniReadInt(STOCK_CONFIG_FILE, "SoftWare",
        "CHIP_TIMES", 0);
    LogTraceEvent(g_Log, "Read config file success. [%s, %s, %s, %d]\n",
        STOCK_CONFIG_FILE, "SoftWare", "CHIP_TIMES",
        g_stCfg.iChipTimes);

    return SUCCESS;
} /* End of ConfigInit */

/*******************************************************************************
函数名称: CheckWindows
功能说明: 检查各个窗口是否ok
输入参数: 无
输出参数: 无
返 回 值:
    >=0: 成功
    < 0: 错误代码
说    明: 无
*******************************************************************************/
OSP_S32 CheckWindows(OSP_VOID)
{
    HWND hwnd, hwnd2;

    /* 获取预警系统相关信息 */
    // 获取预警主窗口
    hwnd = WindowFind(NULL, NULL, g_stCfg.szWatchMainWndName);
    if (NULL == hwnd)
    {
        LogTraceError(g_Log, "Can not find the watch main window. [%s]\n",
            g_stCfg.szWatchMainWndName);
        return -ERR_STOCK_FIND_WATCH_MAIN_WND;
    }

    LogTraceDebug(g_Log, "The watch main window found. [%s, %d]\n",
        g_stCfg.szWatchMainWndName, hwnd);
    
    if (g_stCfg.hwndWatchMain != hwnd)
    {
        LogTraceEvent(g_Log, "The watch main window is changed. [%s, %d, %d]\n",
            g_stCfg.szWatchMainWndName, hwnd, g_stCfg.hwndWatchMain);
        g_stCfg.hwndWatchMain = hwnd;
    }

    // 获取预警信息捕捉窗口
    hwnd = WindowFind(NULL, NULL, g_stCfg.szWatchInfoWndName);
    if (NULL == hwnd)
    {
        LogTraceError(g_Log, "Can not find the watch info window. [%s]\n",
            g_stCfg.szWatchInfoWndName);
        return -ERR_STOCK_FIND_WATCH_INFO_WND;
    }

    hwnd2 = WindowFind(hwnd, g_stCfg.szWatchInfoWndClass, NULL);
    if (NULL == hwnd2)
    {
        LogTraceError(g_Log, "Can not find the watch info window. [%s, %d]\n",
            g_stCfg.szWatchInfoWndClass, hwnd);
        return -ERR_STOCK_FIND_WATCH_INFO_CHILD_WND;
    }
    
    LogTraceDebug(g_Log, "The watch info window found. [%s, %s, %d]\n",
        g_stCfg.szWatchInfoWndClass, g_stCfg.szWatchInfoWndName,
        hwnd2);
    
    if (g_stCfg.hwndWatchInfo != hwnd2)
    {
        LogTraceEvent(g_Log, "The watch info window changed. [%s, %s, %d, %d]\n",
            g_stCfg.szWatchInfoWndClass, g_stCfg.szWatchInfoWndName,
            hwnd2, g_stCfg.hwndWatchInfo);
        g_stCfg.hwndWatchInfo = hwnd2;

        /* 重新初始化预警信息 */
        if (SUCCESS != WatchInit())
        {
            LogTraceError(g_Log, "Watch init failed.\n");
            return -ERR_STOCK_INIT_WATCH;
        }
        
        LogTraceEvent(g_Log, "Initialize the watch info ok.\n");
    }
    
    /* 获取下单系统相关信息 */
    hwnd = WindowFind(NULL, NULL, g_stCfg.szChipMainWndName);
    if (NULL == hwnd)
    {
        LogTraceError(g_Log, "Can not find the chip main window. [%s]\n",
            g_stCfg.szChipMainWndName);
        return -ERR_STOCK_FIND_CHIP_MAIN_WND;
    }
    
    LogTraceDebug(g_Log, "The chip main window found. [%s, %d]\n",
        g_stCfg.szChipMainWndName, hwnd);
    
    if (g_stCfg.hwndChipMain != hwnd)
    {
        LogTraceEvent(g_Log, "The chip main window changed. [%s, %d, %d]\n",
            g_stCfg.szChipMainWndName, hwnd, g_stCfg.hwndChipMain);
        g_stCfg.hwndChipMain = hwnd;
    }

    return SUCCESS;
} /* End of CheckWindows */

/*******************************************************************************
函数名称: StockInit
功能说明: 无
输入参数: 无
输出参数: 无
返 回 值: 无
说    明: 无
*******************************************************************************/
OSP_S32 StockInit(OSP_VOID)
{
    if (SUCCESS != ConfigInit(STOCK_CONFIG_FILE))
    {
        LogTraceError(g_Log, "Config init failed. [%s]\n", STOCK_CONFIG_FILE);
        return -ERR_STOCK_INIT_CONFIG;
    }
    
    LogTraceEvent(g_Log, "Config init success. [%s]\n", STOCK_CONFIG_FILE);

    if (SUCCESS != TranslateInit(STOCK_CODE_TABLE))
    {
        LogTraceError(g_Log, "Tranlsate init failed. [%s]\n", STOCK_CODE_TABLE);
        return -ERR_STOCK_INIT_TRANSLATE;
    }

    LogTraceEvent(g_Log, "Tranlsate init success. [%s]\n", STOCK_CODE_TABLE);
    
    return SUCCESS;
} /* End of StockInit */

/*******************************************************************************
函数名称: StockEnd
功能说明: 无
输入参数: 无
输出参数: 无
返 回 值: 无
说    明: 无
*******************************************************************************/
OSP_VOID StockEnd(OSP_VOID)
{
    WatchEnd();
    TranslateEnd();
} /* End of StockEnd */

/*******************************************************************************
函数名称: StockTranslate
功能说明: 无
输入参数: 无
输出参数: 无
返 回 值:
    >=0: 成功
    < 0: 错误代码
说    明: 无
*******************************************************************************/
static OSP_S32 StockTranslate(STOCK_INFO_S * v_pstSI)
{
    if (WATCH_CODE == g_stCfg.iWatchMethod)
    { /* 从预警窗口仅仅获取到了股票代码, 将其转换出股票名称 */
        if (SUCCESS != TranslateCode(v_pstSI->szCode, v_pstSI->szName, STOCK_NAME_LEN))
        {
            LogTraceError(g_Log, "Translate stock code failed. [%s]\n",
                v_pstSI->szCode);
            return -ERR_STOCK_TRANSLATE_CODE;
        }

        LogTraceEvent(g_Log, "Translate stock code success. [%s, %s]\n",
            v_pstSI->szCode, v_pstSI->szName);
    }
    else if (WATCH_NAME == g_stCfg.iWatchMethod)
    { /* 从预警窗口仅仅获取到了股票名称, 将其转换出股票代码 */
        if (SUCCESS != TranslateName(v_pstSI->szName, v_pstSI->szCode, STOCK_CODE_LEN))
        {
            LogTraceError(g_Log, "Translate stock name failed. [%s]\n",
                v_pstSI->szName);
            return -ERR_STOCK_TRANSLATE_NAME;
        }

        LogTraceEvent(g_Log, "Translate stock name success. [%s, %s]\n",
            v_pstSI->szName, v_pstSI->szCode);
    }
    else
    { /* 既获取到了名称, 又获取到了代码, 就不用处理 */
    
    }

    return SUCCESS;
} /* End of StockTranslate */

/*******************************************************************************
函数名称: StockChip
功能说明: 无
输入参数: 无
输出参数: 无
返 回 值:
    >=0: 成功
    < 0: 错误代码
说    明: 无
*******************************************************************************/
OSP_S32 StockChip(STOCK_INFO_S * v_pstSI)
{
    if (CHIP_SELL == v_pstSI->iBuy)
    {
        if (SUCCESS != ChipSell(v_pstSI, g_stCfg.hwndChipMain))
        {
            LogTraceError(g_Log, "Sell chips failed. [%s, %s]\n",
                v_pstSI->szCode, v_pstSI->szName);
            return -ERR_STOCK_SELL_CHIPS;
        }

        LogTraceEvent(g_Log, "Sell chips success. [%s, %s]\n",
            v_pstSI->szCode, v_pstSI->szName);
    }
    else if (CHIP_BUY == v_pstSI->iBuy)
    {
        if (SUCCESS != ChipBuy(v_pstSI, g_stCfg.hwndChipMain))
        {
            LogTraceError(g_Log, "Buy chips failed. [%s, %s]\n",
                v_pstSI->szCode, v_pstSI->szName);
            return -ERR_STOCK_BUY_CHIPS;
        }
        
        LogTraceEvent(g_Log, "Buy chips success. [%s, %s]\n",
            v_pstSI->szCode, v_pstSI->szName);
    }
    else
    { /* 不是卖, 也不是买, 那是什么? */
        LogTraceEmerg(g_Log, "Failed. [%d, %s, %s, %d, %.2f]\n",
            v_pstSI->iBuy, v_pstSI->szCode, v_pstSI->szName, v_pstSI->iNum, v_pstSI->fPrice);
        return -ERR_STOCK_CHAOS_CHIPS;
    }

    return SUCCESS;
} /* End of StockChip */


/*******************************************************************************
函数名称: StockMain
功能说明: 无
输入参数: 无
输出参数: 无
返 回 值: 无
说    明: 无
*******************************************************************************/
OSP_S32 StockMain(OSP_VOID)
{
    STOCK_INFO_S stSI;
    OSP_S32 iRet = SUCCESS;
    OSP_U64 llWatchTick = 0;
    OSP_U64 llChipTick = 0;

    if (FALSE == EnableDebugPrivilege())
    {
        LogTraceError(g_Log, "Enable debug privilege failed.\n");
        return -ERR_STOCK_ENABLE_DEBUG_PRIVILEGE;
    }

    LogTraceEvent(g_Log, "Enable debug privilege success.\n");
    
    /* 初始化系统 */
    if (0 != StockInit())
    {
        LogTraceError(g_Log, "Stock system init failed.\n");
        return -ERR_STOCK_INIT_FAILED;
    }
    
    LogTraceEvent(g_Log, "Stock system init success.\n");
    
    while (1)
    {
        /* 检查窗口是否有变化 */
        if (SUCCESS != CheckWindows())
        {
            KeepAliveWatch(NULL);
            KeepAliveChip(NULL);

            Sleep(2000);
            
            LogTraceError(g_Log, "The are some windows not exist.\n");
            Sleep(g_stCfg.iCheckWndTime);

            continue;
        }
        
        /* 从预警窗口抓取预警信息 */
        OSP_S32 iRet = WatchGetResult(&stSI, g_stCfg.hwndWatchInfo);
        if (iRet > SUCCESS)
        { /* 已经处理完了 */
            Sleep(100);
            
            if ((GetTickCount() - llWatchTick) >= g_stCfg.iWatchFlushTime)
            { /* 保活预警系统 */
                KeepAliveWatch(g_stCfg.hwndWatchMain);
                llWatchTick = GetTickCount();
            }

            if ((GetTickCount() - llChipTick) >= g_stCfg.iChipFlushTime)
            { /* 保活下单系统 */
                KeepAliveChip(g_stCfg.hwndChipMain);
                llChipTick = GetTickCount();
            }
            
            continue;
        }
        else if (iRet < SUCCESS)
        { /* 出错了 */
            LogTraceError(g_Log, "Get info from watch window failed.\n");
            break;
        }

        /* 转换一下信息, 供下单系统使用 */
        if (SUCCESS != StockTranslate(&stSI))
        {
            LogTraceError(g_Log, "Translate record failed.\n");
            continue;
        }

        if (g_stCfg.iChipTimes == 0)
        { /* 跳过交易 */
            continue;
        }

        /* 下单: 买或者卖 */
		if (SUCCESS != StockChip(&stSI))
        {
            LogTraceError(g_Log, "Buy/sell chips failed.\n");
            continue;
        }

        /* 成功交易时, 需将下单次数减1 */
        if (g_stCfg.iChipTimes > 0)
        {
            g_stCfg.iChipTimes--;
        }
    }

    /* 结束系统 */
    StockEnd();

    return iRet;
} /* End of StockMain */


