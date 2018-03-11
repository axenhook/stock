/******************************************************************************
版权所有, 2010~2013, 曾华荣 (zeng_hr@163.com)
文件名: STOCKTRANSLATE.CPP
作者: 曾华荣 (zeng_hr@163.com)  日期: 2010年5月6日
功能描述: 
函数列表: 
  1. ...: 
  
修改历史: 
  作者: 曾华荣 (zeng_hr@163.com)  日期: 2010年5月6日
  -----------------------------------------------------------------------------
  初始版本
******************************************************************************/
//#define   _CRTDBG_MAP_ALLOC   
#include <stdlib.h>   
#include <crtdbg.h>
#include <stdio.h>

#include "osp_globals.h"
#include "Stock.h"
#include "Log.h"

#define STOCK_NAME_REAL_LEN   (STOCK_NAME_LEN - 1)
#define STOCK_CODE_REAL_LEN   (STOCK_CODE_LEN - 1)

#define STATE_CHANGE  0
#define STATE_CODE    1 // 解析股票代码状态
#define STATE_NAME    2 // 解析股票名称状态
#define STATE_ERROR   3 // 遇到错误, 需找到下一个字符串的开始

typedef struct tagTRANSLATE_TABLE
{
    OSP_S8 szName[STOCK_NAME_LEN];  /* 股票名称 */
    OSP_S8 szCode[STOCK_CODE_LEN];  /* 股票代码 */
} TRANSLATE_TABLE; /* End of TRANSLATE_TABLE */

static TRANSLATE_TABLE *g_pastTrTbl = NULL;
static OSP_S32 g_iTrTblSize = 0;

/******************************************************************************
函数名称: GetRecordCnt
功能说明: 无
输入参数: 无
输出参数: 无
返 回 值: 无
说    明: 无
******************************************************************************/
static OSP_S32 GetRecordCnt(OSP_S8 *v_pszContent, OSP_S32 v_iSize)
{
    OSP_S32 iState = STATE_CHANGE;
    OSP_S32 iIdx = 0;
    OSP_S32 iCnt = 0;
    
    /* 计算总共有多少有效记录 */
    for (OSP_S32 i = 0; i < v_iSize; i++)
    {
        switch (iState)
        {
            case STATE_CHANGE:
            {
                if (v_pszContent[i] == '\0')
                { // 继续检查下一个是否是结束符
                    break;
                }
                
                // 有内容了, 应该是股票代码, 不能加break
                iIdx = 0;
                iState = STATE_CODE;
                LogTraceDebug(g_Log, "Current format changed. [%d]\n", iState);
            }
            
            case STATE_CODE:
            {
                if (v_pszContent[i] == ',')
                { // 紧接着就是股票名称
                    if (iIdx == 0)
                    { // 还没有获取到股票代码呢
                        iState = STATE_ERROR;
                        LogTraceDebug(g_Log, "Current format changed. [%d]\n",
                            iState);
                        break;
                    }

                    // 获取到了股票代码
                    iIdx = 0;
                    iState = STATE_NAME;
                    LogTraceDebug(g_Log, "Current format changed. [%d]\n",
                        iState);
                    break;
                }
                else if (v_pszContent[i] == '\0')
                { // 还没获取到股票名称呢, 就结束了
                    iIdx = 0;
                    LogTraceDebug(g_Log, "Current format changed. [%d]\n",
                        iState);
                    break;
                }
                else
                { // 填充股票代码
                    iIdx++;
                }
                
                break;
            }
            
            case STATE_NAME:
            {
                if (v_pszContent[i] == '\0')
                { // 字符串结束了
                    if (iIdx == 0)
                    { // 还没获取到股票名称呢, 当前记录无效
                        iState = STATE_CHANGE;
                        LogTraceDebug(g_Log, "Current format changed. [%d]\n",
                            iState);
                        break;
                    }

                    iCnt++;
                    iState = STATE_CHANGE;
                    LogTraceDebug(g_Log, "Current format changed. [%d]\n",
                        iState);
                    break;
                }
                else
                { // 填充股票名称
                    iIdx++;
                }
                
                break;
            }
            
            case STATE_ERROR:
            {
                if (v_pszContent[i] != '\0')
                { // 继续检查下一个是否是结束符
                    break;
                }
                
                // 找到结束符了
                iState = STATE_CHANGE;
                LogTraceDebug(g_Log, "Current format changed. [%d]\n", iState);
                break;
            }
            
            default:
            {
                LogTraceEmerg(g_Log, "State chaos, Please check code.\n");
                break;
            }
        }
    }

    if ((iState == STATE_NAME) && (iIdx != 0))
    { // 处于获取名称的状态, 并且获取到了名称, 说明最后一条记录可能没有结束符
        iCnt++;
    }

    return iCnt;
} /* End of GetRecordCnt */

