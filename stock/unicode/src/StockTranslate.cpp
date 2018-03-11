/******************************************************************************
��Ȩ����, 2010~2013, ������ (zeng_hr@163.com)
�ļ���: STOCKTRANSLATE.CPP
����: ������ (zeng_hr@163.com)  ����: 2010��5��6��
��������: 
�����б�: 
  1. ...: 
  
�޸���ʷ: 
  ����: ������ (zeng_hr@163.com)  ����: 2010��5��6��
  -----------------------------------------------------------------------------
  ��ʼ�汾
******************************************************************************/
#define   _CRTDBG_MAP_ALLOC   
#include <stdlib.h>   
#include <crtdbg.h>

#include <stdio.h>

#include "Stock.h"
#include "Log.h"

#define STOCK_NAME_REAL_LEN   (STOCK_NAME_LEN - 1)
#define STOCK_CODE_REAL_LEN   (STOCK_CODE_LEN - 1)

#define STATE_CHANGE  0
#define STATE_CODE    1 // ������Ʊ����״̬
#define STATE_NAME    2 // ������Ʊ����״̬
#define STATE_ERROR   3 // ��������, ���ҵ���һ���ַ����Ŀ�ʼ

typedef struct tagTRANSLATE_TABLE
{
    char szName[STOCK_NAME_LEN];  /* ��Ʊ���� */
    char szCode[STOCK_CODE_LEN];  /* ��Ʊ���� */
} TRANSLATE_TABLE; /* End of TRANSLATE_TABLE */

static TRANSLATE_TABLE *g_pastTrTbl = NULL;
static int g_iTrTblSize = 0;

/******************************************************************************
��������: GetRecordCnt
����˵��: ��
�������: ��
�������: ��
�� �� ֵ: ��
˵    ��: ��
******************************************************************************/
int GetRecordCnt(char *pcContent, int iSize)
{
    int i = 0;
    int state = STATE_CHANGE;
    int iIdx = 0;
    int cnt = 0;
    
    /* �����ܹ��ж�����Ч��¼ */
    for (i = 0; i < iSize; i++)
    {
        switch (state)
        {
            case STATE_CHANGE:
            {
                if (pcContent[i] == '\0')
                { // ���������һ���Ƿ��ǽ�����
                    break;
                }
                
                // ��������, Ӧ���ǹ�Ʊ����, ���ܼ�break
                iIdx = 0;
                state = STATE_CODE;
                LogTraceDebug(g_Log, "Current format changed. [%d]\n", state);
            }
            
            case STATE_CODE:
            {
                if (pcContent[i] == ',')
                { // �����ž��ǹ�Ʊ����
                    if (iIdx == 0)
                    { // ��û�л�ȡ����Ʊ������
                        state = STATE_ERROR;
                        LogTraceDebug(g_Log, "Current format changed. [%d]\n",
                            state);
                        break;
                    }

                    // ��ȡ���˹�Ʊ����
                    iIdx = 0;
                    state = STATE_NAME;
                    LogTraceDebug(g_Log, "Current format changed. [%d]\n",
                        state);
                    break;
                }
                else if (pcContent[i] == '\0')
                { // ��û��ȡ����Ʊ������, �ͽ�����
                    iIdx = 0;
                    LogTraceDebug(g_Log, "Current format changed. [%d]\n",
                        state);
                    break;
                }
                else
                { // ����Ʊ����
                    iIdx++;
                }
                break;
            }
            
            case STATE_NAME:
            {
                if (pcContent[i] == '\0')
                { // �ַ���������
                    if (iIdx == 0)
                    { // ��û��ȡ����Ʊ������, ��ǰ��¼��Ч
                        state = STATE_CHANGE;
                        LogTraceDebug(g_Log, "Current format changed. [%d]\n",
                            state);
                        break;
                    }

                    cnt++;
                    state = STATE_CHANGE;
                    LogTraceDebug(g_Log, "Current format changed. [%d]\n",
                        state);
                    break;
                }
                else
                { // ����Ʊ����
                    iIdx++;
                }
                break;
            }
            
            case STATE_ERROR:
            {
                if (pcContent[i] != '\0')
                { // ���������һ���Ƿ��ǽ�����
                    break;
                }
                
                // �ҵ���������
                state = STATE_CHANGE;
                LogTraceDebug(g_Log, "Current format changed. [%d]\n", state);
                break;
            }
            
            default:
            {
                LogTraceEmerg(g_Log, "State chaos, Please check code.\n");
                break;
            }
        }
    }

    if ((state == STATE_NAME) && (iIdx != 0))
    { // ���ڻ�ȡ���Ƶ�״̬, ���һ�ȡ��������, ˵�����һ����¼����û�н�����
        cnt++;
    }

    return cnt;
} /* End of GetRecordCnt */

