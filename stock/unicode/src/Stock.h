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

#define STOCK_PATH_LEN   256

// ���һ���ַ��ǽ�����
#define STOCK_NAME_LEN   (12 + 1)
#define STOCK_CODE_LEN   (6 + 1)

#define STOCK_CODE_TABLE   "data\\CodeTable.txt"
#define STOCK_WATCH_DATA   "data\\WatchData.txt"
#define STOCK_CHIP_DATA    "data\\ChipData.txt"

#define STOCK_CONFIG_FILE  "config\\config.ini"

#define STOCK_BUY_CHIP     "config\\ChipBuy.tcl"
#define STOCK_SELL_CHIP    "config\\ChipSell.tcl"
#define STOCK_LOGIN_CHIP   "config\\ChipLogin.tcl"
#define STOCK_FLUSH_CHIP   "config\\ChipFlush.tcl"

#define STOCK_FLUSH_WATCH  "config\\WatchFlush.tcl"
#define STOCK_LOGIN_WATCH  "config\\WatchLogin.tcl"

/* ������붨�� */
#define ERR_INVALID_PARA   -1

/* STOCK_INFO_S�ṹ����iBuy��Ա��ֵ: �������� */
#define CHIP_SELL    0
#define CHIP_BUY     1

typedef struct tagSTOCK_INFO_S
{
    int   iBuy;                    /* ��������, 0: ��, ����: �� */
    char  szName[STOCK_NAME_LEN];  /* ��Ʊ���� */
    char  szCode[STOCK_CODE_LEN];  /* ��Ʊ���� */
    int   iNum;                    /* ��Ʊ��Ŀ */
    float fPrice;                  /* ��Ʊ�۸� */
} STOCK_INFO_S; /* End of STOCK_INFO_S */

/* STOCK_CONFIG_S�ṹ����iWatchMethod��Ա��ֵ: ��ȡ����Ԥ����Ʊ��Ϣ */
#define WATCH_CODE  0
#define WATCH_NAME  1
#define WATCH_BOTH  2

typedef struct tagSTOCK_CONFIG_S
{
    int  iWatchMethod; /* Ԥ����ʽ */
    char szWatchPathName[STOCK_PATH_LEN];     /* Ԥ������·�� */
    char szWatchMainWndName[STOCK_PATH_LEN];  /* Ԥ������������ */
    char szWatchInfoWndName[STOCK_PATH_LEN];  /* Ԥ����Ϣ�������� */
    
    char szChipPathName[STOCK_PATH_LEN];      /* �µ�����·�� */
    char szChipWndName[STOCK_PATH_LEN];       /* �µ��������� */
} STOCK_CONFIG_S; /* End of STOCK_CONFIG_S */

extern int TranslateInit(const char *pszCodeTableName);
extern int TranslateCode(const char *pszCode, char *pszName, int iNameLen);
extern int TranslateName(const char *pszName, char *pszCode, int iCodeLen);
extern void TranslateEnd(void);

extern int KeepAliveWatch(void);
extern int KeepAliveChip(void);

extern int WatchInit(void);
extern int WatchGetResult(STOCK_INFO_S *pstSI);
extern int WatchGetCount(int *piCnt);
extern void WatchEnd(void);

extern int ChipBuy(STOCK_INFO_S *pstSI);
extern int ChipSell(STOCK_INFO_S *pstSI);

#endif /* End of __STOCK_H__ */