/******************************************************************************
函数名称: FormatTable
功能说明: 无
输入参数: 无
输出参数: 无
返 回 值: 无
说    明: 无
******************************************************************************/
static OSP_S32 FormatTable(OSP_S8 *v_pszContent, OSP_S32 v_iSize)
{
    OSP_S32 iState = STATE_CHANGE;
    OSP_S32 iIdx = 0;
    OSP_S32 iTooLong = 0;
    OSP_S8 *pcStart = NULL;
    OSP_S32 iCnt = 0;
    
    /* 将所有的换行符转换成结束符 */
    for (OSP_S32 i = 0; i < v_iSize; i++)
    {
        if ((v_pszContent[i] == 0x0D) || (v_pszContent[i] == 0x0A))
        {
            v_pszContent[i] = 0x00;
        }
    }

    iCnt = GetRecordCnt(v_pszContent, v_iSize);
    if (0 == iCnt)
    { // 无记录
        LogTraceError(g_Log, "There are no record in code table file.\n");
        return -ERR_TRANSLATE_NO_RECORD;
    }
    
    g_pastTrTbl = (TRANSLATE_TABLE *)malloc(iCnt * sizeof(TRANSLATE_TABLE));
    if (NULL == g_pastTrTbl)
    {

        LogTraceError(g_Log, "Malloc memory failed. [%d]\n",
            (iCnt * sizeof(TRANSLATE_TABLE)));
        return -ERR_TRANSLATE_MALLOC;
    }
    
    memset(g_pastTrTbl, 0, (iCnt * sizeof(TRANSLATE_TABLE)));
    g_iTrTblSize = 0;

    LogTraceDebug(g_Log, "Initiate iState. [%d]\n", iState);
    
    /* 分解出代码和名称 */
    for (OSP_S32 i = 0; i < v_iSize; i++)
    {
        switch (iState)
        {
            case STATE_CHANGE:
            {
                if (v_pszContent[i] == '\0')
                { // 继续检查下一个是否是结束符
                    break;
                }
                
                // 有内容了, 应该是股票代码, 不能加break
                iState = STATE_CODE;
                iIdx = 0;
                iTooLong = 0;
                pcStart = &v_pszContent[i];
                memset(g_pastTrTbl + g_iTrTblSize, 0,
                    sizeof(TRANSLATE_TABLE));
                LogTraceDebug(g_Log, "Current format changed. [%d]\n", iState);
            }
            
            case STATE_CODE:
            {
                if (v_pszContent[i] == ',')
                { // 紧接着就是股票名称
                    if (iIdx == 0)
                    { // 还没有获取到股票代码呢
                        iState = STATE_ERROR;
                        LogTraceDebug(g_Log, "Current format changed. [%d]\n",
                            iState);
                        break;
                    }

                    // 获取到了股票代码
                    iState = STATE_NAME;
                    iIdx = 0;
                    iTooLong = 0;
                    pcStart = &v_pszContent[i+1];
                    LogTraceDebug(g_Log, "Current format changed. [%d]\n",
                        iState);
                    break;
                }
                else if (v_pszContent[i] == '\0')
                { // 还没获取到股票名称呢, 就结束了
                    iIdx = 0;
                    memset(g_pastTrTbl + g_iTrTblSize, 0,
                        sizeof(TRANSLATE_TABLE));
                    LogTraceDebug(g_Log, "Current format changed. [%d]\n",
                        iState);
                    break;
                }
                else if (iIdx < STOCK_CODE_REAL_LEN)
                { // 填充股票代码
                    g_pastTrTbl[g_iTrTblSize].szCode[iIdx++] = v_pszContent[i];
                }
                else if (0 == iTooLong)
                { // 超过股票代码长度, 就截掉
                    iTooLong = 1;
                    LogTraceWarning(g_Log, "The code is too long. [%s, %d]\n",
                        pcStart, STOCK_CODE_REAL_LEN);
                }
                
                break;
            }
            
            case STATE_NAME:
            {
                if (v_pszContent[i] == '\0')
                { // 字符串结束了
                    if (iIdx == 0)
                    { // 还没获取到股票名称呢, 当前记录无效
                        iState = STATE_CHANGE;
                        LogTraceDebug(g_Log, "Current format changed. [%d]\n",
                            iState);
                        break;
                    }

                    // 获取到了股票名称, 当前记录有效
                    LogTraceDebug(g_Log, "Get one record. [%s, %s, %d]\n",
                        g_pastTrTbl[g_iTrTblSize].szCode,
                        g_pastTrTbl[g_iTrTblSize].szName,
                        g_iTrTblSize);
                    
                    g_iTrTblSize++;
                    iState = STATE_CHANGE;
                    LogTraceDebug(g_Log, "Current format changed. [%d]\n",
                        iState);
                    
                    break;
                }
                else if (iIdx < STOCK_NAME_REAL_LEN)
                { // 填充股票名称
                    g_pastTrTbl[g_iTrTblSize].szName[iIdx++] = v_pszContent[i];
                }
                else if (0 == iTooLong)
                { // 超过股票名称长度, 就截掉
                    iTooLong = 1;
                    LogTraceWarning(g_Log, "The name is too long. [%s, %d]\n",
                        pcStart, STOCK_NAME_REAL_LEN);
                }
                
                break;
            }
            
            case STATE_ERROR:
            {
                if (v_pszContent[i] != '\0')
                { // 继续检查下一个是否是结束符
                    break;
                }
                
                // 找到结束符了
                iState = STATE_CHANGE;
                LogTraceDebug(g_Log, "Current format changed. [%d]\n", iState);
                
                break;
            }
            
            default:
            {
                LogTraceEmerg(g_Log, "State chaos, Please check code.\n");
                break;
            }
        }
    }

    if ((iState == STATE_NAME) && (iIdx != 0))
    { // 处于获取名称的状态, 并且获取到了名称, 说明最后一条记录可能没有结束符
        // 获取到了股票名称, 当前记录有效
        LogTraceDebug(g_Log, "Get one record. [%s, %s, %d]\n",
            g_pastTrTbl[g_iTrTblSize].szCode, 
            g_pastTrTbl[g_iTrTblSize].szName,
            g_iTrTblSize);
        
        g_iTrTblSize++;
    }

    LogTraceDebug(g_Log, "=========================================\n");
    for (OSP_S32 i = 0; i < g_iTrTblSize; i++)
    {
        LogTraceDebug(g_Log, "The record. [%d, %s, %s]\n", i,
            g_pastTrTbl[i].szCode, 
            g_pastTrTbl[i].szName);
    }
    
    LogTraceDebug(g_Log, "=========================================\n");
    
    if (iCnt != g_iTrTblSize)
    {
        LogTraceEmerg(g_Log, "The record count chaos. [%d, %d]\n",
            iCnt, g_iTrTblSize);
    }
    else
    {
        LogTraceDebug(g_Log, "The record got ok. [%d, %d]\n",
            iCnt, g_iTrTblSize);
    }
    
    LogTraceDebug(g_Log, "=========================================\n");
    
    return SUCCESS;
} /* End of FormatTable */