/******************************************************************************
��������: FormatTable
����˵��: ��
�������: ��
�������: ��
�� �� ֵ: ��
˵    ��: ��
******************************************************************************/
int FormatTable(char *pcContent, int iSize)
{
    int i = 0;
    int state = STATE_CHANGE;
    int iIdx = 0;
    int iTooLong = 0;
    char *pcStart;
    int cnt = 0;
    
    /* �����еĻ��з�ת���ɽ����� */
    for (i = 0; i < iSize; i++)
    {
        if ((pcContent[i] == 0x0D) || (pcContent[i] == 0x0A))
        {
            pcContent[i] = 0x00;
        }
    }

    cnt = GetRecordCnt(pcContent, iSize);
    if (0 == cnt)
    { // �޼�¼
        LogTraceError(g_Log, "There are no record in code table file.\n");
        return -1;
    }
    
    g_pastTrTbl = (TRANSLATE_TABLE *)malloc(cnt * sizeof(TRANSLATE_TABLE));
    if (NULL == g_pastTrTbl)
    {
        LogTraceError(g_Log, "Malloc memory failed. [%d]\n",
            (cnt * sizeof(TRANSLATE_TABLE)));
        return -2;
    }
    
    memset(g_pastTrTbl, 0, (cnt * sizeof(TRANSLATE_TABLE)));
    g_iTrTblSize = 0;

    LogTraceDebug(g_Log, "Initiate state. [%d]\n", state);
    
    /* �ֽ����������� */
    for (i = 0; i < iSize; i++)
    {
        switch (state)
        {
            case STATE_CHANGE:
            {
                if (pcContent[i] == '\0')
                { // ���������һ���Ƿ��ǽ�����
                    break;
                }
                
                // ��������, Ӧ���ǹ�Ʊ����, ���ܼ�break
                state = STATE_CODE;
                iIdx = 0;
                iTooLong = 0;
                pcStart = &pcContent[i];
                memset(g_pastTrTbl + g_iTrTblSize, 0,
                    sizeof(TRANSLATE_TABLE));
                LogTraceDebug(g_Log, "Current format changed. [%d]\n", state);
            }
            
            case STATE_CODE:
            {
                if (pcContent[i] == ',')
                { // �����ž��ǹ�Ʊ����
                    if (iIdx == 0)
                    { // ��û�л�ȡ����Ʊ������
                        state = STATE_ERROR;
                        LogTraceDebug(g_Log, "Current format changed. [%d]\n",
                            state);
                        break;
                    }

                    // ��ȡ���˹�Ʊ����
                    state = STATE_NAME;
                    iIdx = 0;
                    iTooLong = 0;
                    pcStart = &pcContent[i+1];
                    LogTraceDebug(g_Log, "Current format changed. [%d]\n",
                        state);
                    break;
                }
                else if (pcContent[i] == '\0')
                { // ��û��ȡ����Ʊ������, �ͽ�����
                    iIdx = 0;
                    memset(g_pastTrTbl + g_iTrTblSize, 0,
                        sizeof(TRANSLATE_TABLE));
                    LogTraceDebug(g_Log, "Current format changed. [%d]\n",
                        state);
                    break;
                }
                else if (iIdx < STOCK_CODE_REAL_LEN)
                { // ����Ʊ����
                    g_pastTrTbl[g_iTrTblSize].szCode[iIdx++] = pcContent[i];
                }
                else if (0 == iTooLong)
                { // ������Ʊ���볤��, �ͽص�
                    iTooLong = 1;
                    LogTraceWarning(g_Log, "The code is too long. [%s, %d]\n",
                        pcStart, STOCK_CODE_REAL_LEN);
                }
                break;
            }
            
            case STATE_NAME:
            {
                if (pcContent[i] == '\0')
                { // �ַ���������
                    if (iIdx == 0)
                    { // ��û��ȡ����Ʊ������, ��ǰ��¼��Ч
                        state = STATE_CHANGE;
                        LogTraceDebug(g_Log, "Current format changed. [%d]\n",
                            state);
                        break;
                    }

                    // ��ȡ���˹�Ʊ����, ��ǰ��¼��Ч
                    LogTraceDebug(g_Log, "Get one record. [%s, %s]\n",
                        g_pastTrTbl[g_iTrTblSize].szCode,
                        g_pastTrTbl[g_iTrTblSize].szName);
                    
                    g_iTrTblSize++;
                    state = STATE_CHANGE;
                    LogTraceDebug(g_Log, "Current format changed. [%d]\n",
                        state);
                    break;
                }
                else if (iIdx < STOCK_NAME_REAL_LEN)
                { // ����Ʊ����
                    g_pastTrTbl[g_iTrTblSize].szName[iIdx++] = pcContent[i];
                }
                else if (0 == iTooLong)
                { // ������Ʊ���Ƴ���, �ͽص�
                    iTooLong = 1;
                    LogTraceWarning(g_Log, "The name is too long. [%s, %d]\n",
                        pcStart, STOCK_NAME_REAL_LEN);
                }
                break;
            }
            
            case STATE_ERROR:
            {
                if (pcContent[i] != '\0')
                { // ���������һ���Ƿ��ǽ�����
                    break;
                }
                
                // �ҵ���������
                state = STATE_CHANGE;
                LogTraceDebug(g_Log, "Current format changed. [%d]\n", state);
                break;
            }
            
            default:
            {
                LogTraceEmerg(g_Log, "State chaos, Please check code.\n");
                break;
            }
        }
    }

    if ((state == STATE_NAME) && (iIdx != 0))
    { // ���ڻ�ȡ���Ƶ�״̬, ���һ�ȡ��������, ˵�����һ����¼����û�н�����
        // ��ȡ���˹�Ʊ����, ��ǰ��¼��Ч
        LogTraceDebug(g_Log, "Get one record. [%s, %s]\n",
            g_pastTrTbl[g_iTrTblSize].szCode, 
            g_pastTrTbl[g_iTrTblSize].szName);
        
        g_iTrTblSize++;
    }

    LogTraceDebug(g_Log, "=========================================\n");
    for (i = 0; i < g_iTrTblSize; i++)
    {
        LogTraceDebug(g_Log, "The record. [%d, %s, %s]\n", i,
            g_pastTrTbl[i].szCode, 
            g_pastTrTbl[i].szName);
    }
    
    LogTraceDebug(g_Log, "=========================================\n");
    
    if (cnt != g_iTrTblSize)
    {
        LogTraceEmerg(g_Log, "The record count chaos. [%d, %d]\n",
            cnt, g_iTrTblSize);
    }
    else
    {
        LogTraceDebug(g_Log, "The record got ok. [%d, %d]\n",
            cnt, g_iTrTblSize);
    }
    
    LogTraceDebug(g_Log, "=========================================\n");
    
    return 0;
} /* End of FormatTable */

