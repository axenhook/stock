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

#include "Stock.h"
#include "Log.h"

STOCK_CONFIG_S g_stConfig;

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
int ConfigInit(const char *pszConfigFile)
{
    
    return 0;
} /* End of ConfigInit */

/*******************************************************************************
函数名称: StockInit
功能说明: 无
输入参数: 无
输出参数: 无
返 回 值: 无
说    明: 无
*******************************************************************************/
int StockInit(void)
{
    memset(&g_stConfig, 0, sizeof(STOCK_CONFIG_S));

    if (0 != ConfigInit(STOCK_CONFIG_FILE))
    {
        LogTraceError(g_Log, "Config init failed. [%s]\n", STOCK_CONFIG_FILE);
        return -1;
    }

    if (0 != TranslateInit(STOCK_CODE_TABLE))
    {
        LogTraceError(g_Log, "Tranlsate init failed. [%s]\n", STOCK_CODE_TABLE);
        return -2;
    }

    if (0 != WatchInit())
    {
        LogTraceError(g_Log, "Watch init failed.\n");
        return -3;
    }

    return 0;
} /* End of StockInit */

/*******************************************************************************
函数名称: StockEnd
功能说明: 无
输入参数: 无
输出参数: 无
返 回 值: 无
说    明: 无
*******************************************************************************/
void StockEnd(void)
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
int StockTranslate(STOCK_INFO_S * pstSI)
{
    if (WATCH_CODE == g_stConfig.iWatchMethod)
    { /* 从预警窗口仅仅获取到了股票代码, 将其转换出股票名称 */
        if (0 != TranslateCode(pstSI->szCode, pstSI->szName, STOCK_NAME_LEN))
        {
            LogTraceError(g_Log, "Translate stock code failed. [%s]\n",
                pstSI->szCode);
            return -1;
        }
        else
        {
            LogTraceEvent(g_Log, "Translate stock code success. [%s, %s]\n",
                pstSI->szCode, pstSI->szName);
        }
    }
    else if (WATCH_NAME == g_stConfig.iWatchMethod)
    { /* 从预警窗口仅仅获取到了股票名称, 将其转换出股票代码 */
        if (0 != TranslateName(pstSI->szName, pstSI->szCode, STOCK_CODE_LEN))
        {
            LogTraceError(g_Log, "Translate stock name failed. [%s]\n",
                pstSI->szName);
            return -2;
        }
        else
        {
            LogTraceEvent(g_Log, "Translate stock name success. [%s, %s]\n",
                pstSI->szName, pstSI->szCode);
        }
    }
    else
    { /* 既获取到了名称, 又获取到了代码, 就不用处理 */
    
    }

    return 0;
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
int StockChip(STOCK_INFO_S * pstSI)
{
    if (CHIP_SELL == pstSI->iBuy)
    {
        if (0 != ChipSell(pstSI))
        {
            LogTraceError(g_Log, "Sell chips failed. [%s, %s]\n",
                pstSI->szCode, pstSI->szName);
            return -1;
        }
        else
        {
            LogTraceEvent(g_Log, "Sell chips success. [%s, %s]\n",
                pstSI->szCode, pstSI->szName);
        }
    }
    else if (CHIP_BUY == pstSI->iBuy)
    {
        if (0 != ChipBuy(pstSI))
        {
            LogTraceError(g_Log, "Buy chips failed. [%s, %s]\n",
                pstSI->szCode, pstSI->szName);
            return -2;
        }
        else
        {
            LogTraceEvent(g_Log, "Buy chips success. [%s, %s]\n",
                pstSI->szCode, pstSI->szName);
        }
    }
    else
    { /* 不是卖, 也不是买, 那是什么? */
        LogTraceEmerg(g_Log, "Failed. [%d, %s, %s, %d, %.2f]\n",
            pstSI->iBuy, pstSI->szCode, pstSI->szName, pstSI->iNum, pstSI->fPrice);
        return -3;
    }

    return 0;
} /* End of StockChip */


/*******************************************************************************
函数名称: StockMain
功能说明: 无
输入参数: 无
输出参数: 无
返 回 值: 无
说    明: 无
*******************************************************************************/
void StockMain(void)
{
    STOCK_INFO_S stSI;

    /* 初始化系统 */
    if (0 != StockInit())
    {
        return;
    }
    
    while (1)
    {
        /* 从预警窗口抓取预警信息 */
        if (0 != WatchGetResult(&stSI))
        {
            Sleep(100);

            /* 保活预警系统 */
            KeepAliveWatch();
            
            /* 保活下单系统 */
            KeepAliveChip();
            
            continue;
        }

        /* 转换一下信息, 供下单系统使用 */
        if (0 != StockTranslate(&stSI))
        {
            continue;
        }

        /* 下单: 买或者卖 */
        if (0 != StockChip(&stSI))
        {
            continue;
        }
    }

    /* 结束系统 */
    StockEnd();
} /* End of StockMain */


