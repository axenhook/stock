/******************************************************************************
��Ȩ����, 2010~2013, ������ (zeng_hr@163.com)
�ļ���: STOCKWATCH.CPP
����: ������ (zeng_hr@163.com)  ����: 2010��5��6��
��������: 
�����б�: 
  1. ...: 
  
�޸���ʷ: 
  ����: ������ (zeng_hr@163.com)  ����: 2010��5��6��
  -----------------------------------------------------------------------------
  ��ʼ�汾
******************************************************************************/

#include <string.h>

#include "osp_globals.h"
#include "Stock.h"
#include "Log.h"
#include "WndLib.h"
#include "inifile.h"

#define INVALID_PARA   (-1)

typedef struct tagWATCH_PARSE_S
{ /* Ԥ����Ϣ���ڵ�λ�� */
    OSP_S32 iColumn;  /* ��Ϣ���ڵ��к� */
    OSP_S32 iStart;   /* ��Ϣ���ڵ��ַ�����ʼ��� */
    OSP_S32 iLen;     /* ��Ϣ���ڵ��ַ�������, 0�������� */
} WATCH_PARSE_S; /* End of WATCH_PARSE */

typedef struct tagWATCH_INFO_E
{
    /* Ԥ����Ϣ��������, �������ļ��ж�ȡ�Ĳ��ṹ�� */
    WATCH_PARSE_S stBuy;    /* �������� */
    WATCH_PARSE_S stCode;   /* ֤ȯ���� */
    WATCH_PARSE_S stName;   /* ֤ȯ���� */
    WATCH_PARSE_S stPrice;  /* �����۸� */
    WATCH_PARSE_S stNum;    /* �������� */
    WATCH_PARSE_S stTime;   /* Ԥ��������ʱ�� */

    /* Ԥ����Ϣ���ڼ�¼�������Ϣ */
    OSP_S32 iDoneRecords;    /* �Ѿ������˶���Ԥ����¼ */
    OSP_S32 iTotalRecords;   /* ��ǰ�ܹ��ж���Ԥ����¼ */
} WATCH_INFO_E; /* End of WATCH_INFO */

WATCH_INFO_E g_stWi;

