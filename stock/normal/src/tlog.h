#ifndef _TLOG_H_
#define _TLOG_H_

#include <string.h>
#include <stdio.h>
#include <stdarg.h>
#include <sys/types.h>
#include <sys/stat.h>


#ifdef WIN32
#include <afxmt.h>
#include <winsock.h>
#include <cassert>
#include <io.h>
#else
#include <unistd.h>
#include <assert.h>
#endif

#define MAX_FILE_LINES    200000

#define LOG_TO_NULL        0
#define LOG_TO_FILE        1
#define LOG_TO_SCREEN    2
#define LOG_TO_SCNFILE    3

#ifndef WIN32
#define MAX_PATH            256
#endif
#define MAX_VERSION_LEN        256

typedef int CTbool;
#define CT_boolFALSE 0
#define CT_boolTRUE  1

class TLog
{
public:
    void SetVersion(const char *szVersion);
    TLog(const char *module, const char *version,int bakmode = 0, int multiuser = 1);
    virtual ~TLog();
    //����־�ļ�
    FILE *open(int mode = LOG_TO_FILE);
    //�ر���־�ļ�
    void close();
    //������־�������ʽ����nul,FILE��ȱʡ��,SCREEN or both

    void setMode(int mode);
    //�����Ƿ�����ˢ����־��־,0=������ˢ�£�1=����ˢ�£�ȱʡ��

    void setFlushFlag(int flag){ m_nFlushFlag = flag;}

    //������־�ļ��������
    void setMaxLines(int nMaxLines);

    //�õ���־�������ʽ
    int getMode() const { return m_nTraceMode; }

    //��ͨ��־����ͷ��ӡʱ��
    void print(const char *format,...);
    void trace(int level, const char *format,...);

    //��ͨ��־����ͷ��ӡʱ���ID
    void print(int ID, const char *format,...);

    //��ͨ��־����ͷ����ӡʱ��
    void printnt(const char *format,...);
    void tracent(int level, const char *format,...);

    //��������־����ͷ��ӡʱ��
    void printb(const char *title, const unsigned char *buf,int len);
    void traceb(int level, const char *title, const unsigned char *buf,int len);

    //��������־����ͷ��ӡʱ��
    void printb(unsigned int ID, const char *title, const unsigned char *buf,int len);

    //��������־����ͷ����ӡʱ��
    void printnb(const unsigned char *buf,int len);
    void tracenb(int level, const unsigned char *buf,int len);

    //��ӡ������־����ͷ��ӡʱ��
    void perror(const char *format,...);

    //���Դ��ݱ䳤����ָ����д�ӡ,��ͷ��ӡʱ���ID
    void vprint(int ID, const char *format, va_list argptr);

    //ˢ����־
    void flush();

    void SetTraceLevelThreshold(int level);    //���ڶ�̬������־����

private:
    //��ֹ�����÷���printnt(0, "%d", 1234);
    void printnt(int , const char *format,...);
    void perror(int , const char *format,...);

    //����
    void Backup();
    //��ӡ�汾��Ϣ
    void PrintVersion(void);
    CTbool CanTrace(int level);            //���ڿ����Ƿ��ӡ��־

    inline void LOCK()  { if(m_nMultiUser)  m_Lock.Lock(); }
    inline void UNLOCK()  { if(m_nMultiUser)  m_Lock.Unlock(); }

private:
    FILE *m_FP;                            //�ļ�������
    CCriticalSection m_Lock;
    char m_acFileName[MAX_PATH];        //��־�ļ���
    char m_acBakFileName[MAX_PATH];        //������־��LOG_Bak
    char m_acErrorFileName[MAX_PATH];    //������־�ļ���
    int m_nTotalCount;                //����������
    int m_nMaxLines;                    //��־�ļ����������
    char m_acModuleName[MAX_PATH];        //ģ����
    int m_nTraceMode;                    //��־���λ�ã�nul,�ļ�,��Ļ,both
    int m_nFlushFlag;                    //�Ƿ�����ˢ����־
    int m_nMultiUser;                    //�Ƿ���û������û�ʹ��ʱ��Ҫ�������ʣ����û�ʹ��ʱ����Ҫ����
    char m_acVersion[MAX_VERSION_LEN];    //�汾��Ϣ
    int m_iTraceLevelThreshold;
    int m_iBakMode;                        //���ݷ�ʽ 0 - ���ļ��������� ��1- ���ñ��� , > 2 ѭ������
    int    m_iIdx;

}; 




#endif //_TLOG_H_
