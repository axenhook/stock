/******************************************************************************
��Ȩ����, 2010~2013, ������ (zeng_hr@163.com)
�ļ���: STOCK.H
����: ������ (zeng_hr@163.com)  ����: 2010��5��6��
��������: 
�����б�: 
  1. ...: 
  
�޸���ʷ: 
  ����: ������ (zeng_hr@163.com)  ����: 2010��5��6��
  -----------------------------------------------------------------------------
  ��ʼ�汾
******************************************************************************/
#ifndef __STOCK_H__
#define __STOCK_H__

#define STOCK_MAX_LEN    (256)
#define STOCK_TIME_LEN   (128)

#define INI_BUF_SIZE     (128)
#define STOCK_CMD_LEN    (1024)

// ���һ���ַ��ǽ�����
#define STOCK_NAME_LEN   (12 + 1)
#define STOCK_CODE_LEN   (6 + 1)

#define STOCK_CODE_TABLE   "config\\CodeTable.txt"
#define STOCK_WATCH_DATA   "config\\WatchData.txt"
#define STOCK_CHIP_DATA    "config\\ChipData.txt"

#define STOCK_CONFIG_FILE  "config\\config.ini"

#define STOCK_BUY_CHIP     "config\\ChipBuy.script"
#define STOCK_SELL_CHIP    "config\\ChipSell.script"

#define STOCK_FLUSH_CHIP   "config\\ChipKeepAlive.script"
#define STOCK_FLUSH_WATCH  "config\\WatchKeepAlive.script"

/* STOCK_INFO_S�ṹ����iBuy��Ա��ֵ: �������� */
#define CHIP_SELL    0
#define CHIP_BUY     1

typedef struct tagSTOCK_INFO_S
{
    OSP_S32 iBuy;                    /* ��������, 0: ��, ����: �� */
    OSP_S8  szName[STOCK_NAME_LEN];  /* ��Ʊ���� */
    OSP_S8  szCode[STOCK_CODE_LEN];  /* ��Ʊ���� */
    OSP_S32 iNum;                    /* ��Ʊ��Ŀ */
    float   fPrice;                  /* ��Ʊ�۸� */
    OSP_S8  szTime[STOCK_TIME_LEN];  /* ��ƱԤ��ʱ�� */
} STOCK_INFO_S; /* End of STOCK_INFO_S */

/* STOCK_CONFIG_S�ṹ����iWatchMethod��Ա��ֵ: ��ȡ����Ԥ����Ʊ��Ϣ */
#define WATCH_CODE  0
#define WATCH_NAME  1
#define WATCH_BOTH  2

#include <afx.h>

typedef struct tagSTOCK_CONFIG_S
{
    /* Ԥ����Ϣ */
    OSP_S32 iWatchMethod; /* Ԥ����ʽ */
    OSP_S8  szWatchPathName[STOCK_MAX_LEN];     /* Ԥ������·�� */
    OSP_S8  szWatchMainWndName[STOCK_MAX_LEN];  /* Ԥ������������ */
    OSP_S8  szWatchInfoWndName[STOCK_MAX_LEN];  /* Ԥ����Ϣ�������� */
    OSP_S8  szWatchInfoWndClass[STOCK_MAX_LEN]; /* Ԥ����Ϣ���������� */
    
    /* Ԥ���µ���Ϣ */
    OSP_S8  szChipPathName[STOCK_MAX_LEN];      /* �µ�����·�� */
    OSP_S8  szChipMainWndName[STOCK_MAX_LEN];   /* �µ����������� */
    
    /* ˢ��Ƶ�� */
    OSP_S32 iWatchFlushTime;  /* ��λ: ms */
    OSP_S32 iChipFlushTime;   /* ��λ: ms */
    
    /* �������δ��ʱ�ļ���� */
    OSP_S32 iCheckWndTime;   /* ��λ: ms */
    
    /*
    ; ���н��׵Ĵ���
    ; <0: ���޽��״���
    ; =0: ֻ����Ԥ��, �������µ�����
    ; >0: ���׵Ĵ���
    */
    OSP_S32 iChipTimes;
    
    /* ���й����е���Ϣ */
    HWND    hwndWatchMain;   /* Ԥ�������򴰿ھ�� */
    HWND    hwndWatchInfo;   /* Ԥ����Ϣ���ھ�� */
    HWND    hwndChipMain;    /* �µ����ھ�� */
} STOCK_CONFIG_S; /* End of STOCK_CONFIG_S */

extern STOCK_CONFIG_S g_stCfg;

extern OSP_S32 TranslateInit(const OSP_S8 *v_pszCodeTableName);
extern OSP_S32 TranslateCode(const OSP_S8 *v_pszCode, OSP_S8 *v_pszName, OSP_S32 v_iNameLen);
extern OSP_S32 TranslateName(const OSP_S8 *v_pszName, OSP_S8 *v_pszCode, OSP_S32 v_iCodeLen);
extern OSP_VOID TranslateEnd(OSP_VOID);

extern OSP_S32 KeepAliveWatch(HWND v_hwnd);
extern OSP_S32 KeepAliveChip(HWND v_hwnd);

extern OSP_S32 WatchInit(OSP_VOID);
extern OSP_S32 WatchGetResult(STOCK_INFO_S *v_pstSI, HWND v_hwnd);
extern OSP_VOID WatchEnd(OSP_VOID);

extern OSP_S32 ChipBuy(STOCK_INFO_S *v_pstSI, HWND v_hwnd);
extern OSP_S32 ChipSell(STOCK_INFO_S *v_pstSI, HWND v_hwnd);

#endif /* End of __STOCK_H__ */
