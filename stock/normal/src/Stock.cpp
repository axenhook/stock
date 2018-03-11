/******************************************************************************
��Ȩ����, 2010~2013, ������ (zeng_hr@163.com)
�ļ���: STOCK.CPP
����: ������ (zeng_hr@163.com)  ����: 2010��5��3��
��������: 
�����б�: 
  1. ...: 
  
�޸���ʷ: 
  ����: ������ (zeng_hr@163.com)  ����: 2010��5��3��
  -----------------------------------------------------------------------------
  ��ʼ�汾
******************************************************************************/

#include "osp_globals.h"

#include "Stock.h"
#include "Log.h"
#include "WndLib.h"
#include "inifile.h"

STOCK_CONFIG_S g_stCfg;

/*******************************************************************************
��������: ConfigInit
����˵��: ��
�������: ��
�������: ��
�� �� ֵ:
    >=0: �ɹ�
    < 0: �������
˵    ��: ��
*******************************************************************************/
OSP_S32 ConfigInit(const OSP_S8 *v_pszConfigFile)
{
    memset(&g_stCfg, 0, sizeof(STOCK_CONFIG_S));

    /* ��ȡԤ������������ */
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

    /* ��ȡԤ����Ϣ�������� */
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

    /* ��ȡԤ����Ϣ���ڿؼ����� */
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

    /* ��ȡ�µ����������� */
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

    /* ��ȡԤ����ʽ */
    g_stCfg.iWatchMethod = IniReadInt(STOCK_CONFIG_FILE, "SoftWare",
        "WATCH_METHOD", WATCH_NAME);
    LogTraceEvent(g_Log, "Read config file success. [%s, %s, %s, %d]\n",
        STOCK_CONFIG_FILE, "SoftWare", "WATCH_METHOD",
        g_stCfg.iWatchMethod);

    /* ��ȡˢ��Ԥ�������ʱ���� */
    g_stCfg.iWatchFlushTime = IniReadInt(STOCK_CONFIG_FILE, "SoftWare",
        "WATCH_FLUSH_TIME", 5000);
    LogTraceEvent(g_Log, "Read config file success. [%s, %s, %s, %d]\n",
        STOCK_CONFIG_FILE, "SoftWare", "WATCH_FLUSH_TIME",
        g_stCfg.iWatchFlushTime);

    /* ��ȡˢ���µ������ʱ���� */
    g_stCfg.iChipFlushTime = IniReadInt(STOCK_CONFIG_FILE, "SoftWare",
        "CHIP_FLUSH_TIME", 5000);
    LogTraceEvent(g_Log, "Read config file success. [%s, %s, %s, %d]\n",
        STOCK_CONFIG_FILE, "SoftWare", "CHIP_FLUSH_TIME",
        g_stCfg.iChipFlushTime);

    /* ��ȡ�������ʱ���� */
    g_stCfg.iCheckWndTime = IniReadInt(STOCK_CONFIG_FILE, "SoftWare",
        "CHECK_WND_TIME", 5000);
    LogTraceEvent(g_Log, "Read config file success. [%s, %s, %s, %d]\n",
        STOCK_CONFIG_FILE, "SoftWare", "CHECK_WND_TIME",
        g_stCfg.iCheckWndTime);

    /* ��ȡ�Ƿ��������� */
    g_stCfg.iChipTimes = IniReadInt(STOCK_CONFIG_FILE, "SoftWare",
        "CHIP_TIMES", 0);
    LogTraceEvent(g_Log, "Read config file success. [%s, %s, %s, %d]\n",
        STOCK_CONFIG_FILE, "SoftWare", "CHIP_TIMES",
        g_stCfg.iChipTimes);

    return SUCCESS;
} /* End of ConfigInit */

/*******************************************************************************
��������: CheckWindows
����˵��: �����������Ƿ�ok
�������: ��
�������: ��
�� �� ֵ:
    >=0: �ɹ�
    < 0: �������
˵    ��: ��
*******************************************************************************/
OSP_S32 CheckWindows(OSP_VOID)
{
    HWND hwnd, hwnd2;

    /* ��ȡԤ��ϵͳ�����Ϣ */
    // ��ȡԤ��������
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

    // ��ȡԤ����Ϣ��׽����
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

        /* ���³�ʼ��Ԥ����Ϣ */
        if (SUCCESS != WatchInit())
        {
            LogTraceError(g_Log, "Watch init failed.\n");
            return -ERR_STOCK_INIT_WATCH;
        }
        
        LogTraceEvent(g_Log, "Initialize the watch info ok.\n");
    }
    
    /* ��ȡ�µ�ϵͳ�����Ϣ */
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
��������: StockInit
����˵��: ��
�������: ��
�������: ��
�� �� ֵ: ��
˵    ��: ��
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
��������: StockEnd
����˵��: ��
�������: ��
�������: ��
�� �� ֵ: ��
˵    ��: ��
*******************************************************************************/
OSP_VOID StockEnd(OSP_VOID)
{
    WatchEnd();
    TranslateEnd();
} /* End of StockEnd */