/******************************************************************************
��������: TranslateInit
����˵��: ��
�������: ��
�������: ��
�� �� ֵ: ��
˵    ��: ��
******************************************************************************/
int TranslateInit(const char *pszCodeTableName)
{
    FILE *pstF = NULL;
    int iFileSize = 0;
    int iRead = 0;
    char *pcContent = NULL;

    pstF = fopen(pszCodeTableName, "rb");
    if (NULL == pstF)
    {
        LogTraceError(g_Log, "Open code table file failed. [%s]\n",
            pszCodeTableName);
        return -1;
    }

    if (0 != fseek(pstF, 0, SEEK_END))
    {
        fclose(pstF);
        LogTraceError(g_Log, "Set code table file position failed. [%s]\n", 
            pszCodeTableName);
        return -2;
    }

    iFileSize = ftell(pstF);
    LogTraceDebug(g_Log, "The code table file size is got. [%s, %d]\n", 
        pszCodeTableName, iFileSize);

    pcContent = (char *)malloc(iFileSize);
    if (NULL == pcContent)
    {
        fclose(pstF);
        LogTraceError(g_Log, "Malloc memory failed. [%s, %d]\n",
            pszCodeTableName, iFileSize);
        return -3;
    }
    
    if (0 != fseek(pstF, 0, SEEK_SET))
    {
        free(pcContent);
        fclose(pstF);
        LogTraceError(g_Log, "Set code table file position failed. [%s]\n", 
            pszCodeTableName);
        return -5;
    }

    iRead = fread(pcContent, 1, iFileSize, pstF);
    if (iRead != iFileSize)
    {
        free(pcContent);
        fclose(pstF);
        LogTraceError(g_Log, "Read file failed. [%s, %d, %d]\n", 
            pszCodeTableName, iFileSize, iRead);
        return -6;
    }

    fclose(pstF);
    
    if (0 != FormatTable(pcContent, iFileSize))
    {
        free(pcContent);
        return -7;
    }

    _CrtDumpMemoryLeaks();
    free(pcContent);
    
    return 0;
} /* End of TranslateInit */