/******************************************************************************
��������: WatchInit
����˵��: ��
�������: ��
�������: ��
�� �� ֵ: ��
˵    ��: ��
******************************************************************************/
OSP_S32 WatchInit(OSP_VOID)
{
    g_stWi.iDoneRecords = 0;
    g_stWi.iTotalRecords = 0;

    
    /* ��ȡ������������ */
    //g_stWi.stBuy.iColumn = 1;
    //g_stWi.stBuy.iStart = 0;
    //g_stWi.stBuy.iLen = 4;
    g_stWi.stBuy.iColumn = IniReadInt(STOCK_CONFIG_FILE, "Watch",
        "BUY_COLUMN", INVALID_PARA);
    LogTraceEvent(g_Log, "Read config file success. [%s, %s, %s, %d]\n",
        STOCK_CONFIG_FILE, "Watch", "BUY_COLUMN",
        g_stWi.stBuy.iColumn);
    
    g_stWi.stBuy.iStart = IniReadInt(STOCK_CONFIG_FILE, "Watch",
        "BUY_START", INVALID_PARA);
    LogTraceEvent(g_Log, "Read config file success. [%s, %s, %s, %d]\n",
        STOCK_CONFIG_FILE, "Watch", "BUY_START",
        g_stWi.stBuy.iStart);
    
    g_stWi.stBuy.iLen = IniReadInt(STOCK_CONFIG_FILE, "Watch",
        "BUY_LEN", INVALID_PARA);
    LogTraceEvent(g_Log, "Read config file success. [%s, %s, %s, %d]\n",
        STOCK_CONFIG_FILE, "Watch", "BUY_LEN",
        g_stWi.stBuy.iLen);

    
    /* ��ȡ֤ȯ�������� */
    //g_stWi.stCode.iColumn = INVALID_PARA;
    //g_stWi.stCode.iStart = INVALID_PARA;
    //g_stWi.stCode.iLen = INVALID_PARA;
    g_stWi.stCode.iColumn = IniReadInt(STOCK_CONFIG_FILE, "Watch",
        "CODE_COLUMN", INVALID_PARA);
    LogTraceEvent(g_Log, "Read config file success. [%s, %s, %s, %d]\n",
        STOCK_CONFIG_FILE, "Watch", "CODE_COLUMN",
        g_stWi.stCode.iColumn);
    
    g_stWi.stCode.iStart = IniReadInt(STOCK_CONFIG_FILE, "Watch",
        "CODE_START", INVALID_PARA);
    LogTraceEvent(g_Log, "Read config file success. [%s, %s, %s, %d]\n",
        STOCK_CONFIG_FILE, "Watch", "CODE_START",
        g_stWi.stCode.iStart);
    
    g_stWi.stCode.iLen = IniReadInt(STOCK_CONFIG_FILE, "Watch",
        "CODE_LEN", INVALID_PARA);
    LogTraceEvent(g_Log, "Read config file success. [%s, %s, %s, %d]\n",
        STOCK_CONFIG_FILE, "Watch", "CODE_LEN",
        g_stWi.stCode.iLen);
    
    /* ��ȡ֤ȯ�������� */
    //g_stWi.stName.iColumn = 0;
    //g_stWi.stName.iStart = 0;
    //g_stWi.stName.iLen = 0;
    g_stWi.stName.iColumn = IniReadInt(STOCK_CONFIG_FILE, "Watch",
        "NAME_COLUMN", INVALID_PARA);
    LogTraceEvent(g_Log, "Read config file success. [%s, %s, %s, %d]\n",
        STOCK_CONFIG_FILE, "Watch", "NAME_COLUMN",
        g_stWi.stName.iColumn);
    
    g_stWi.stName.iStart = IniReadInt(STOCK_CONFIG_FILE, "Watch",
        "NAME_START", INVALID_PARA);
    LogTraceEvent(g_Log, "Read config file success. [%s, %s, %s, %d]\n",
        STOCK_CONFIG_FILE, "Watch", "NAME_START",
        g_stWi.stName.iStart);
    
    g_stWi.stName.iLen = IniReadInt(STOCK_CONFIG_FILE, "Watch",
        "NAME_LEN", INVALID_PARA);
    LogTraceEvent(g_Log, "Read config file success. [%s, %s, %s, %d]\n",
        STOCK_CONFIG_FILE, "Watch", "NAME_LEN",
        g_stWi.stName.iLen);
    
    /* ��ȡ�����۸����� */
    //g_stWi.stPrice.iColumn = 2;
    //g_stWi.stPrice.iStart = 0;
    //g_stWi.stPrice.iLen = 0;
    g_stWi.stPrice.iColumn = IniReadInt(STOCK_CONFIG_FILE, "Watch",
        "PRICE_COLUMN", INVALID_PARA);
    LogTraceEvent(g_Log, "Read config file success. [%s, %s, %s, %d]\n",
        STOCK_CONFIG_FILE, "Watch", "PRICE_COLUMN",
        g_stWi.stPrice.iColumn);
    
    g_stWi.stPrice.iStart = IniReadInt(STOCK_CONFIG_FILE, "Watch",
        "PRICE_START", INVALID_PARA);
    LogTraceEvent(g_Log, "Read config file success. [%s, %s, %s, %d]\n",
        STOCK_CONFIG_FILE, "Watch", "PRICE_START",
        g_stWi.stPrice.iStart);
    
    g_stWi.stPrice.iLen = IniReadInt(STOCK_CONFIG_FILE, "Watch",
        "PRICE_LEN", INVALID_PARA);
    LogTraceEvent(g_Log, "Read config file success. [%s, %s, %s, %d]\n",
        STOCK_CONFIG_FILE, "Watch", "PRICE_LEN",
        g_stWi.stPrice.iLen);
    
    /* ��ȡ������������ */
    //g_stWi.stNum.iColumn = 1;
    //g_stWi.stNum.iStart = 4;
    //g_stWi.stNum.iLen = 6;
    g_stWi.stNum.iColumn = IniReadInt(STOCK_CONFIG_FILE, "Watch",
        "NUM_COLUMN", INVALID_PARA);
    LogTraceEvent(g_Log, "Read config file success. [%s, %s, %s, %d]\n",
        STOCK_CONFIG_FILE, "Watch", "NUM_COLUMN",
        g_stWi.stNum.iColumn);
    
    g_stWi.stNum.iStart = IniReadInt(STOCK_CONFIG_FILE, "Watch",
        "NUM_START", INVALID_PARA);
    LogTraceEvent(g_Log, "Read config file success. [%s, %s, %s, %d]\n",
        STOCK_CONFIG_FILE, "Watch", "NUM_START",
        g_stWi.stNum.iStart);
    
    g_stWi.stNum.iLen = IniReadInt(STOCK_CONFIG_FILE, "Watch",
        "NUM_LEN", INVALID_PARA);
    LogTraceEvent(g_Log, "Read config file success. [%s, %s, %s, %d]\n",
        STOCK_CONFIG_FILE, "Watch", "NUM_LEN",
        g_stWi.stNum.iLen);
    
    /* ��ȡԤ��ʱ������ */
    //g_stWi.stTime.iColumn = 3;
    //g_stWi.stTime.iStart = 0;
    // g_stWi.stTime.iLen = 0;
    g_stWi.stTime.iColumn = IniReadInt(STOCK_CONFIG_FILE, "Watch",
        "TIME_COLUMN", INVALID_PARA);
    LogTraceEvent(g_Log, "Read config file success. [%s, %s, %s, %d]\n",
        STOCK_CONFIG_FILE, "Watch", "TIME_COLUMN",
        g_stWi.stTime.iColumn);
    
    g_stWi.stTime.iStart = IniReadInt(STOCK_CONFIG_FILE, "Watch",
        "TIME_START", INVALID_PARA);
    LogTraceEvent(g_Log, "Read config file success. [%s, %s, %s, %d]\n",
        STOCK_CONFIG_FILE, "Watch", "TIME_START",
        g_stWi.stTime.iStart);
    
    g_stWi.stTime.iLen = IniReadInt(STOCK_CONFIG_FILE, "Watch",
        "TIME_LEN", INVALID_PARA);
    LogTraceEvent(g_Log, "Read config file success. [%s, %s, %s, %d]\n",
        STOCK_CONFIG_FILE, "Watch", "TIME_LEN",
        g_stWi.stTime.iLen);
    
    return SUCCESS;
} /* End of WatchInit */

