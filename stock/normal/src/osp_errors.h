/*******************************************************************************

            版权所有(C), 2010~2013, 曾华荣 (EMail: zeng_hr@163.com)
********************************************************************************
文 件 名: OSP_ERRORS.H
作    者: 曾华荣 (EMail: zeng_hr@163.com)
版    本: V1.00
日    期: 2010年5月26日
功能描述: 
函数列表: 
    1. ...: 
修改历史: 
    作者: 曾华荣 (EMail: zeng_hr@163.com)    日期: 2010年5月26日
--------------------------------------------------------------------------------
    1. 初始版本
*******************************************************************************/
#ifndef __OSP_ERRORS_H__
#define __OSP_ERRORS_H__

/* 错误代码定义 */
typedef enum tagERROR_CODE_E
{
    ERR_STOCK_START = 2000,
    ERR_STOCK_INVALID_PARA,
    ERR_STOCK_FIND_WATCH_MAIN_WND,
    ERR_STOCK_FIND_WATCH_INFO_WND,
    ERR_STOCK_FIND_WATCH_INFO_CHILD_WND,
    ERR_STOCK_FIND_CHIP_MAIN_WND,
    ERR_STOCK_INIT_TRANSLATE,
    ERR_STOCK_INIT_WATCH,
    ERR_STOCK_TRANSLATE_CODE,
    ERR_STOCK_TRANSLATE_NAME,
    ERR_STOCK_INIT_CONFIG,
    ERR_STOCK_CHECK_CONFIG,
    ERR_STOCK_SELL_CHIPS,
    ERR_STOCK_BUY_CHIPS,
    ERR_STOCK_CHAOS_CHIPS,
    ERR_STOCK_INIT_FAILED,
    ERR_STOCK_READ_CONFIG,
    ERR_STOCK_ENABLE_DEBUG_PRIVILEGE,
    
    ERR_CONFIG_START = 3000,

    
    ERR_TRANSLATE_START = 4000,
    ERR_TRANSLATE_ALREADY_INIT,
    ERR_TRANSLATE_NO_RECORD,
    ERR_TRANSLATE_MALLOC,
    ERR_TRANSLATE_FOPEN,
    ERR_TRANSLATE_FSEEK,
    ERR_TRANSLATE_FREAD,
    ERR_TRANSLATE_FORMAT_TBL,
    ERR_TRANSLATE_CODE,
    ERR_TRANSLATE_NAME,

    ERR_CHIP_START = 5000,
    ERR_CHIP_BUY_METHOD,
    ERR_CHIP_HWND,
    ERR_CHIP_NAME_NULL,
    ERR_CHIP_CODE_NULL,
    ERR_CHIP_NUMBER_ZERO,
    ERR_CHIP_PRICE_ZERO,
    ERR_CHIP_EXEC_BUY_SCRIPT,
    
    ERR_WATCH_START = 6000,
    ERR_WATCH_INFO_HWND_INVALID,
    ERR_WATCH_GET_CONTENT,
    ERR_WATCH_PARSE_FAILED,
    ERR_WATCH_MEMORY_TOO_SMALL,
    ERR_WATCH_PARSE_BUY,
    ERR_WATCH_BUY_INFO_INVALID,
    ERR_WATCH_PARSE_CODE,
    ERR_WATCH_PARSE_NAME,
    ERR_WATCH_PARSE_PRICE,
    ERR_WATCH_PARSE_NUMBER,
    ERR_WATCH_PARSE_TIME,
    ERR_WATCH_HWND,
    ERR_WATCH_EXEC_KEEP_ALIVE_CHIP_SCRIPT,
    ERR_WATCH_EXEC_KEEP_ALIVE_WATCH_SCRIPT,
} ERROR_CODE_E; /* End of ERROR_CODE_E */

/* 成功代码定义 */
typedef enum tagSUCCESS_CODE_E
{
    SUCCESS_WATCH_START = 6000,
    SUCCESS_WATCH_DONE_LARGER,
    SUCCESS_WATCH_ALL_DONE,
    SUCCESS_WATCH_NEED_NOT_PARSE,
    
} SUCCESS_CODE_E; /* End of SUCCESS_CODE_E */


#endif /* End of __OSP_ERRORS_H__ */
