#include "pluginmain.h"
#include "plugin.h"

int g_pluginHandle;
HWND g_hwndDlg;
int g_hMenu;
int g_hMenuDisasm;
int g_hMenuDump;
int g_hMenuStack;

PLUG_EXPORT void CBMENUENTRY(CBTYPE cbType, PLUG_CB_MENUENTRY* info)
{
    PluginHandleMenuCommand(cbType, info);
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