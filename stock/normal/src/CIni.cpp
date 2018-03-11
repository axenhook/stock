//********************************************
//	Ini 相关函数
//********************************************

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include "CIni.h"

#ifndef SAFE_DELETE
#define SAFE_DELETE(x)	if( (x)!=NULL ) { delete (x); (x)=NULL; }
#endif

#ifndef SAFE_DELETE_ARRAY
#define SAFE_DELETE_ARRAY(x)	if( (x)!=NULL ) { delete[] (x); (x)=NULL; }
#endif

#ifndef SAFE_FREE
#define SAFE_FREE(x)	if( (x)!=NULL ) { free(x); (x)=NULL; }
#endif

#ifndef SAFE_RELEASE
#define SAFE_RELEASE(x)	if( (x)!=NULL ) { (x)->Release(); (x)=NULL; }
#endif


////////////////////////////////////////////////
// 通用接口
////////////////////////////////////////////////

// 初始化
CIni::CIni()
{
	m_uiDataLen = 0;
	m_uiSessionNum = 0;
	m_pacData = NULL;
	m_puiSessionList = NULL;
}

// 初始化
CIni::CIni(char *filename)
{
	m_uiDataLen = 0;
	m_uiSessionNum = 0;
	m_pacData = NULL;
	m_puiSessionList = NULL;
    
	Load(filename);
}

// 析构释放
CIni::~CIni()
{
	if (0 != m_uiDataLen)
	{
		SAFE_DELETE(m_pacData);
		m_uiDataLen = 0;
	}

	if (0 != m_uiSessionNum)
	{
		SAFE_DELETE(m_puiSessionList);
		m_uiSessionNum = 0;
	}
}

// 读入文件
int CIni::Load(const char *filename)
{
	FILE *fp = NULL;
    
	snprintf(m_szFileName, MAX_PATH, "%s", filename);

	SAFE_FREE(m_pacData);
    
	// 打开文件
	fp = fopen(filename, "rb");
    if (NULL == fp)
    { // 打开文件失败，可能是文件不存在
        return -CINI_ERR_OPEN_FILE_FOR_READ;
    }

	// 获取文件长度
	fseek(fp, 0L, SEEK_END);
	m_uiDataLen = ftell(fp);
	fseek(fp, 0L, SEEK_SET);
    
	if (m_uiDataLen > 0)
	{
		m_pacData = new char[m_uiDataLen];
		fread(m_pacData, m_uiDataLen, 1, fp); // 读数据
		fclose(fp);
        
		// 初始化索引
		_InitSessions();
		return 0;
	}
    
	m_uiDataLen = 1;
	m_pacData = new char[m_uiDataLen];
	memset(m_pacData, 0, 1);
    
	// 初始化索引
	_InitSessions();

	return 0;
}

// 保存到文件
int CIni::Save(const char *filename)
{
	FILE *fp = NULL;
    
	if (NULL == filename)
	{
		filename = m_szFileName;
	}

	fp = fopen(filename, "wb");
    if (NULL == fp)
    { // 打开文件失败，后可能是文件不存在
        return -CINI_ERR_OPEN_FILE_FOR_WRITE;
    }

	fwrite(m_pacData, m_uiDataLen, 1, fp);
	fclose(fp);

	return 0;
}

////////////////////////////////////////////////
// 内部函数
////////////////////////////////////////////////

// 计算出所有的session的位置
void CIni::_InitSessions(void)
{
	m_uiSessionNum = 0;

	for (int i = 0; i < m_uiDataLen; i++)
	{
		if (('[' == m_pacData[i]) && ((0 == i) || ('\n' == m_pacData[i - 1])))
		{ // 找到了
			m_uiSessionNum++;
		}
	}

	// 申请内存
	SAFE_DELETE(m_puiSessionList);
	if (m_uiSessionNum > 0)
	{
		m_puiSessionList = new int[m_uiSessionNum];
    }

	int n = 0;

	for (int i = 0; i < m_uiDataLen; i++)
	{
		if (('[' == m_pacData[i]) && ((0 == i) || ('\n' == m_pacData[i - 1])))
		{
			m_puiSessionList[n++] = i + 1;
		}
	}
}

// 查找指定标题的位置
int CIni::_FindSession(const char *pszSessionName)
{
	for (int i = 0; i < m_uiSessionNum; i++)
	{
		char *str = _ReadText(m_puiSessionList[i]);
        
		if (0 == strcmp(pszSessionName, str))
		{
			SAFE_FREE( str );
			return m_puiSessionList[i];
		}
        
		SAFE_FREE( str );
	}
    
	return -CINI_ERR_SESSION_NOT_FOUND;
}