/*******************************************************************************
��������: StockTranslate
����˵��: ��
�������: ��
�������: ��
�� �� ֵ:
    >=0: �ɹ�
    < 0: �������
˵    ��: ��
*******************************************************************************/
static OSP_S32 StockTranslate(STOCK_INFO_S * v_pstSI)
{
    if (WATCH_CODE == g_stCfg.iWatchMethod)
    { /* ��Ԥ�����ڽ�����ȡ���˹�Ʊ����, ����ת������Ʊ���� */
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
    { /* ��Ԥ�����ڽ�����ȡ���˹�Ʊ����, ����ת������Ʊ���� */
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
    { /* �Ȼ�ȡ��������, �ֻ�ȡ���˴���, �Ͳ��ô��� */
    
    }

    return SUCCESS;
} /* End of StockTranslate */

/*******************************************************************************
��������: StockChip
����˵��: ��
�������: ��
�������: ��
�� �� ֵ:
    >=0: �ɹ�
    < 0: �������
˵    ��: ��
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
    { /* ������, Ҳ������, ����ʲô? */
        LogTraceEmerg(g_Log, "Failed. [%d, %s, %s, %d, %.2f]\n",
            v_pstSI->iBuy, v_pstSI->szCode, v_pstSI->szName, v_pstSI->iNum, v_pstSI->fPrice);
        return -ERR_STOCK_CHAOS_CHIPS;
    }

    return SUCCESS;
} /* End of StockChip */


/*******************************************************************************
��������: StockMain
����˵��: ��
�������: ��
�������: ��
�� �� ֵ: ��
˵    ��: ��
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
    
    /* ��ʼ��ϵͳ */
    if (0 != StockInit())
    {
        LogTraceError(g_Log, "Stock system init failed.\n");
        return -ERR_STOCK_INIT_FAILED;
    }
    
    LogTraceEvent(g_Log, "Stock system init success.\n");
    
    while (1)
    {
        /* ��鴰���Ƿ��б仯 */
        if (SUCCESS != CheckWindows())
        {
            KeepAliveWatch(NULL);
            KeepAliveChip(NULL);

            Sleep(2000);
            
            LogTraceError(g_Log, "The are some windows not exist.\n");
            Sleep(g_stCfg.iCheckWndTime);

            continue;
        }
        
        /* ��Ԥ������ץȡԤ����Ϣ */
        OSP_S32 iRet = WatchGetResult(&stSI, g_stCfg.hwndWatchInfo);
        if (iRet > SUCCESS)
        { /* �Ѿ��������� */
            Sleep(100);
            
            if ((GetTickCount() - llWatchTick) >= g_stCfg.iWatchFlushTime)
            { /* ����Ԥ��ϵͳ */
                KeepAliveWatch(g_stCfg.hwndWatchMain);
                llWatchTick = GetTickCount();
            }

            if ((GetTickCount() - llChipTick) >= g_stCfg.iChipFlushTime)
            { /* �����µ�ϵͳ */
                KeepAliveChip(g_stCfg.hwndChipMain);
                llChipTick = GetTickCount();
            }
            
            continue;
        }
        else if (iRet < SUCCESS)
        { /* ������ */
            LogTraceError(g_Log, "Get info from watch window failed.\n");
            break;
        }

        /* ת��һ����Ϣ, ���µ�ϵͳʹ�� */
        if (SUCCESS != StockTranslate(&stSI))
        {
            LogTraceError(g_Log, "Translate record failed.\n");
            continue;
        }

        if (g_stCfg.iChipTimes == 0)
        { /* �������� */
            continue;
        }

        /* �µ�: ������� */
		if (SUCCESS != StockChip(&stSI))
        {
            LogTraceError(g_Log, "Buy/sell chips failed.\n");
            continue;
        }

        /* �ɹ�����ʱ, �轫�µ�������1 */
        if (g_stCfg.iChipTimes > 0)
        {
            g_stCfg.iChipTimes--;
        }
    }

    /* ����ϵͳ */
    StockEnd();

    return iRet;
} /* End of StockMain */


