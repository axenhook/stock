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

#include "Stock.h"
#include "Log.h"

STOCK_CONFIG_S g_stConfig;

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
int ConfigInit(const char *pszConfigFile)
{
    
    return 0;
} /* End of ConfigInit */

/*******************************************************************************
��������: StockInit
����˵��: ��
�������: ��
�������: ��
�� �� ֵ: ��
˵    ��: ��
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
��������: StockEnd
����˵��: ��
�������: ��
�������: ��
�� �� ֵ: ��
˵    ��: ��
*******************************************************************************/
void StockEnd(void)
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
int StockTranslate(STOCK_INFO_S * pstSI)
{
    if (WATCH_CODE == g_stConfig.iWatchMethod)
    { /* ��Ԥ�����ڽ�����ȡ���˹�Ʊ����, ����ת������Ʊ���� */
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
    { /* ��Ԥ�����ڽ�����ȡ���˹�Ʊ����, ����ת������Ʊ���� */
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
    { /* �Ȼ�ȡ��������, �ֻ�ȡ���˴���, �Ͳ��ô��� */
    
    }

    return 0;
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
    { /* ������, Ҳ������, ����ʲô? */
        LogTraceEmerg(g_Log, "Failed. [%d, %s, %s, %d, %.2f]\n",
            pstSI->iBuy, pstSI->szCode, pstSI->szName, pstSI->iNum, pstSI->fPrice);
        return -3;
    }

    return 0;
} /* End of StockChip */


/*******************************************************************************
��������: StockMain
����˵��: ��
�������: ��
�������: ��
�� �� ֵ: ��
˵    ��: ��
*******************************************************************************/
void StockMain(void)
{
    STOCK_INFO_S stSI;

    /* ��ʼ��ϵͳ */
    if (0 != StockInit())
    {
        return;
    }
    
    while (1)
    {
        /* ��Ԥ������ץȡԤ����Ϣ */
        if (0 != WatchGetResult(&stSI))
        {
            Sleep(100);

            /* ����Ԥ��ϵͳ */
            KeepAliveWatch();
            
            /* �����µ�ϵͳ */
            KeepAliveChip();
            
            continue;
        }

        /* ת��һ����Ϣ, ���µ�ϵͳʹ�� */
        if (0 != StockTranslate(&stSI))
        {
            continue;
        }

        /* �µ�: ������� */
        if (0 != StockChip(&stSI))
        {
            continue;
        }
    }

    /* ����ϵͳ */
    StockEnd();
} /* End of StockMain */