/*******************************************************************************
��������: WatchNCpy
����˵��: ��
�������: ��
�������: ��
�� �� ֵ:
    >=0: �ɹ�
    < 0: �������
˵    ��: ��
*******************************************************************************/
OSP_VOID WatchNCpy(OSP_S8 *v_pszDst, const OSP_S8 *v_pszSrc, OSP_S32 v_iLen)
{
    while ((0 != *v_pszSrc) && (COLUMN_SIGNATURE != *v_pszSrc))
    {
        *v_pszDst++ = *v_pszSrc++;
        
        if (0 == v_iLen)
        {
            continue;
        }
        
        if (0 == --v_iLen)
        {
            break;
        }
    }
} /* End of WatchNCpy */

/*******************************************************************************
��������: ParseSingleInfo
����˵��: ��
�������: ��
�������: ��
�� �� ֵ:
    >=0: �ɹ�
    < 0: �������
˵    ��: ��
*******************************************************************************/
static OSP_S32 ParseSingleInfo(const OSP_S8 *v_pszSrc,
    OSP_S8 *v_pszDst, OSP_S32 v_iDstLen, WATCH_PARSE_S *v_pstWp)
{
    OSP_S8 *pcStart = (OSP_S8 *)v_pszSrc;
    
    if ((0 != v_pstWp->iLen) && (v_iDstLen < v_pstWp->iLen))
    {
        LogTraceEmerg(g_Log, "Memory is too small. [%d, %d]\n",
            v_iDstLen, v_pstWp->iLen);
        return -ERR_WATCH_MEMORY_TOO_SMALL;
    }

    if (INVALID_PARA == v_pstWp->iColumn)
    {
        LogTraceDebug(g_Log, "No need parse this record.\n");
        return SUCCESS_WATCH_NEED_NOT_PARSE;
    }

    for (OSP_S32 i = 0; i < v_pstWp->iColumn; i++)
    { /* ������ */
        while (*pcStart != COLUMN_SIGNATURE)
        {
            if (0 == *pcStart)
            { /* �ַ��������� */
                LogTraceError(g_Log, "Come to string end when parse.\n");
                return -ERR_WATCH_PARSE_FAILED;
            }
            
            pcStart++; 
        }
        
        pcStart++; /* �����б�ʶ�� */
    }
    
    if (0 == *pcStart)
    { /* �ַ��������� */
        LogTraceError(g_Log, "Come to string end when parse.\n");
        return -ERR_WATCH_PARSE_FAILED;
    }
    
    for (OSP_S32 i = 0; i < v_pstWp->iStart; i++)
    { /* ������ʼλ�� */
        pcStart++;
        
        if (0 == *pcStart)
        { /* �ַ��������� */
            LogTraceError(g_Log, "Come to string end when parse.\n");
            return -ERR_WATCH_PARSE_FAILED;
        }
    }

    memset(v_pszDst, 0, v_iDstLen);
    WatchNCpy(v_pszDst, pcStart, v_pstWp->iLen);
    
    return SUCCESS;
} /* End of ParseSingleInfo */

