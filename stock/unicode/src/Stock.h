/******************************************************************************
版权所有, 2010~2013, 曾华荣 (zeng_hr@163.com)
文件名: STOCK.H
作者: 曾华荣 (zeng_hr@163.com)  日期: 2010年5月6日
功能描述: 
函数列表: 
  1. ...: 
  
修改历史: 
  作者: 曾华荣 (zeng_hr@163.com)  日期: 2010年5月6日
  -----------------------------------------------------------------------------
  初始版本
******************************************************************************/
#ifndef __STOCK_H__
#define __STOCK_H__

#define STOCK_PATH_LEN   256

// 最后一个字符是结束符
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

/* 错误代码定义 */
#define ERR_INVALID_PARA   -1

/* STOCK_INFO_S结构体中iBuy成员的值: 买卖方向 */
#define CHIP_SELL    0
#define CHIP_BUY     1

typedef struct tagSTOCK_INFO_S
{
    int   iBuy;                    /* 买卖方向, 0: 卖, 其他: 买 */
    char  szName[STOCK_NAME_LEN];  /* 股票名称 */
    char  szCode[STOCK_CODE_LEN];  /* 股票代码 */
    int   iNum;                    /* 股票数目 */
    float fPrice;                  /* 股票价格 */
} STOCK_INFO_S; /* End of STOCK_INFO_S */

/* STOCK_CONFIG_S结构体中iWatchMethod成员的值: 获取到的预警股票信息 */
#define WATCH_CODE  0
#define WATCH_NAME  1
#define WATCH_BOTH  2

typedef struct tagSTOCK_CONFIG_S
{
    int  iWatchMethod; /* 预警方式 */
    char szWatchPathName[STOCK_PATH_LEN];     /* 预警程序路径 */
    char szWatchMainWndName[STOCK_PATH_LEN];  /* 预警主窗口名称 */
    char szWatchInfoWndName[STOCK_PATH_LEN];  /* 预警信息窗口命名 */
    
    char szChipPathName[STOCK_PATH_LEN];      /* 下单程序路径 */
    char szChipWndName[STOCK_PATH_LEN];       /* 下单窗口名称 */
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