//返回指定数据的位置
int CIni::_FindData(int index, char *string)
{
	int p=index;	//指针

	while(1)
	{
		p=_GotoNextLine(p);
		char *name=_ReadDataName(p);
		if( strcmp(string, name)==0 )
		{
			SAFE_FREE( name );
			return p;
		}

		SAFE_FREE( name );
		if( p>=m_uiDataLen ) return -1;
	}
	return -1;
}

// 提行
int CIni::_GotoNextLine(int p)
{
	for (int i = p; i < m_uiDataLen; i++)
	{
		if (m_pacData[i] == '\n')
		{
			return i + 1;
        }
	}
    
	return i;
}

//在指定位置读一数据名称
char *CIni::_ReadDataName(int &p)
{
	char chr;
	char *Ret;
	int m=0;

	Ret=new char[64];
	memset(Ret, 0, 64);

	for(int i=p; i<m_uiDataLen; i++)
	{
		chr = m_pacData[i];

		//结束
		if( chr == '\r' )
		{
			p=i+1;
			return Ret;
		}
		
		//结束
		if( chr == '=' || chr == ';' )
		{
			p=i+1;
			return Ret;
		}
		
		Ret[m]=chr;
		m++;
	}
	return Ret;
}

// 在指定位置读一字符串
char *CIni::_ReadText(int p)
{
	char chr;
	char *pac;
	int n=p, m=0;

	int ichNum = _GotoNextLine(p) - p + 1;
    
	pac = new char[ichNum];
	memset(pac, 0, ichNum);

	for (int i = 0; i < ichNum; i++)
	{
		chr = m_pacData[n];

		if ((chr == ';') || (chr == '\r') || (chr == '\t') || (chr == ']'))
		{ // 结束
			return pac;
		}
		
		pac[m] = chr;
		m++;
		n++;
	}

	return pac;
}

//加入一个索引
bool CIni::_AddSession(char *index)
{
	char str[256];
	memset(str, 0, 256);
	int n=_FindSession(index);

	if( n == -1 )	//新建索引
	{
		sprintf(str,"\r\n[%s]\r\n",index);
		m_pacData = (char *)realloc(m_pacData, m_uiDataLen+strlen(str));	//重新分配内存
		sprintf(&m_pacData[m_uiDataLen], "%s", str);
		m_uiDataLen+=strlen(str);

		_InitSessions();
		return true;
	}
	
	return false;	//已经存在
}

//在当前位置加入一个数据
bool CIni::_AddData(int p, char *name, char *string)
{
	char *str;
	int len=strlen(string);
	str=new char[len+256];
	memset(str, 0, len+256);
	sprintf(str,"%s=%s\r\n",name,string);
	len=strlen(str);

	p=_GotoNextLine(p);	//提行
	m_pacData = (char *)realloc(m_pacData, m_uiDataLen+len);	//重新分配内存

	char *temp=new char[m_uiDataLen-p];
	memcpy(temp, &m_pacData[p], m_uiDataLen-p);
	memcpy(&m_pacData[p+len], temp, m_uiDataLen-p);	//把后面的搬到末尾
	memcpy(&m_pacData[p], str, len);
	m_uiDataLen+=len;

	SAFE_DELETE( temp );
	SAFE_DELETE( str );
	_InitSessions();	//nxx
	return true;
}

//在当前位置修改一个数据的值
bool CIni::_ModityData(int p, char *name, char *string)
{
	int n=_FindData(p, name);

	char *t=_ReadText(n);
	p=n+strlen(t);
	if( strlen(t)>0 ) free(t);

	int newlen=strlen(string);
	int oldlen=p-n;

	m_pacData = (char *)realloc(m_pacData, m_uiDataLen+newlen-oldlen);	//重新分配内存

	char *temp=new char[m_uiDataLen-p];
	memcpy(temp, &m_pacData[p], m_uiDataLen-p);
	memcpy(&m_pacData[n+newlen], temp, m_uiDataLen-p);			//把后面的搬到末尾
	memcpy(&m_pacData[n], string, newlen);
	m_uiDataLen+=newlen-oldlen;

	SAFE_DELETE( temp );
	_InitSessions(); //nxx
	return true;
}

//把指针移动到本INDEX的最后一行
int CIni::_GotoLastLine(char *index)
{
	int n=_FindSession(index);
	n=_GotoNextLine(n);
	while(1)
	{
		if( m_pacData[n] == '\r' || m_pacData[n] == EOF || m_pacData[n] == -3 || m_pacData[n] == ' ' || m_pacData[n] == '/' || m_pacData[n] == '\t' || m_pacData[n] == '\n' )
		{
			return n;
		}
		else
		{
			n=_GotoNextLine(n);
			if( n >= m_uiDataLen ) return n;
		}
	}
}

