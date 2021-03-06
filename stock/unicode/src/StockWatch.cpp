/******************************************************************************
版权所有, 2010~2013, 曾华荣 (zeng_hr@163.com)
文件名: STOCKWATCH.CPP
作者: 曾华荣 (zeng_hr@163.com)  日期: 2010年5月6日
功能描述: 
函数列表: 
  1. ...: 
  
修改历史: 
  作者: 曾华荣 (zeng_hr@163.com)  日期: 2010年5月6日
  -----------------------------------------------------------------------------
  初始版本
******************************************************************************/

#include <string.h>

#include "Stock.h"

/******************************************************************************
函数名称: WatchInit
功能说明: 无
输入参数: 无
输出参数: 无
返 回 值: 无
说    明: 无
******************************************************************************/
int WatchInit(void)
{
    return 0;
} /* End of WatchInit */


/******************************************************************************
函数名称: WatchGetResult
功能说明: 无
输入参数: 无
输出参数: 无
返 回 值: 无
说    明: 无
******************************************************************************/
int WatchGetResult(STOCK_INFO_S *pstSI)
{
    memset(pstSI, 0, sizeof(STOCK_INFO_S));

    pstSI->iBuy = CHIP_SELL;
    strcpy(pstSI->szCode, "002269");
    pstSI->iNum = 12345;
    pstSI->fPrice = (float)28.18;

    return 0;
    
    return -1;
} /* End of WatchGetResult */

/******************************************************************************
函数名称: WatchGetCount
功能说明: 无
输入参数: 无
输出参数: 无
返 回 值: 无
说    明: 无
******************************************************************************/
int WatchGetCount(int *piCnt)
{
    return 0;
} /* End of WatchGetCount */

/*******************************************************************************
函数名称: WatchEnd
功能说明: 无
输入参数: 无
输出参数: 无
返 回 值: 无
说    明: 无
*******************************************************************************/
void WatchEnd(void)
{
    
} /* End of WatchEnd */

