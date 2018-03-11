#ifndef _LOG_H_
#define _LOG_H_

#include "tlog.h"

extern TLog *LogTraceOpen(const char *fileName, const char *version,
    int logLevel, int logType);
extern void LogTraceClose(TLog *log);
extern void LogTraceSetLevel(TLog *log, int logLevel);


    #ifdef WIN32 // Windows

        #if _MSC_VER <= 1200 // VC6.0
    
class MyTracer
{
    public:
        MyTracer(int level, const char *prefix, const char *file, int line)
            :_level(level), _prefix(prefix), _file(file), _line(line)
        {
        
        }
        
        void operator()(TLog *log, const char *fmt, ...)
        {
            va_list ap;
            char buffer[1024];
            
            va_start(ap, fmt);
            memset(buffer, 0, sizeof(buffer));
            _vsnprintf(buffer, 1023, fmt, ap);
            assert(log);
            log->trace(_level, "[%s][%s:%ld][%d]: %s", _prefix, _file,
                GetCurrentThreadId(), _line, buffer);
            va_end(ap);
        }
    private:
        // copy-ctor and operator=
        MyTracer(const MyTracer&);
        MyTracer& operator=(const MyTracer&);
        
        const char *_file;
        const char *_prefix;
        int _level;
        int _line;
};
    
#define LogTraceDebug    (MyTracer(4, "DEBUG", __FILE__, __LINE__))
#define LogTraceEvent    (MyTracer(3, "EVENT", __FILE__, __LINE__))
#define LogTraceWarning  (MyTracer(2, "WARNING", __FILE__, __LINE__))
#define LogTraceError    (MyTracer(1, "ERROR", __FILE__, __LINE__))
#define LogTraceEmerg    (MyTracer(0, "EMERG", __FILE__, __LINE__))
    
        #else // > VC6.0
    
#define LogTraceDebug(log, fmt, ...) \
{ \
    assert(NULL != log); \
    log->trace(4, "[DEBUG][%s:%ld][%s:%d]: "fmt, __FILE__, \
        GetCurrentThreadId(), __FUNCTION__, __LINE__, __VA_ARGS__); \
}
    
#define LogTraceEvent(log, fmt, ...) \
{ \
    assert(NULL != log); \
    log->trace(3, "[EVENT][%s:%ld][%s:%d]: "fmt, __FILE__, \
        GetCurrentThreadId(), __FUNCTION__, __LINE__, __VA_ARGS__); \
}
    
#define LogTraceWarning(log, fmt, ...) \
{ \
    assert(NULL != log); \
    log->trace(2, "[WARNING][%s:%ld][%s:%d]: "fmt, __FILE__, \
        GetCurrentThreadId(), __FUNCTION__, __LINE__, __VA_ARGS__); \
}
    
#define LogTraceError(log, fmt, ...) \
{ \
    assert(NULL != log); \
    log->trace(1, "[ERROR][%s:%ld][%s:%d]: "fmt, __FILE__, \
        GetCurrentThreadId(), __FUNCTION__, __LINE__, __VA_ARGS__); \
}
    
#define LogTraceEmerg(log, fmt, ...) \
{ \
    assert(NULL != log); \
    log->trace(0, "[EMERG][%s:%ld][%s:%d]: "fmt, __FILE__, \
        GetCurrentThreadId(), __FUNCTION__, __LINE__, __VA_ARGS__); \
}

        #endif

    #else // Linux

#define LogTraceDebug(log, fmt, ...) \
{ \
    assert(NULL != log); \
    log->trace(4, "[DEBUG][%s:%ld][%s:%d]: "fmt, __FILE__, \
        pthread_self(), __FUNCTION__, __LINE__, ##args);
}

#define LogTraceEvent(log, fmt, ...) \
{ \
    assert(NULL != log); \
    log->trace(3, "[EVENT][%s:%ld][%s:%d]: "fmt, __FILE__, \
        pthread_self(), __FUNCTION__, __LINE__, ##args);
}

#define LogTraceWarning(log, fmt, ...) \
{ \
    assert(NULL != log); \
    log->trace(2, "[WARNING][%s:%ld][%s:%d]: "fmt, __FILE__, \
        pthread_self(), __FUNCTION__, __LINE__, ##args);
}

#define LogTraceError(log, fmt, ...) \
{ \
    assert(NULL != log); \
    log->trace(1, "[ERROR][%s:%ld][%s:%d]: "fmt, __FILE__, \
        pthread_self(), __FUNCTION__, __LINE__, ##args);
}

#define LogTraceEmerg(log, fmt, ...) \
{ \
    assert(NULL != log); \
    log->trace(0, "[EMERG][%s:%ld][%s:%d]: "fmt, __FILE__, \
        pthread_self(), __FUNCTION__, __LINE__, ##args);
}

    #endif /* End of _LOG_H_ */


extern void DebugLog(void);
extern TLog *g_Log;

#endif
