//********************************************
//	Ini ��غ���
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



//�����ļ���
class CIni
{
////////////////////////////////////////////////
// �ڲ�����
////////////////////////////////////////////////
private:
	char m_szFileName[MAX_PATH];	// �ļ���
	char* m_pacData;				    // �ļ�����
	int m_uiDataLen;				// �ļ�����

	unsigned int m_uiSessionNum;	// ������Ŀ��[]����Ŀ��
	unsigned int *m_puiSessionList; // ������λ���б�
	//int Point;						// ��ǰָ��
	//int Line, Word;					// ��ǰ����

////////////////////////////////////////////////
// �ڲ�����
////////////////////////////////////////////////
private:
    void _InitSessions(void);                       //��ʼ������
    int _FindSession(const char *);        //���ر���λ��
    int _FindData(int, char *);             //��������λ��
    int _GotoNextLine(int);                     //����
    char *_ReadDataName(int &);              //��ָ��λ�ö�һ��������
    char *_ReadText(int);                    //��ָ��λ�ö��ַ���

    bool _AddSession(char *);                   //����һ������
    bool _AddData(int, char *, char *);     //�ڵ�ǰλ�ü���һ������
    bool _ModityData(int, char *, char *);  //�ڵ�ǰλ���޸�һ�����ݵ�ֵ
    int _GotoLastLine(char *index);         //��ָ���ƶ�����INDEX�����һ��

////////////////////////////////////////////////
// ͨ�ýӿ�
////////////////////////////////////////////////
public:
	CIni();
	CIni(char*);							// ��ʼ���������ļ�
	virtual ~CIni();						// �ͷ��ڴ�

	int Load(const char *);						// �������ļ�
	int Save(const char *filename = NULL);			// ���������ļ�

////////////////////////////////////////////////
// ���ýӿ�
////////////////////////////////////////////////
public:
	int ReadInt(char *, char *);			// ��һ������
	int ReadInt(char *, int );				// ��ָ�����ж�һ����
	char *_ReadText(char *, char *);			// ��һ���ַ���
	char *_ReadText(char *, int);			// ��ָ�����ж�һ�ַ���
	char *ReadCaption(char *, int);			// ��ָ���ж�һ�ַ�����
	bool SetKeyInt(char *, char *, int);    // дһ������
	bool SetKeyStr(char *, char *, char *); // дһ���ַ���
	bool SetSession(char *, char *, int);    // дһ������
	bool GetSession(char *, char *, char *); // дһ���ַ���
	int GetContinueDataNum(char *);			// ������������������INDEX����һ�����У�
};

#endif