/*******************************************************************************
��������: ParseWatchInfo
����˵��: ��
�������: ��
�������: ��
�� �� ֵ:
    >=0: �ɹ�
    < 0: �������
˵    ��: ��
*******************************************************************************/
static OSP_S32 ParseWatchInfo(const OSP_S8 *v_pszContent, STOCK_INFO_S *v_pstSI)
{
    OSP_S8 szDst[STOCK_MAX_LEN];
    OSP_S32 iRet;
    
    memset(v_pstSI, 0, sizeof(STOCK_INFO_S));
    
    /* ������������ */
    iRet = ParseSingleInfo(v_pszContent, szDst, STOCK_MAX_LEN, &g_stWi.stBuy);
    if (iRet < SUCCESS)
    {
        LogTraceError(g_Log, "Parse buy/sell failed. [%s]\n", v_pszContent);
        return -ERR_WATCH_PARSE_BUY;
    }
    else if (iRet == SUCCESS)
    {
        if (0 == strcmp(szDst, "����"))
        {
            v_pstSI->iBuy = CHIP_BUY;
        }
        else if (0 == strcmp(szDst, "����"))
        {
            v_pstSI->iBuy = CHIP_SELL;
        }
        else
        {
            LogTraceError(g_Log, "buy/sell info is invalid. [%s, %s]\n",
                v_pszContent, szDst);
            return -ERR_WATCH_BUY_INFO_INVALID;
        }
        
        LogTraceDebug(g_Log, "Parse buy/sell success. [%s, %s, %d]\n",
            v_pszContent, szDst, v_pstSI->iBuy);
    }
    
    /* ֤ȯ����, ���ó���Ч */
    iRet = ParseSingleInfo(v_pszContent, szDst, STOCK_MAX_LEN, &g_stWi.stCode);
    if (iRet < SUCCESS)
    {
        LogTraceError(g_Log, "Parse code failed. [%s]\n", v_pszContent);
        return -ERR_WATCH_PARSE_CODE;
    }
    else if (iRet == SUCCESS)
    {
        strncpy(v_pstSI->szCode, szDst, STOCK_CODE_LEN);
        LogTraceDebug(g_Log, "Parse code success. [%s, %s, %s]\n",
            v_pszContent, szDst, v_pstSI->szCode);
    }
    
    /* ֤ȯ���� */
    iRet = ParseSingleInfo(v_pszContent, szDst, STOCK_MAX_LEN, &g_stWi.stName);
    if (iRet < SUCCESS)
    {
        LogTraceError(g_Log, "Parse name failed. [%s]\n", v_pszContent);
        return -ERR_WATCH_PARSE_NAME;
    }
    else if (iRet == SUCCESS)
    {
        strncpy(v_pstSI->szName, szDst, STOCK_NAME_LEN);
        LogTraceDebug(g_Log, "Parse name success. [%s, %s, %s]\n",
            v_pszContent, szDst, v_pstSI->szName);
    }
    
    /* �����۸� */
    iRet = ParseSingleInfo(v_pszContent, szDst, STOCK_MAX_LEN, &g_stWi.stPrice);
    if (iRet < SUCCESS)
    {
        LogTraceError(g_Log, "Parse price failed. [%s]\n", v_pszContent);
        return -ERR_WATCH_PARSE_PRICE;
    }
    else if (iRet == SUCCESS)
    {
        v_pstSI->fPrice = atof(szDst);
        LogTraceDebug(g_Log, "Parse price success. [%s, %s, %f]\n",
            v_pszContent, szDst, v_pstSI->fPrice);
    }
    
    /* �������� */
    iRet = ParseSingleInfo(v_pszContent, szDst, STOCK_MAX_LEN, &g_stWi.stNum);
    if (iRet < SUCCESS)
    {
        LogTraceError(g_Log, "Parse number failed. [%s]\n", v_pszContent);
        return -ERR_WATCH_PARSE_NUMBER;
    }
    else if (iRet == SUCCESS)
    {
        v_pstSI->iNum = atoi(szDst);
        LogTraceDebug(g_Log, "Parse number success. [%s, %s, %d]\n",
            v_pszContent, szDst, v_pstSI->iNum);
    }
    
    /* ����ʱ�� */
    iRet = ParseSingleInfo(v_pszContent, szDst, STOCK_MAX_LEN, &g_stWi.stTime);
    if (iRet < SUCCESS)
    {
        LogTraceError(g_Log, "Parse time failed. [%s]\n", v_pszContent);
        return -ERR_WATCH_PARSE_TIME;
    }
    else if (iRet == SUCCESS)


    {
        strncpy(v_pstSI->szTime, szDst, STOCK_TIME_LEN);
        LogTraceDebug(g_Log, "Parse time success. [%s, %s, %s]\n",
            v_pszContent, szDst, v_pstSI->szTime);
    }
    
    LogTraceEvent(g_Log, "Parse info success. [%s, %d, %s, %s, %d, %f, %s]\n",
        v_pszContent, v_pstSI->iBuy, v_pstSI->szName, v_pstSI->szCode,
        v_pstSI->iNum, v_pstSI->fPrice, v_pstSI->szTime);
    
    return SUCCESS;
} /* End of ParseWatchInfo */

