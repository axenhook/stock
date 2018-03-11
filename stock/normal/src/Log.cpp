#include "tlog.h"
#include "Log.h"

/*******************************************************************************
函数名称: LogTraceOpen
功能说明: 无
输入参数: 无
输出参数: 无
返 回 值: 无
说    明: 无
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
函数名称: LogTraceClose
功能说明: 无
输入参数: 无
输出参数: 无
返 回 值: 无
说    明: 无
*******************************************************************************/
void LogTraceClose(TLog *log)
{
    log->close();
    delete log;
} /* End of LogTraceClose */

/*******************************************************************************
函数名称: LogTraceSetLevel
功能说明: 无
输入参数: 无
输出参数: 无
返 回 值: 无
说    明: 无
*******************************************************************************/
void LogTraceSetLevel(TLog *lh, int logLevel)
{
    lh->SetTraceLevelThreshold(logLevel);
} /* End of LogTraceSetLevel */

/*******************************************************************************
函数名称: DebugLog
功能说明: 无
输入参数: 无
输出参数: 无
返 回 值: 无
说    明: 无
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


