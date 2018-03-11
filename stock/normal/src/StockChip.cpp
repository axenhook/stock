/******************************************************************************
��Ȩ����, 2010~2013, ������ (zeng_hr@163.com)
�ļ���: STOCKCHIP.CPP
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
#include <assert.h>

#include "osp_globals.h"
#include "Stock.h"
#include "Log.h"
#include "TclExtCmd.h"

/******************************************************************************
��������: ChipBuy
����˵��: ��
�������: ��
�������: ��
�� �� ֵ: ��
˵    ��: ��
******************************************************************************/
OSP_S32 ChipBuy(STOCK_INFO_S *v_pstSI, HWND v_hwnd)
{
    OSP_S8 szCmd[STOCK_CMD_LEN];
        
    assert(NULL != v_pstSI);
    
    LogTraceEvent(g_Log, "Buy chip. [%d, %s, %s, %d, %.2f]\n", v_pstSI->iBuy, 
        v_pstSI->szCode, v_pstSI->szName, v_pstSI->iNum, v_pstSI->fPrice);

    if (NULL == v_hwnd)
    { /* ���ھ���쳣 */
        LogTraceError(g_Log, "Chip window handle is NULL.\n");
        return -ERR_CHIP_HWND;
    }
    
    if (CHIP_BUY != v_pstSI->iBuy)
    {
        LogTraceEmerg(g_Log, "Expect buy chip, but get sell information\n");
        return -ERR_CHIP_BUY_METHOD;
    }

    if (0 == v_pstSI->szName[0])
    {
        LogTraceEmerg(g_Log, "Chip name is NULL in buy information\n");
        return -ERR_CHIP_NAME_NULL;
    }

    if (0 == v_pstSI->szCode[0])
    {
        LogTraceEmerg(g_Log, "Chip code is NULL in buy information\n");
        return -ERR_CHIP_CODE_NULL;
    }
    
    if (0 == v_pstSI->iNum)
    {
        LogTraceEmerg(g_Log, "The chips number is 0 in buy information\n");
        return -ERR_CHIP_NUMBER_ZERO;
    }

    if (0 == v_pstSI->fPrice)
    {
        LogTraceEmerg(g_Log, "The price is 0 in buy information\n");
        return -ERR_CHIP_PRICE_ZERO;
    }

    _snprintf(szCmd, STOCK_CMD_LEN, "set name %s\n" "set code %s\n" "set num %d\n"
        "set price %.2f\n" "set hwnd %d\n", v_pstSI->szName, v_pstSI->szCode,
        v_pstSI->iNum, v_pstSI->fPrice, v_hwnd);

    LogTraceEvent(g_Log, "Execute cmd. [%s, %s]\n", szCmd, STOCK_BUY_CHIP);
    
    if (0 != ExecuteScriptAndFile(szCmd, STOCK_BUY_CHIP))
    {
        LogTraceError(g_Log, "Execute cmd failed. [%s, %s]\n",
            szCmd, STOCK_BUY_CHIP);
        return -ERR_CHIP_EXEC_BUY_SCRIPT;
    }
    
    return SUCCESS;
} /* End of ChipBuy */

