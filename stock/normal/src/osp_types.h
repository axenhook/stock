/*******************************************************************************

            ��Ȩ����(C), 2010~2013, ������ (EMail: zeng_hr@163.com)
********************************************************************************
�� �� ��: OSP_TYPES.H
��    ��: ������ (EMail: zeng_hr@163.com)
��    ��: V1.00
��    ��: 2010��5��26��
��������: 
�����б�: 
    1. ...: 
�޸���ʷ: 
    ����: ������ (EMail: zeng_hr@163.com)    ����: 2010��5��26��
--------------------------------------------------------------------------------
    1. ��ʼ�汾
*******************************************************************************/
#ifndef __OSP_TYPES_H__
#define __OSP_TYPES_H__

/* �����������Ͷ��� */
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