/******************************************************************************
函数名称: TranslateInit
功能说明: 无
输入参数: 无
输出参数: 无
返 回 值: 无
说    明: 无
******************************************************************************/
OSP_S32 TranslateInit(const OSP_S8 *v_pszCodeTableName)
{
    FILE *pstF = NULL;
    OSP_S32 iFileSize = 0;
    OSP_S32 iRead = 0;
    OSP_S8 *pszContent = NULL;

    if (NULL != g_pastTrTbl)
    {
        LogTraceEmerg(g_Log, "Translate has already initialized. [%p, %d]\n",
            g_pastTrTbl, g_iTrTblSize);
        return SUCCESS;
    }

    /* 如果未分配内存，那么大小一定是0 */
    assert(0 == g_iTrTblSize);

    pstF = fopen(v_pszCodeTableName, "rb");
    if (NULL == pstF)
    {
        LogTraceError(g_Log, "Open code table file failed. [%s]\n",
            v_pszCodeTableName);
        return -ERR_TRANSLATE_FOPEN;
    }

    if (0 != fseek(pstF, 0, SEEK_END))
    {
        fclose(pstF);
        LogTraceError(g_Log, "Set code table file position failed. [%s]\n", 
            v_pszCodeTableName);
        return -ERR_TRANSLATE_FSEEK;
    }

    iFileSize = ftell(pstF);
    LogTraceDebug(g_Log, "The code table file size is got. [%s, %d]\n", 
        v_pszCodeTableName, iFileSize);

    pszContent = (OSP_S8 *)malloc(iFileSize);
    if (NULL == pszContent)
    {
        fclose(pstF);
        LogTraceError(g_Log, "Malloc memory failed. [%s, %d]\n",
            v_pszCodeTableName, iFileSize);
        return -ERR_TRANSLATE_MALLOC;
    }
    
    if (0 != fseek(pstF, 0, SEEK_SET))
    {
        free(pszContent);
        fclose(pstF);
        LogTraceError(g_Log, "Set code table file position failed. [%s]\n", 
            v_pszCodeTableName);
        return -ERR_TRANSLATE_FSEEK;
    }

    iRead = fread(pszContent, 1, iFileSize, pstF);
    if (iRead != iFileSize)
    {
        free(pszContent);
        fclose(pstF);
        LogTraceError(g_Log, "Read file failed. [%s, %d, %d]\n", 
            v_pszCodeTableName, iFileSize, iRead);
        return -ERR_TRANSLATE_FREAD;
    }

    fclose(pstF);
    
    if (SUCCESS != FormatTable(pszContent, iFileSize))
    {
        free(pszContent);
        return -ERR_TRANSLATE_FORMAT_TBL;
    }

    free(pszContent);
    //_CrtDumpMemoryLeaks();
    
    return SUCCESS;
} /* End of TranslateInit */

