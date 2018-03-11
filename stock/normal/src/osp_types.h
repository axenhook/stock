/*******************************************************************************

            版权所有(C), 2010~2013, 曾华荣 (EMail: zeng_hr@163.com)
********************************************************************************
文 件 名: OSP_TYPES.H
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
#ifndef __OSP_TYPES_H__
#define __OSP_TYPES_H__

/* 基本数据类型定义 */
typedef char               OSP_S8;
typedef short              OSP_S16;
typedef int                OSP_S32;
typedef long long          OSP_S64;

typedef unsigned char      OSP_U8;
typedef unsigned short     OSP_U16;
typedef unsigned int       OSP_U32;
typedef unsigned long long OSP_U64;

typedef OSP_S32            OSP_BOOL;
typedef void               OSP_VOID;

#define B_TRUE   1
#define B_FALSE  0

#ifndef SUCCESS
#define SUCCESS 0
#endif /* End of SUCCESS */

#endif /* End of __OSP_TYPES_H__ */
