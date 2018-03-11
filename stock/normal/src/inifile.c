#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <string.h>
#include <ctype.h>

#include "inifile.h"

#ifdef __cplusplus
extern "C"
{
#endif

#define INI_MAX_FILE_SIZE (1024 * 16)
#define INT_VALUE_LEN (32)

#define UNKNOW_POS   (-1) /* ��ʼ����λ����Ϣ������λ��δ֪ */

#define LEFT_BRACE  '['
#define RIGHT_BRACE ']'
#define LINE_CHANGE '\n'  /* 0x0A */
#define LINE_END    '\r'  /* 0x0D */
#define STR_END     '\0'  /* 0x00 */

static int IsNewLine(char v_c)
{
	return ((LINE_CHANGE == v_c) ||  (LINE_END == v_c)) ? 1 : 0;
}

static int IsStrEnd(char v_c)
{
	return (STR_END == v_c) ? 1 : 0;
}

static int IsLeftBrace(char v_c)
{
	return (LEFT_BRACE == v_c) ? 1 : 0;
}

static int IsRightBrace(char v_c)
{
	return (RIGHT_BRACE == v_c) ? 1 : 0;
}

/*******************************************************************************
��������: LoadIniFile
����˵��: ��ini�ļ������������ڴ�
�������:
    v_pszFileName: Ҫװ�ص�ini�ļ���
�������:
    v_pszBuf     : Ҫװ�ص��ڴ�
    v_piFileSize : �ļ��Ĵ�С
�� �� ֵ:
    >=0: �ɹ�
    < 0: �������
˵    ��: ��
*******************************************************************************/
static int LoadIniFile(const char *v_pszFileName,
    char *v_pszBuf, int *v_piFileSize)
{
	FILE *pstFile = NULL;
	int i = 0;

	assert(NULL != v_pszFileName);
	assert(NULL != v_pszBuf);

	*v_piFileSize = 0;

    /* ��ֻ����ʽ���ļ� */
	pstFile = fopen(v_pszFileName, "r");
	if (NULL == pstFile)
    {
		return -INI_ERR_OPEN_FILE_FOR_READ;
	}

	/* װ���ļ� */
	v_pszBuf[i] = fgetc(pstFile);
	while (v_pszBuf[i] != (char)EOF)
    {
		i++;
		if (i >= INI_MAX_FILE_SIZE)
        { /* �ļ����ܳ���ָ����С */
            return -INI_ERR_FILE_TOO_LARGE;
		}

		v_pszBuf[i] = fgetc(pstFile);
	}

	v_pszBuf[i] = STR_END;
	*v_piFileSize = i;

	fclose(pstFile);

	return 0;
}

/*******************************************************************************
��������: SaveIniFile
����˵��: ���ڴ��е�����д�뵽ini�ļ���ȥ
�������:
    v_pszFileName: Ҫװ�ص�ini�ļ���
    v_pszBuf     : Ҫװ�ص��ڴ�
�������: ��
�� �� ֵ:
    >=0: �ɹ�
    < 0: �������
˵    ��: ��
*******************************************************************************/
static int SaveIniFile(const char *v_pszFileName, const char *v_pszBuf)
{
	FILE *pstFile = NULL;

	assert(NULL != v_pszFileName);
	assert(NULL != v_pszBuf);

    /* ��ֻд��ʽ���ļ� */
	pstFile = fopen(v_pszFileName, "w");
	if (NULL == pstFile)
    {
		return -INI_ERR_OPEN_FILE_FOR_WRITE;
	}

	/* д���ļ� */
	if (EOF == fputs(v_pszBuf, pstFile))
	{
		fclose(pstFile);
		return -INI_ERR_WRITE_FILE;
	}

	fclose(pstFile);

	return 0;
}

/*******************************************************************************
��������: ParseFile
����˵��: ��
�������:
    v_pszBuf     : װ����ini�ļ����ݵ��ڴ�
    v_pszSection : Ҫ���ҵ�section������
    v_pszKey     : Ҫ���ҵ�key������
�������:
    v_piSecStart  : section����ʼλ��
    v_piSecEnd    : section�Ľ���λ��
    v_piKeyStart  : key����ʼλ��
    v_piKeyEnd    : key�Ľ���λ��
    v_piValueStart: value����ʼλ��
    v_piValueEnd  : value�Ľ���λ��
�� �� ֵ:
    >=0: �ɹ�
    < 0: �������
˵    ��: ��
*******************************************************************************/
#if 0

static int ParseFile(
    const char *v_pszBuf, const char *v_pszSection, const char *v_pszKey,
    int *v_piSecStart, int *v_piSecEnd, int *v_piKeyStart, int *v_piKeyEnd,
    int *v_piValueStart, int *v_piValueEnd)
{
	const char *p = v_pszBuf;
	int i = 0;
	int iSecLen;
	int iKeyLen;

    iSecLen = strlen(v_pszSection);
    iKeyLen  = strlen(v_pszKey);

	assert(NULL != v_pszBuf);
	assert((NULL != v_pszSection) && (0 != iSecLen));
	assert((NULL != v_pszKey) && (0 != iKeyLen));

    *v_piSecStart = UNKNOW_POS;
	*v_piSecEnd = UNKNOW_POS;
    *v_piKeyStart = UNKNOW_POS;
    *v_piKeyEnd = UNKNOW_POS;
    *v_piValueStart = UNKNOW_POS;
    *v_piValueEnd = UNKNOW_POS;

	while (!IsStrEnd(p[i]))
    {
		if (((0 == i) ||  IsNewLine(p[i-1])) && IsLeftBrace(p[i]))
		{ /* �ҵ���'[' */
			int iSecStart = i + 1;

			/* Ѱ��']' */
			do
            {
				i++;
			} while (!IsRightBrace(p[i]) && !IsStrEnd(p[i]));

			if ((0 == strncmp(p + iSecStart, v_pszSection, i - iSecStart))
                && (iSecLen == i - iSecStart))
            {
				int iNewLineStart = 0;

				i++;

				/* ����']'��Ŀհ� */
				while (isspace(p[i]))
				{
					i++;
				}

				/* �ҵ���section */
				*v_piSecStart = iSecStart;
				*v_piSecEnd = i;

				while (!(IsNewLine(p[i-1]) && IsLeftBrace(p[i]))
				    && !IsStrEnd(p[i]))
			    {
					int j = 0;

					/* ����һ������ */
					iNewLineStart = i;

					while (!IsNewLine(p[i]) && !IsStrEnd(p[i]))
                    {
						i++;
					}

					/* now i  is equal to end of the line */
					j = iNewLineStart;

					if (';' != p[j]) /* skip over comment */
					{
						while ((j < i) && ('=' != p[j]))
                        {
							j++;
							if ('=' == p[j])
                            {
								if ((strncmp(v_pszKey,p+iNewLineStart,j-iNewLineStart)==0)
                                    && (iKeyLen==j-iNewLineStart))
								{
									/*find the key ok */
									*v_piKeyStart = iNewLineStart;
									*v_piKeyEnd = j-1;

									*v_piValueStart = j+1;
									*v_piValueEnd = i;

									return 0;
								}
							}
						}
					}

					i++;
				}
			}
		}
		else
		{
			i++;
		}
	}

	return -INI_ERR_PARSE_FILE;
}

#else

static int ParseFile(
    const char *v_pszBuf, const char *v_pszSection, const char *v_pszKey,
    int *v_piSecStart, int *v_piSecEnd, int *v_piKeyStart, int *v_piKeyEnd,
    int *v_piValueStart, int *v_piValueEnd)
{
	const char *p = v_pszBuf;
	int i = 0;
	int iSecLen;
	int iKeyLen;

    iSecLen = strlen(v_pszSection);
    iKeyLen  = strlen(v_pszKey);

	assert(NULL != v_pszBuf);
	assert((NULL != v_pszSection) && (0 != iSecLen));
	assert((NULL != v_pszKey) && (0 != iKeyLen));

    *v_piSecStart = UNKNOW_POS;
	*v_piSecEnd = UNKNOW_POS;
    *v_piKeyStart = UNKNOW_POS;
    *v_piKeyEnd = UNKNOW_POS;
    *v_piValueStart = UNKNOW_POS;
    *v_piValueEnd = UNKNOW_POS;

	while (!IsStrEnd(p[i]))
    {
		if (((0 == i) ||  IsNewLine(p[i-1])) && IsLeftBrace(p[i]))
		{ /* �ҵ���'[' */
			int iSecStart = i + 1;

			/* Ѱ��']' */
			do
            {
				i++;
			} while (!IsRightBrace(p[i]) && !IsStrEnd(p[i]));

			if ((0 == strncmp(p + iSecStart, v_pszSection, i - iSecStart))
                && (iSecLen == i - iSecStart))
            {
				int iNewLineStart = 0;

				i++;

				/* ����']'��Ŀհ� */
				while (isspace(p[i]))
				{
					i++;
				}

				/* �ҵ���section */
				*v_piSecStart = iSecStart;
				*v_piSecEnd = i;

				while (!(IsNewLine(p[i-1]) && IsLeftBrace(p[i]))
				    && !IsStrEnd(p[i]))
			    {
					int j = 0;

					/* ����һ������ */
					iNewLineStart = i;

					while (!IsNewLine(p[i]) && !IsStrEnd(p[i]))
                    {
						i++;
					}

					/* now i  is equal to end of the line */
					j = iNewLineStart;

					if (';' != p[j]) /* skip over comment */
					{
						while ((j < i) && ('=' != p[j]))
                        {
							j++;
							if ('=' == p[j])
                            {
								if ((strncmp(v_pszKey,p+iNewLineStart,j-iNewLineStart)==0)
                                    && (iKeyLen==j-iNewLineStart))
								{
									/*find the key ok */
									*v_piKeyStart = iNewLineStart;
									*v_piKeyEnd = j-1;

									*v_piValueStart = j+1;
									*v_piValueEnd = i;

									return 0;
								}
							}
						}
					}

					i++;
				}
			}
		}
		else
		{
			i++;
		}
	}

	return -INI_ERR_PARSE_FILE;
}

#endif

/*******************************************************************************
��������: IniReadString
����˵��: ��ini�ļ��ж�ָ���ֶε�ֵ
�������:
    v_pszFileName    : Ҫ��ȡ��ini�ļ���
    v_pszSection     : Ҫ��ȡ��section������
    v_pszKey         : Ҫ��ȡ��key������
    iValueSize       : ��ȡ����value����󳤶�
    v_pszDefaultValue: ����Ҳ�����Ӧ���ֶΣ���ô�ͽ���Ĭ��ֵ��ֵ��v_pszValue
�������:
    v_pszValue   : ��ȡ����valueֵ
�� �� ֵ:
    > 0: �ɹ�����
    ==0: �ɹ�
    < 0: �������
˵    ��: ��
*******************************************************************************/
int IniReadString(
    const char *v_pszFileName, const char *v_pszSection, const char *v_pszKey,
    char *v_pszValue, int v_iValueSize, const char *v_pszDefaultValue)
{
	char *pszBuf = NULL;
	int iFileSize = 0;
	int iSecStart = 0;
    int iSecEnd = 0;
    int iKeyStart= 0;
    int iKeyEnd = 0;
    int iValueStart = 0;
    int iValueEnd = 0;
    int iCopyCnt = 0;

	/* ������ */
    if ((NULL == v_pszFileName) || (0 == strlen(v_pszFileName)))
    {
        return -INI_ERR_NO_FILE_NAME;
    }

    if ((NULL == v_pszSection) || (0 == strlen(v_pszSection)))
    {
        return -INI_ERR_NO_SECTION;
    }

    if ((NULL == v_pszKey) || (0 == strlen(v_pszKey)))
    {
        return -INI_ERR_NO_KEY;
    }

    if ((NULL == v_pszValue) || (0 >= v_iValueSize))
    {
        return -INI_ERR_NO_VALUE;
    }

    /* �����ڴ� */
    pszBuf = malloc(INI_MAX_FILE_SIZE);
    if (NULL == pszBuf)
    {
        return -INI_ERR_MALLOC;
    }

    /* �����ļ� */
	if (0 != LoadIniFile(v_pszFileName, pszBuf, &iFileSize))
	{ /* ����ʧ�� */
		if (NULL != v_pszDefaultValue)
		{
			strncpy(v_pszValue, v_pszDefaultValue, v_iValueSize);
            free(pszBuf);
            return INI_ERR_LOAD_FILE;
		}

        free(pszBuf);
		return -INI_ERR_LOAD_FILE;
	}

	if (0 != ParseFile(pszBuf, v_pszSection, v_pszKey, &iSecStart, &iSecEnd,
        &iKeyStart, &iKeyEnd, &iValueStart, &iValueEnd))
	{ /* ָ���ֶ�δ�ҵ� */
		if (NULL != v_pszDefaultValue)
		{
			strncpy(v_pszValue, v_pszDefaultValue, v_iValueSize);
            free(pszBuf);
            return INI_ERR_PARSE_FILE;
		}

        free(pszBuf);
		return -INI_ERR_PARSE_FILE;
	}

	iCopyCnt = iValueEnd - iValueStart;

    /* �۳��ַ���������������Ҫ-1 */
	if ((v_iValueSize - 1) < iCopyCnt)
	{
		iCopyCnt = (v_iValueSize - 1);
	}

	memcpy(v_pszValue, pszBuf + iValueStart, iCopyCnt);
	v_pszValue[iCopyCnt] = STR_END;

    free(pszBuf);
	return 0;
}

/*******************************************************************************
��������: IniWriteString
����˵��: ��ֵд��ini�ļ���ָ���ֶ�
�������:
    v_pszFileName    : Ҫд���ini�ļ���
    v_pszSection     : Ҫд���section������
    v_pszKey         : Ҫд���key������
    v_pszValue       : Ҫд���valueֵ
�������: ��
�� �� ֵ:
    >=0: �ɹ�
    < 0: �������
˵    ��: ��
*******************************************************************************/
int IniWriteString(
    const char *v_pszFileName, const char *v_pszSection, const char *v_pszKey,
    const char *v_pszValue)
{
	char *pszWrBuf = NULL;
	char *pszBuf = NULL;
	int iFileSize = 0;
	int iRet = 0;
	int iSecStart = UNKNOW_POS;
    int iSecEnd = 0;
    int iKeyStart= 0;
    int iKeyEnd = 0;
    int iValueStart = 0;
    int iValueEnd = 0;
    int iValueSize = (int)strlen(v_pszValue);

	/* ������ */
    if ((NULL == v_pszFileName) || (0 == strlen(v_pszFileName)))
    {
        return -INI_ERR_NO_FILE_NAME;
    }

    if ((NULL == v_pszSection) || (0 == strlen(v_pszSection)))
    {
        return -INI_ERR_NO_SECTION;
    }

    if ((NULL == v_pszKey) || (0 == strlen(v_pszKey)))
    {
        return -INI_ERR_NO_KEY;
    }

    if (NULL == v_pszValue)
    {
        return -INI_ERR_NO_VALUE;
    }

    /* �����ڴ� */
    pszBuf = malloc(INI_MAX_FILE_SIZE);
    if (NULL == pszBuf)
    {
        return -INI_ERR_MALLOC;
    }

    /* �����ڴ� */
    pszWrBuf = malloc(INI_MAX_FILE_SIZE);
    if (NULL == pszWrBuf)
    {
        free(pszBuf);
        return -INI_ERR_MALLOC;
    }

    memset(pszWrBuf, 0, INI_MAX_FILE_SIZE);

    /* �����ļ� */
	if (0 == LoadIniFile(v_pszFileName, pszBuf, &iFileSize))
	{ /* ����ɹ� */
		ParseFile(pszBuf, v_pszSection, v_pszKey, &iSecStart, &iSecEnd,
            &iKeyStart, &iKeyEnd, &iValueStart, &iValueEnd);
	}

	if (UNKNOW_POS == iSecStart)
	{ /* sectionλ��δ��ʼ�� */
		if (0 == iFileSize)
		{ /* ���ļ� */
			sprintf(pszWrBuf + iFileSize, "[%s]\n%s=%s\n",
                v_pszSection, v_pszKey, v_pszValue);
		}
		else
		{ /* ��ָ����section */
			memcpy(pszWrBuf,pszBuf,iFileSize);
			sprintf(pszWrBuf + iFileSize, "\n[%s]\n%s=%s\n",
                v_pszSection, v_pszKey, v_pszValue);
		}
	}
	else if (UNKNOW_POS == iKeyStart)
	{ /* ��ָ����section������û��ָ����key */
		memcpy(pszWrBuf, pszBuf, iSecEnd);
		sprintf(pszWrBuf + iSecEnd, "%s=%s\n", v_pszKey, v_pszValue);
		memcpy(pszWrBuf + iSecEnd + strlen(v_pszKey) + iValueSize + 2,
            pszBuf + iSecEnd, iFileSize - iSecEnd);
	}
	else
	{ /* ��ָ����section��Ҳ��ָ����key */
		memcpy(pszWrBuf, pszBuf, iValueStart);
		memcpy(pszWrBuf + iValueStart, v_pszValue, iValueSize);
		memcpy(pszWrBuf + iValueStart + iValueSize, pszBuf + iValueEnd,
            iFileSize - iValueEnd);
	}

	iRet = SaveIniFile(v_pszFileName, pszWrBuf);

    free(pszWrBuf);
    free(pszBuf);

    return iRet;
}

/*******************************************************************************
��������: IniReadInt
����˵��: ��
�������: ��
�������: ��
�� �� ֵ:
    >=0: �ɹ�
    < 0: �������
˵    ��: ��
*******************************************************************************/
int IniReadInt(
    const char *v_pszFileName, const char *v_pszSection, const char *v_pszKey,
    int v_pszDefaultValue)
{
	char szValue[INT_VALUE_LEN] = {0};

	if (0 != IniReadString(v_pszFileName, v_pszSection, v_pszKey,
        szValue, INT_VALUE_LEN, NULL))
	{
		return v_pszDefaultValue;
	}

	return atoi(szValue);
}

#if 1

#include <stdio.h>
#include "inifile.h"


#define TEST_BUF_SIZE 256
#define TEST_INI_FILE "config\\sample.ini"

int DebugIniFile(void)
{
    const char szSection[] = "student";
    const char szKeyName[] = "name";
    const char szKeyAge[] = "age";
    char szValue[TEST_BUF_SIZE]={0};
    int age;

    if (0 != IniWriteString(TEST_INI_FILE, szSection, szKeyName, "Tony"))
    {
        printf("IniWriteString failed. [%s, %s, %s]\n",
            szSection, szKeyName, "Tony");
    }
    else
    {
        printf("IniWriteString success. [%s, %s, %s]\n",
            szSection, szKeyName, "Tony");
    }

    if (0 != IniWriteString(TEST_INI_FILE, szSection, szKeyAge, "20"))
    {
        printf("IniWriteString failed. [%s, %s, %s]\n",
            szSection, szKeyAge, "20");
    }
    else
    {
        printf("IniWriteString Success. [%s, %s, %s]\n",
            szSection, szKeyAge, "20");
    }

    if (0 != IniReadString(TEST_INI_FILE, szSection, szKeyName,
        szValue, TEST_BUF_SIZE, ""))
    {
        printf("IniReadString failed. [%s, %s]\n",
            szSection, szKeyName);
    }
    else
    {
        printf("IniReadString success. [%s, %s, %s]\n",
            szSection, szKeyName, szValue);
    }

    age = IniReadInt(TEST_INI_FILE, szSection, szKeyAge, 0);
    printf("IniReadInt here. [%s, %s, %d]\n",
    szSection, szKeyAge, age);

    /* ���� */
    if (0 != IniReadString(TEST_INI_FILE, "Main", "test case",
        szValue, TEST_BUF_SIZE, NULL))
    {
        printf("IniReadString failed. [%s, %s]\n",
            "Main", "test case");
    }
    else
    {
        printf("IniReadString success. [%s, %s, %s]\n",
            "Main", "test case", szValue);
    }

    /* ���� */
    if (0 != IniReadString(TEST_INI_FILE, "Main", "test case1",
        szValue, TEST_BUF_SIZE, NULL))
    {
        printf("IniReadString failed. [%s, %s]\n",
            "Main", "test case1");
    }
    else
    {
        printf("IniReadString success. [%s, %s, %s]\n",
            "Main", "test case1", szValue);
    }

    /* ���� */
    if (0 != IniWriteString(TEST_INI_FILE, "Main", "test case1",
        "TEST CASE1"))
    {
        printf("IniWriteString failed. [%s, %s]\n",
            "Main", "test case1");
    }
    else
    {
        printf("IniWriteString success. [%s, %s]\n",
            "Main", "test case1");
    }

    if (0 != IniReadString(TEST_INI_FILE, "Main", "test case1",
        szValue, TEST_BUF_SIZE, NULL))
    {
        printf("IniReadString failed. [%s, %s]\n",
            "Main", "test case1");
    }
    else
    {
        printf("IniReadString success. [%s, %s, %s]\n",
            "Main", "test case1", szValue);
    }


    return 0;
}

#endif

#ifdef __cplusplus
}; /*end of extern "C" { */
#endif