/******************************************************************************
��������: ChipSell
����˵��: ��
�������: ��
�������: ��
�� �� ֵ: ��
˵    ��: ��
******************************************************************************/
OSP_S32 ChipSell(STOCK_INFO_S *v_pstSI, HWND v_hwnd)
{
    OSP_S8 szCmd[STOCK_CMD_LEN];
        
    assert(NULL != v_pstSI);
    
    LogTraceEvent(g_Log, "Sell chip. [%d, %s, %s, %d, %.2f]\n", v_pstSI->iBuy, 
        v_pstSI->szCode, v_pstSI->szName, v_pstSI->iNum, v_pstSI->fPrice);
    
    if (NULL == v_hwnd)
    { /* ���ھ���쳣 */
        LogTraceError(g_Log, "Chip window handle is NULL.\n");
        return -ERR_CHIP_HWND;
    }
    
    if (CHIP_SELL != v_pstSI->iBuy)
    {
        LogTraceEmerg(g_Log, "Expect buy chip, but get sell information\n");
        return -ERR_CHIP_BUY_METHOD;
    }

    if (0 == v_pstSI->szName[0])
    {
        LogTraceEmerg(g_Log, "Chip name is NULL in sell information\n");
        return -ERR_CHIP_NAME_NULL;
    }

    if (0 == v_pstSI->szCode[0])
    {
        LogTraceEmerg(g_Log, "Chip code is NULL in sell information\n");
        return -ERR_CHIP_CODE_NULL;
    }
    
    if (0 == v_pstSI->iNum)
    {
        LogTraceEmerg(g_Log, "The chips number is 0 in sell information\n");
        return -ERR_CHIP_NUMBER_ZERO;
    }

    if (0 == v_pstSI->fPrice)
    {
        LogTraceEmerg(g_Log, "The price is 0 in sell information\n");
        return -ERR_CHIP_PRICE_ZERO;
    }

    _snprintf(szCmd, STOCK_CMD_LEN, "set name %s\n" "set code %s\n" "set num %d\n"
        "set price %.2f\n" "set hwnd %d\n", v_pstSI->szName, v_pstSI->szCode,
        v_pstSI->iNum, v_pstSI->fPrice, v_hwnd);
    
    LogTraceEvent(g_Log, "Execute cmd. [%s, %s]\n", szCmd, STOCK_SELL_CHIP);
    
    if (0 != ExecuteScriptAndFile(szCmd, STOCK_SELL_CHIP))
    {
        LogTraceError(g_Log, "Execute cmd failed. [%s, %s]\n",
            szCmd, STOCK_SELL_CHIP);
        return -ERR_CHIP_EXEC_BUY_SCRIPT;
    }
    
    return SUCCESS;
} /* End of ChipSell */

#define __DEBUG_STOCK_CHIP__

#ifdef __DEBUG_STOCK_CHIP__

/*******************************************************************************
��������: DebugStockChip
����˵��: ��
�������: ��
�������: ��
�� �� ֵ: ��
˵    ��: ��
*******************************************************************************/
OSP_VOID DebugStockChip(OSP_VOID)
{
    STOCK_INFO_S stSI;
    HWND hwnd = (HWND)853878;

    if (SUCCESS != TranslateInit(STOCK_CODE_TABLE))
    {
        printf("Init failed. [%s]\n", STOCK_CODE_TABLE);
        return;
    }
    
    memset(&stSI, 0, sizeof(stSI));
    stSI.iBuy = CHIP_BUY;
    strcpy(stSI.szName, "�й�����");
    stSI.iNum = 100;
    stSI.fPrice = (float)11.18;

    if (SUCCESS != TranslateName(stSI.szName, stSI.szCode, STOCK_CODE_LEN))
    {
        printf("Translate name failed. [%s]\n", stSI.szName);
    }
    else
    {
        printf("Translate name success. [%s, %s]\n", stSI.szName, stSI.szCode);
    }
    
    if (SUCCESS != ChipSell(&stSI, hwnd))
    {
        printf("Sell chip failed. [%s]\n", stSI.szName);
    }
    else
    {
        printf("Sell chip success. [%s]\n", stSI.szName);
    }
    
    if (SUCCESS != ChipBuy(&stSI, hwnd))
    {
        printf("Buy chip failed. [%s]\n", stSI.szName);
    }
    else
    {
        printf("Buy chip success. [%s]\n", stSI.szName);
    }
    
    memset(&stSI, 0, sizeof(stSI));
    stSI.iBuy = CHIP_SELL;
    strcpy(stSI.szCode, "002269");
    stSI.iNum = 100;
    stSI.fPrice = (float)20.18;

    if (SUCCESS != TranslateCode(stSI.szCode, stSI.szName, STOCK_NAME_LEN))
    {
        printf("Translate code failed. [%s]\n", stSI.szCode);
    }
    else
    {
        printf("Translate code success. [%s, %s]\n", stSI.szCode, stSI.szName);
    }
    
    if (SUCCESS != ChipBuy(&stSI, hwnd))
    {
        printf("Buy chip failed. [%s]\n", stSI.szName);
    }
    else
    {
        printf("Buy chip success. [%s]\n", stSI.szName);
    }
    
    if (SUCCESS != ChipSell(&stSI, hwnd))
    {
        printf("Sell chip failed. [%s]\n", stSI.szName);
    }
    else
    {
        printf("Sell chip success. [%s]\n", stSI.szName);
    }
    
    TranslateEnd();
} /* End of DebugStockChip */

#endif

