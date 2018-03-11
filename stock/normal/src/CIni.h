//********************************************
//	Ini 相关函数
//********************************************

#ifndef _INI_H_
#define _INI_H_

#define ERROR_DATA -99999999

#ifndef MAX_PATH
#define MAX_PATH 256
#endif /* End of MAX_PATH */

typedef enum tagCINI_ERROR_CODE_E
{
    CINI_ERR_START = 0x10000,
    CINI_ERR_OPEN_FILE_FOR_READ,
    CINI_ERR_OPEN_FILE_FOR_WRITE,
    CINI_ERR_SESSION_NOT_FOUND,
        
} CINI_ERROR_CODE_E; /* End of CINI_ERROR_CODE_E */



//配置文件类
class CIni
{
////////////////////////////////////////////////
// 内部数据
////////////////////////////////////////////////
private:
	char m_szFileName[MAX_PATH];	// 文件名
	char* m_pacData;				    // 文件内容
	int m_uiDataLen;				// 文件长度

	unsigned int m_uiSessionNum;	// 索引数目（[]的数目）
	unsigned int *m_puiSessionList; // 索引点位置列表
	//int Point;						// 当前指针
	//int Line, Word;					// 当前行列

////////////////////////////////////////////////
// 内部函数
////////////////////////////////////////////////
private:
    void _InitSessions(void);                       //初始化索引
    int _FindSession(const char *);        //返回标题位置
    int _FindData(int, char *);             //返回数据位置
    int _GotoNextLine(int);                     //提行
    char *_ReadDataName(int &);              //在指定位置读一数据名称
    char *_ReadText(int);                    //在指定位置读字符串

    bool _AddSession(char *);                   //加入一个索引
    bool _AddData(int, char *, char *);     //在当前位置加入一个数据
    bool _ModityData(int, char *, char *);  //在当前位置修改一个数据的值
    int _GotoLastLine(char *index);         //把指针移动到本INDEX的最后一行

////////////////////////////////////////////////
// 通用接口
////////////////////////////////////////////////
public:
	CIni();
	CIni(char*);							// 初始化打开配置文件
	virtual ~CIni();						// 释放内存

	int Load(const char *);						// 打开配置文件
	int Save(const char *filename = NULL);			// 保存配置文件

////////////////////////////////////////////////
// 公用接口
////////////////////////////////////////////////
public:
	int ReadInt(char *, char *);			// 读一个整数
	int ReadInt(char *, int );				// 在指定的行读一整数
	char *_ReadText(char *, char *);			// 读一个字符串
	char *_ReadText(char *, int);			// 在指定的行读一字符串
	char *ReadCaption(char *, int);			// 在指定行读一字符名称
	bool SetKeyInt(char *, char *, int);    // 写一个整数
	bool SetKeyStr(char *, char *, char *); // 写一个字符串
	bool SetSession(char *, char *, int);    // 写一个整数
	bool GetSession(char *, char *, char *); // 写一个字符串
	int GetContinueDataNum(char *);			// 返回连续的行数（从INDEX到第一个空行）
};

#endif

