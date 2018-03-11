
#include <afx.h>

#include <stdio.h>

#include "osp_globals.h"
#include "KbdMouseLib.h"
#include "WndLib.h"
#include "TclExtCmd.h"
#include "Log.h"
#include "inifile.h"
#include "Stock.h"

extern OSP_VOID DebugKbdMouseLib(OSP_VOID);
extern OSP_VOID DebugWndLib(OSP_VOID);
extern OSP_VOID DebugStockTranslate(OSP_VOID);
extern OSP_VOID DebugStockChip(OSP_VOID);
extern OSP_S32 StockMain(OSP_VOID);
extern OSP_VOID DebugStockWatch(OSP_VOID);

OSP_S32 main(OSP_S32 argc, OSP_S8 *argv[])
{
    int iLogLevel = 0;
    
    printf("Welcome to Stock System\n");
    
    iLogLevel = IniReadInt(STOCK_CONFIG_FILE, "SoftWare",
        "LOG_LEVEL", 4);
    printf("Read config file success. [%s, %s, %s, %d]\n",
        STOCK_CONFIG_FILE, "SoftWare", "LOG_LEVEL",
        iLogLevel);
    
    g_Log = LogTraceOpen("Stock", "V100R001C01B001", iLogLevel, LOG_TO_FILE);
    if (NULL == g_Log)
    {
        printf("Open Log file failed\n");
        return -1;
    }


    StockMain();
    //while (1)
    //{
       // KeepAliveWatch(0);
       // Sleep(2000);
    //}
    
    //DebugStockChip();
    //DebugStockWatch();
    //DebugStockTranslate();
    //DebugKbdMouseLib();
    //DebugWndLib();
    //DebugIniFile();
    
    //ExecuteScriptAndFile(NULL, "paint.tcl");
    //ExecuteScriptAndFile("set hwnd 199508", "paint.tcl");

    LogTraceClose(g_Log);
    system("Pause");
    return 0;
}
