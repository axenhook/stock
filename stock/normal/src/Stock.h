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

#define STOCK_MAX_LEN    (256)
#define STOCK_TIME_LEN   (128)

#define INI_BUF_SIZE     (128)
#define STOCK_CMD_LEN    (1024)

// 最后一个字符是结束符
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

/* STOCK_INFO_S结构体中iBuy成员的值: 买卖方向 */
#define CHIP_SELL    0
#define CHIP_BUY     1

typedef struct tagSTOCK_INFO_S
{
    OSP_S32 iBuy;                    /* 买卖方向, 0: 卖, 其他: 买 */
    OSP_S8  szName[STOCK_NAME_LEN];  /* 股票名称 */
    OSP_S8  szCode[STOCK_CODE_LEN];  /* 股票代码 */
    OSP_S32 iNum;                    /* 股票数目 */
    float   fPrice;                  /* 股票价格 */
    OSP_S8  szTime[STOCK_TIME_LEN];  /* 股票预警时间 */
} STOCK_INFO_S; /* End of STOCK_INFO_S */

/* STOCK_CONFIG_S结构体中iWatchMethod成员的值: 获取到的预警股票信息 */
#define WATCH_CODE  0
#define WATCH_NAME  1
#define WATCH_BOTH  2

#include <afx.h>

typedef struct tagSTOCK_CONFIG_S
{
    /* 预警信息 */
    OSP_S32 iWatchMethod; /* 预警方式 */
    OSP_S8  szWatchPathName[STOCK_MAX_LEN];     /* 预警程序路径 */
    OSP_S8  szWatchMainWndName[STOCK_MAX_LEN];  /* 预警主窗口名称 */
    OSP_S8  szWatchInfoWndName[STOCK_MAX_LEN];  /* 预警信息窗口名称 */
    OSP_S8  szWatchInfoWndClass[STOCK_MAX_LEN]; /* 预警信息窗口类名称 */
    
    /* 预警下单信息 */
    OSP_S8  szChipPathName[STOCK_MAX_LEN];      /* 下单程序路径 */
    OSP_S8  szChipMainWndName[STOCK_MAX_LEN];   /* 下单主窗口名称 */
    
    /* 刷新频率 */
    OSP_S32 iWatchFlushTime;  /* 单位: ms */
    OSP_S32 iChipFlushTime;   /* 单位: ms */
    
    /* 当有软件未打开时的检测间隔 */
    OSP_S32 iCheckWndTime;   /* 单位: ms */
    
    /*
    ; 进行交易的次数
    ; <0: 不限交易次数
    ; =0: 只捕获预警, 不进行下单操作
    ; >0: 交易的次数
    */
    OSP_S32 iChipTimes;
    
    /* 运行过程中的信息 */
    HWND    hwndWatchMain;   /* 预警主程序窗口句柄 */
    HWND    hwndWatchInfo;   /* 预警信息窗口句柄 */
    HWND    hwndChipMain;    /* 下单窗口句柄 */
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