/******************************************************************************
��������: TranslateCode
����˵��: ��
�������: ��
�������: ��
�� �� ֵ: ��
˵    ��: ��
******************************************************************************/
int TranslateCode(const char *pszCode, char *pszName, int iNameLen)
{
    int i = 0;
    
    assert(NULL != pszCode);
    assert(NULL != pszName);
    assert(0 != iNameLen);
    assert(NULL != g_pastTrTbl);
    assert(0 != g_iTrTblSize);

    for (i = 0; i < g_iTrTblSize; i++)
    {
        if (0 == strnicmp(g_pastTrTbl[i].szCode, pszCode, STOCK_CODE_REAL_LEN))
        {
            _snprintf(pszName, iNameLen, "%s", g_pastTrTbl[i].szName);
            return 0;
        }
    }
    
    return -1;
} /* End of TranslateCode */

/******************************************************************************
��������: TranslateName
����˵��: ��
�������: ��
�������: ��
�� �� ֵ: ��
˵    ��: ��
******************************************************************************/
int TranslateName(const char *pszName, char *pszCode, int iCodeLen)
{
    int i = 0;
    
    assert(NULL != pszName);
    assert(NULL != pszCode);
    assert(0 != iCodeLen);
    assert(NULL != g_pastTrTbl);
    assert(0 != g_iTrTblSize);

    for (i = 0; i < g_iTrTblSize; i++)
    {
        if (0 == strnicmp(g_pastTrTbl[i].szName, pszName, iCodeLen))
        {
            _snprintf(pszCode, iCodeLen, "%s", g_pastTrTbl[i].szCode);
            return 0;
        }
    }
    
    return -1;
} /* End of TranslateName */

/******************************************************************************
��������: TranslateEnd
����˵��: ��
�������: ��
�������: ��
�� �� ֵ: ��
˵    ��: ��
******************************************************************************/
void TranslateEnd(void)
{
    if (NULL == g_pastTrTbl)
    {
        return;
    }

    /* �Ѿ��������ڴ�Ļ�, ��ô��С����Ϊ0 */
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
#define DEBUG_STOCK_NAME1   "����CWB1"
#define DEBUG_STOCK_NAME2   "���Ƹ�"

/******************************************************************************
��������: DebugStockTranslate
����˵��: ��
�������: ��
�������: ��
�� �� ֵ: ��
˵    ��: ��
******************************************************************************/
void DebugStockTranslate(void)
{
    char szName[STOCK_NAME_LEN];
    char szCode[STOCK_CODE_LEN];
    
    if (0 != TranslateInit(DEBUG_CODE_TABLE))
    {
        printf("Init failed. [%s]\n", DEBUG_CODE_TABLE);
        return;
    }

    if (0 != TranslateCode(DEBUG_STOCK_CODE1, szName, STOCK_NAME_LEN))
    {
        printf("Translate code failed. [%s]\n", DEBUG_STOCK_CODE1);
    }
    else
    {
        printf("Translate code success. [%s, %s]\n", DEBUG_STOCK_CODE1, szName);
    }

    if (0 != TranslateCode(DEBUG_STOCK_CODE2, szName, STOCK_NAME_LEN))
    {
        printf("Translate code failed. [%s]\n", DEBUG_STOCK_CODE2);
    }
    else
    {
        printf("Translate code success. [%s, %s]\n", DEBUG_STOCK_CODE2, szName);
    }

    if (0 != TranslateName(DEBUG_STOCK_NAME1, szCode, STOCK_CODE_LEN))
    {
        printf("Translate name failed. [%s]\n", DEBUG_STOCK_NAME1);
    }
    else
    {
        printf("Translate name success. [%s, %s]\n", DEBUG_STOCK_NAME1, szCode);
    }
    
    if (0 != TranslateName(DEBUG_STOCK_NAME2, szCode, STOCK_CODE_LEN))
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