/////////////////////////////////////////////////////////////////////
// 对外接口
/////////////////////////////////////////////////////////////////////

//以普通方式读一字符串数据
char *CIni::_ReadText(char *index, char *name)
{
	int n=_FindSession(index);
	assert( n != -1 );

	int m=_FindData(n, name);
	assert( m != -1 );

	return _ReadText(m);
}
	
//在指定的行读一字符串
char *CIni::_ReadText(char *index, int lines)
{
	int n=_FindSession(index);
	assert( n != -1 );

	//跳到指定行数
	n=_GotoNextLine(n);
	for(int i=0; i<lines; i++)
	{
		if( n<m_uiDataLen )
			n=_GotoNextLine(n);
	}

	//读数据
	while( n<=m_uiDataLen )
	{
		if( m_pacData[n] == '=' )
		{
			n++;
			return _ReadText(n);
		}
		if( m_pacData[n] == '\r' )
		{
			return "";
		}
		n++;
	}

	return "";
}

//以普通方式读一整数
int CIni::ReadInt(char *index, char *name)
{
	int n=_FindSession(index);
	assert( n != -1 );

	int m=_FindData(n, name);
	assert( m != -1 );

	char *str=_ReadText(m);
	int ret=atoi(str);
	free(str);
	return ret;
}

//在指定的行读一整数
int CIni::ReadInt(char *index, int lines)
{
	int n=_FindSession(index);
	assert( n != -1 );

	//跳到指定行数
	n=_GotoNextLine(n);
	for(int i=0; i<lines; i++)
	{
		if( n<m_uiDataLen )
			n=_GotoNextLine(n);
	}

	//读数据
	while( n<m_uiDataLen )
	{
		if( m_pacData[n] == '=' )
		{
			n++;
			char *str=_ReadText(n);
			int ret=atoi(str);
			free(str);
			return ret;
		}
		if( m_pacData[n] == '\r' )
		{
			return ERROR_DATA;
		}
		n++;
	}

	return ERROR_DATA;
}

//在指定的行读一数据名称
char *CIni::ReadCaption(char *index, int lines)
{
	int n=_FindSession(index);
	assert( n != -1 );

	//跳到指定行数
	n=_GotoNextLine(n);
	for(int i=0; i<lines; i++)
	{
		if( n<m_uiDataLen )
			n=_GotoNextLine(n);
	}

	return _ReadDataName(n);
}

//以普通方式写一字符串数据
bool CIni::SetKeyStr(char *pszSession, char *pszKey, char *pszStr)
{
	int n=_FindSession(pszSession);
	if( n == -1 )	// 新建索引
	{
		_AddSession(pszSession);
		n=_FindSession(pszSession);
		n=_GotoLastLine(pszSession);
		_AddData(n, pszKey, pszStr);	// 在当前位置n加一个数据
		return true;
	}

	//存在索引
	int m=_FindData(n, pszKey);
	if( m==-1 )		// 新建数据
	{
		n=_GotoLastLine(pszSession);
		_AddData(n, pszKey, pszStr);	// 在当前位置n加一个数据
		return true;
	}

	// 存在数据
	_ModityData(n, pszKey, pszStr);	// 修改一个数据

	return true;
}

//以普通方式写一整数
bool CIni::SetKeyInt(char *pszSession, char *pszKey, int iValue)
{
	char string[32];
	sprintf(string, "%d", iValue);

	int n=_FindSession(pszSession);
	if( n == -1 )	//新建索引
	{
		_AddSession(pszSession);
		n=_FindSession(pszSession);
		n=_GotoLastLine(pszSession);
		_AddData(n, pszKey, string);	//在当前位置n加一个数据
		return true;
	}

	//存在索引
	int m=_FindData(n, pszKey);
	if( m==-1 )		//新建数据
	{
		n=_GotoLastLine(pszSession);
		_AddData(n, pszKey, string);	//在当前位置n加一个数据
		return true;
	}

	//存在数据
	_ModityData(n, pszKey, string);	//修改一个数据

	return true;
}

//返回连续的行数
int CIni::GetContinueDataNum(char *index)
{
	int num=0;
	int n=_FindSession(index);
	n=_GotoNextLine(n);
	while(1)
	{
		if( m_pacData[n] == '\r' || m_pacData[n] == EOF || m_pacData[n] == -3 || m_pacData[n] == ' ' || m_pacData[n] == '/' || m_pacData[n] == '\t' || m_pacData[n] == '\n' )
		{
			return num;
		}
		else
		{
			num++;
			n=_GotoNextLine(n);
			if( n >= m_uiDataLen ) return num;
		}
	}
}
