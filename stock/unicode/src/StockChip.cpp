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

#include "Stock.h"
#include "Log.h"
#include "TclExtCmd.h"

#define CMD_LEN   1024

/******************************************************************************
��������: ChipBuy
����˵��: ��
�������: ��
�������: ��
�� �� ֵ: ��
˵    ��: ��
******************************************************************************/
int ChipBuy(STOCK_INFO_S *pstSI)
{
    char cmd[CMD_LEN];
        
    assert(NULL != pstSI);
    
    LogTraceEvent(g_Log, "Buy chip. [%d, %s, %s, %d, %.2f]\n", pstSI->iBuy, 
        pstSI->szCode, pstSI->szName, pstSI->iNum, pstSI->fPrice);
    
    if (CHIP_BUY != pstSI->iBuy)
    {
        LogTraceEmerg(g_Log, "Expect buy chip, but get sell information\n");
        return ERR_INVALID_PARA;
    }

    if (0 == pstSI->szName[0])
    {
        LogTraceEmerg(g_Log, "Chip name is NULL in buy information\n");
        return ERR_INVALID_PARA;
    }

    if (0 == pstSI->szCode[0])
    {
        LogTraceEmerg(g_Log, "Chip code is NULL in buy information\n");
        return ERR_INVALID_PARA;
    }
    
    if (0 == pstSI->iNum)
    {
        LogTraceEmerg(g_Log, "The chips number is 0 in buy information\n");
        return ERR_INVALID_PARA;
    }

    if (0 == pstSI->fPrice)
    {
        LogTraceEmerg(g_Log, "The price is 0 in buy information\n");
        return ERR_INVALID_PARA;
    }

    _snprintf(cmd, CMD_LEN, "set name %s\n" "set code %s\n" "set num %d\n"
        "set price %.2f\n", pstSI->szName, pstSI->szCode, pstSI->iNum,
        pstSI->fPrice);

    LogTraceEvent(g_Log, "Execute cmd. [%s, %s]\n", cmd, STOCK_BUY_CHIP);
    
    if (0 != ExecuteScriptAndFile(cmd, STOCK_BUY_CHIP))
    {
        LogTraceError(g_Log, "Execute cmd failed. [%s, %s]\n",
            cmd, STOCK_BUY_CHIP);
    }
    
    return 0;
} /* End of ChipBuy */

/******************************************************************************
��������: ChipSell
����˵��: ��
�������: ��
�������: ��
�� �� ֵ: ��
˵    ��: ��
******************************************************************************/
int ChipSell(STOCK_INFO_S *pstSI)
{
    char cmd[CMD_LEN];
        
    assert(NULL != pstSI);
    
    LogTraceEvent(g_Log, "Sell chip. [%d, %s, %s, %d, %.2f]\n", pstSI->iBuy, 
        pstSI->szCode, pstSI->szName, pstSI->iNum, pstSI->fPrice);
    
    if (CHIP_SELL != pstSI->iBuy)
    {
        LogTraceEmerg(g_Log, "Expect buy chip, but get sell information\n");
        return ERR_INVALID_PARA;
    }

    if (0 == pstSI->szName[0])
    {
        LogTraceEmerg(g_Log, "Chip name is NULL in sell information\n");
        return ERR_INVALID_PARA;
    }

    if (0 == pstSI->szCode[0])
    {
        LogTraceEmerg(g_Log, "Chip code is NULL in sell information\n");
        return ERR_INVALID_PARA;
    }
    
    if (0 == pstSI->iNum)
    {
        LogTraceEmerg(g_Log, "The chips number is 0 in sell information\n");
        return ERR_INVALID_PARA;
    }

    if (0 == pstSI->fPrice)
    {
        LogTraceEmerg(g_Log, "The price is 0 in sell information\n");
        return ERR_INVALID_PARA;
    }

    _snprintf(cmd, CMD_LEN, "set name %s\n" "set code %s\n" "set num %d\n"
        "set price %.2f\n", pstSI->szName, pstSI->szCode, pstSI->iNum,
        pstSI->fPrice);
    
    LogTraceEvent(g_Log, "Execute cmd. [%s, %s]\n", cmd, STOCK_SELL_CHIP);
    
    if (0 != ExecuteScriptAndFile(cmd, STOCK_SELL_CHIP))
    {
        LogTraceError(g_Log, "Execute cmd failed. [%s, %s]\n",
            cmd, STOCK_SELL_CHIP);
    }
    
    return 0;
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
void DebugStockChip(void)
{
    STOCK_INFO_S stSI;

    if (0 != TranslateInit(STOCK_CODE_TABLE))
    {
        printf("Init failed. [%s]\n", STOCK_CODE_TABLE);
        return;
    }
    
    memset(&stSI, 0, sizeof(stSI));
    stSI.iBuy = CHIP_BUY;
    strcpy(stSI.szName, "�й�����");
    stSI.iNum = 12345;
    stSI.fPrice = (float)12.18;

    if (0 != TranslateName(stSI.szName, stSI.szCode, STOCK_CODE_LEN))
    {
        printf("Translate name failed. [%s]\n", stSI.szName);
    }
    else
    {
        printf("Translate name success. [%s, %s]\n", stSI.szName, stSI.szCode);
    }
    
    if (0 != ChipSell(&stSI))
    {
        printf("Sell chip failed. [%s]\n", stSI.szName);
    }
    else
    {
        printf("Sell chip success. [%s]\n", stSI.szName);
    }
    
    if (0 != ChipBuy(&stSI))
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
    stSI.iNum = 12345;
    stSI.fPrice = (float)28.18;

    if (0 != TranslateCode(stSI.szCode, stSI.szName, STOCK_NAME_LEN))
    {
        printf("Translate code failed. [%s]\n", stSI.szCode);
    }
    else
    {
        printf("Translate code success. [%s, %s]\n", stSI.szCode, stSI.szName);
    }
    
    if (0 != ChipBuy(&stSI))
    {
        printf("Buy chip failed. [%s]\n", stSI.szName);
    }
    else
    {
        printf("Buy chip success. [%s]\n", stSI.szName);
    }
    
    if (0 != ChipSell(&stSI))
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

