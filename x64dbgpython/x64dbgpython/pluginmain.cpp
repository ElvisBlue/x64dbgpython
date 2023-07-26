#include "pluginmain.h"
#include "plugin.h"

int g_pluginHandle;
HWND g_hwndDlg;
int g_hMenu;
int g_hMenuDisasm;
int g_hMenuDump;
int g_hMenuStack;
HINSTANCE g_dllInstance;

PLUG_EXPORT void CBMENUENTRY(CBTYPE cbType, PLUG_CB_MENUENTRY* info)
{
    PluginHandleMenuCommand(cbType, info);
}

PLUG_EXPORT void CBBREAKPOINT(CBTYPE cbType, PLUG_CB_BREAKPOINT* info)
{
    PluginHandleBreakpoint(cbType, info);
}

PLUG_EXPORT void CBLOADDB(CBTYPE cbType, PLUG_CB_LOADSAVEDB* info)
{
    PluginHandleLoadDB(cbType, info);
}

PLUG_EXPORT void CBSAVEDB(CBTYPE cbType, PLUG_CB_LOADSAVEDB* info)
{
    PluginHandleSaveDB(cbType, info);
}

PLUG_EXPORT bool pluginit(PLUG_INITSTRUCT* initStruct)
{
    initStruct->pluginVersion = PLUGIN_VERSION;
    initStruct->sdkVersion = PLUG_SDKVERSION;
    strncpy_s(initStruct->pluginName, PLUGIN_NAME, _TRUNCATE);
    g_pluginHandle = initStruct->pluginHandle;
    return pluginInit(initStruct);
}

PLUG_EXPORT bool plugstop()
{
    pluginStop();
    return true;
}

PLUG_EXPORT void plugsetup(PLUG_SETUPSTRUCT* setupStruct)
{
    g_hwndDlg = setupStruct->hwndDlg;
    g_hMenu = setupStruct->hMenu;
    g_hMenuDisasm = setupStruct->hMenuDisasm;
    g_hMenuDump = setupStruct->hMenuDump;
    g_hMenuStack = setupStruct->hMenuStack;
    pluginSetup();
}

BOOL WINAPI DllMain(HINSTANCE hinstDLL, DWORD fdwReason, LPVOID lpvReserved)
{
    if (fdwReason == DLL_PROCESS_ATTACH)
        g_dllInstance = hinstDLL;
    return TRUE;
}