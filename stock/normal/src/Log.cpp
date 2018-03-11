#include "tlog.h"
#include "Log.h"

/*******************************************************************************
��������: LogTraceOpen
����˵��: ��
�������: ��
�������: ��
�� �� ֵ: ��
˵    ��: ��
*******************************************************************************/
TLog *LogTraceOpen(const char *fileName, const char *version,
    int logLevel, int logType)
{
    TLog *log = new TLog(fileName, version);
    if (log != NULL)
    {
        log->open(logType);
        log->SetTraceLevelThreshold(logLevel);
    }
    
    return log;
} /* End of LogTraceOpen */

/*******************************************************************************
��������: LogTraceClose
����˵��: ��
�������: ��
�������: ��
�� �� ֵ: ��
˵    ��: ��
*******************************************************************************/
void LogTraceClose(TLog *log)
{
    log->close();
    delete log;
} /* End of LogTraceClose */

/*******************************************************************************
��������: LogTraceSetLevel
����˵��: ��
�������: ��
�������: ��
�� �� ֵ: ��
˵    ��: ��
*******************************************************************************/
void LogTraceSetLevel(TLog *lh, int logLevel)
{
    lh->SetTraceLevelThreshold(logLevel);
} /* End of LogTraceSetLevel */

/*******************************************************************************
��������: DebugLog
����˵��: ��
�������: ��
�������: ��
�� �� ֵ: ��
˵    ��: ��
*******************************************************************************/
void DebugLog(void)
{
    int i;
    TLog *log = LogTraceOpen("test", "V100R001C01", 5, LOG_TO_FILE);
    assert(log);
    
    i = 0;
    i++;
    LogTraceEmerg(log, "Test i. [%d]\n", i);
    i++;
    LogTraceError(log, "Test i. [%d]\n", i);
    i++;
    LogTraceWarning(log, "Test i. [%d]\n", i);
    i++;
    LogTraceEvent(log, "Test i. [%d]\n", i);
    i++;
    LogTraceDebug(log, "Test i. [%d]\n", i);
    
   // LogTraceDebug(log, "Test i. [%d, %s]\n", i, __FUNCTION__);
    LogTraceClose(log);
} /* End of DebugLog */

TLog *g_Log = NULL;


