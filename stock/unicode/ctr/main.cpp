
#include <afx.h>

#include <stdio.h>

#include "KbdMouseLib.h"
#include "WndLib.h"
#include "TclExtCmd.h"
#include "Log.h"

extern void DebugKbdMouseLib(void);
extern void DebugWndLib(void);
extern void DebugStockTranslate(void);
extern void DebugStockChip(void);
extern void StockMain(void);


int main(int argc, char *argv[])
{
    g_Log = LogTraceOpen("Stock", "V100R001C01B001", 5, LOG_TO_FILE);
    if (NULL == g_Log)
    {
        printf("Open Log file failed\n");
        return -1;
    }
    
    printf("Hello\n");

    //StockMain();
    
    //DebugStockChip();
    //DebugStockTranslate();
    //DebugKbdMouseLib();
    DebugWndLib();
    
    
    //ExecuteScriptAndFile(NULL, "paint.tcl");
    //ExecuteScriptAndFile("set hwnd 199508", "paint.tcl");

    LogTraceClose(g_Log);
    system("Pause");
    return 0;
}