/******************************************************************************
函数名称: TranslateCode
功能说明: 无
输入参数: 无
输出参数: 无
返 回 值: 无
说    明: 无
******************************************************************************/
OSP_S32 TranslateCode(const OSP_S8 *v_pszCode, OSP_S8 *v_pszName, OSP_S32 v_iNameLen)
{
    assert(NULL != v_pszCode);
    assert(NULL != v_pszName);
    assert(0 != v_iNameLen);
    assert(NULL != g_pastTrTbl);
    assert(0 != g_iTrTblSize);

    for (OSP_S32 i = 0; i < g_iTrTblSize; i++)
    {
        if (0 == strnicmp(g_pastTrTbl[i].szCode, v_pszCode, STOCK_CODE_REAL_LEN))
        {
            _snprintf(v_pszName, v_iNameLen, "%s", g_pastTrTbl[i].szName);
            return SUCCESS;
        }
    }
    
    return -ERR_TRANSLATE_CODE;
} /* End of TranslateCode */

/******************************************************************************
函数名称: TranslateName
功能说明: 无
输入参数: 无
输出参数: 无
返 回 值: 无
说    明: 无
******************************************************************************/
OSP_S32 TranslateName(const OSP_S8 *v_pszName, OSP_S8 *v_pszCode, OSP_S32 v_iCodeLen)
{
    assert(NULL != v_pszName);
    assert(NULL != v_pszCode);
    assert(0 != v_iCodeLen);
    assert(NULL != g_pastTrTbl);
    assert(0 != g_iTrTblSize);

    for (OSP_S32 i = 0; i < g_iTrTblSize; i++)
    {
        if (0 == strnicmp(g_pastTrTbl[i].szName, v_pszName, v_iCodeLen))
        {
            _snprintf(v_pszCode, v_iCodeLen, "%s", g_pastTrTbl[i].szCode);
            return SUCCESS;
        }
    }
    
    return -ERR_TRANSLATE_NAME;
} /* End of TranslateName */