/******************************************************************************
��������: WatchGetResult
����˵��: ��
�������: ��
�������: ��
�� �� ֵ: ��
˵    ��: ��
******************************************************************************/
OSP_S32 WatchGetResult(STOCK_INFO_S *v_pstSI, HWND v_hwnd)
{
    OSP_S8 szText[STOCK_MAX_LEN];
    OSP_S32 iTotal = 0;
    
    if (NULL == v_hwnd)
    { /* ������Ϣ�쳣 */
        LogTraceError(g_Log, "Watch info window is NULL.\n");
        return -ERR_WATCH_INFO_HWND_INVALID;
    }

    g_stWi.iTotalRecords = LVGetRowNum(v_hwnd);
    
    if (g_stWi.iTotalRecords < 0)
    { /* ��ȡʧ�� */
        LogTraceError(g_Log, "Get row number failed. [%d]\n", g_stWi.iTotalRecords);
        return g_stWi.iTotalRecords;
    }
    
    if (g_stWi.iDoneRecords > g_stWi.iTotalRecords)
    { /* �Ѿ������������е�Ԥ����Ϣ */
        LogTraceEvent(g_Log, "Done is larger than total records. [%d, %d]\n",
            g_stWi.iDoneRecords, g_stWi.iTotalRecords);
        g_stWi.iDoneRecords = g_stWi.iTotalRecords;
        return SUCCESS_WATCH_DONE_LARGER;
    }
    else if (g_stWi.iDoneRecords == g_stWi.iTotalRecords)
    { /* �Ѿ������������е����� */
        return SUCCESS_WATCH_ALL_DONE;
    }

    iTotal = g_stWi.iTotalRecords;
    LogTraceEvent(g_Log, "There are records got. [%d]\n", iTotal);
    
    while (1) 
    {
        g_stWi.iTotalRecords = iTotal; /* ��ȡ��¼ǰ��¼һ�������� */
        
        if (LVGetRowContent(v_hwnd, iTotal - g_stWi.iDoneRecords - 1,
            szText, STOCK_MAX_LEN))
        {
            LogTraceError(g_Log, "Get watch info content failed. [%d]\n",
                g_stWi.iTotalRecords - g_stWi.iDoneRecords - 1);
            return -ERR_WATCH_GET_CONTENT;
        }

        iTotal = LVGetRowNum(v_hwnd); /* ��ȡ��¼���¼һ�������� */

        if (iTotal > g_stWi.iTotalRecords)
        { /* ���Ƿ����µļ�¼���� */
            LogTraceEvent(g_Log, "There are new records, reget now. [%d, %d]\n",
                iTotal, g_stWi.iTotalRecords);
        }
        else
        { /* û���¼�¼���� */
            break;
        }
    }
    
    g_stWi.iDoneRecords++;

    szText[STOCK_MAX_LEN - 1] = 0; /* ȷ���н����� */
    
    return (ParseWatchInfo(szText, v_pstSI));
} /* End of WatchGetResult */

/*******************************************************************************
��������: WatchEnd
����˵��: ��
�������: ��
�������: ��
�� �� ֵ: ��
˵    ��: ��
*******************************************************************************/
OSP_VOID WatchEnd(OSP_VOID)
{
    
} /* End of WatchEnd */

#define __DEBUG_STOCK_WATCH__

#ifdef __DEBUG_STOCK_WATCH__

OSP_VOID DebugStockWatch(OSP_VOID)
{
    STOCK_INFO_S si;
    OSP_S8 szStr[STOCK_MAX_LEN];

    WatchInit();
    
    strcpy(szStr, "��������#����002000�����ַ���#13.62#2010��5��30��");
    if (SUCCESS != ParseWatchInfo(szStr, &si))
    {
        LogTraceEvent(g_Log, "Parse failed.\n");
    }

    strcpy(szStr, "�������#����002000�����ַ���#25#2010��5��30��");
    if (SUCCESS != ParseWatchInfo(szStr, &si))
    {
        LogTraceEvent(g_Log, "Parse failed.\n");
    }

}

#endif