/******************************************************************************
函数名称: TranslateEnd
功能说明: 无
输入参数: 无
输出参数: 无
返 回 值: 无
说    明: 无
******************************************************************************/
OSP_VOID TranslateEnd(OSP_VOID)
{
    if (NULL == g_pastTrTbl)
    {
        return;
    }

    /* 已经分配了内存的话, 那么大小不会为0 */
    assert(0 != g_iTrTblSize);

    free(g_pastTrTbl);
    g_pastTrTbl = NULL;
    g_iTrTblSize = 0;
} /* End of TranslateEnd */


#define __DEBUG_STOCK_TRANSLATE__

#ifdef __DEBUG_STOCK_TRANSLATE__

//#define DEBUG_CODE_TABLE   "data\\TestTable.txt"
#define DEBUG_CODE_TABLE   "data\\CodeTable.txt"
#define DEBUG_STOCK_CODE1   "600578"
#define DEBUG_STOCK_CODE2   "000789"
#define DEBUG_STOCK_NAME1   "宝钢CWB1"
#define DEBUG_STOCK_NAME2   "连云港"

/******************************************************************************
函数名称: DebugStockTranslate
功能说明: 无
输入参数: 无
输出参数: 无
返 回 值: 无
说    明: 无
******************************************************************************/
OSP_VOID DebugStockTranslate(OSP_VOID)
{
    OSP_S8 szName[STOCK_NAME_LEN];
    OSP_S8 szCode[STOCK_CODE_LEN];
    
    if (SUCCESS != TranslateInit(DEBUG_CODE_TABLE))
    {
        printf("Init failed. [%s]\n", DEBUG_CODE_TABLE);
        return;
    }

    if (SUCCESS != TranslateCode(DEBUG_STOCK_CODE1, szName, STOCK_NAME_LEN))
    {
        printf("Translate code failed. [%s]\n", DEBUG_STOCK_CODE1);
    }
    else
    {
        printf("Translate code success. [%s, %s]\n", DEBUG_STOCK_CODE1, szName);
    }

    if (SUCCESS != TranslateCode(DEBUG_STOCK_CODE2, szName, STOCK_NAME_LEN))
    {
        printf("Translate code failed. [%s]\n", DEBUG_STOCK_CODE2);
    }
    else
    {
        printf("Translate code success. [%s, %s]\n", DEBUG_STOCK_CODE2, szName);
    }

    if (SUCCESS != TranslateName(DEBUG_STOCK_NAME1, szCode, STOCK_CODE_LEN))
    {
        printf("Translate name failed. [%s]\n", DEBUG_STOCK_NAME1);
    }
    else
    {
        printf("Translate name success. [%s, %s]\n", DEBUG_STOCK_NAME1, szCode);
    }
    
    if (SUCCESS != TranslateName(DEBUG_STOCK_NAME2, szCode, STOCK_CODE_LEN))
    {
        printf("Translate name failed. [%s]\n", DEBUG_STOCK_NAME2);
    }
    else
    {
        printf("Translate name success. [%s, %s]\n", DEBUG_STOCK_NAME2, szCode);
    }

    TranslateEnd();
} /* End of DebugStockTranslate */

#endif /* End of __DEBUG_STOCK_TRANSLATE__ */


