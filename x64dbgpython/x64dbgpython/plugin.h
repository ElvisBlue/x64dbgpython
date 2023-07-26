#pragma once

#include "pluginmain.h"

//functions
bool pluginInit(PLUG_INITSTRUCT* initStruct);
void pluginStop();
void pluginSetup();

void PluginHandleMenuCommand(CBTYPE cbType, PLUG_CB_MENUENTRY* info);
void PluginHandleBreakpoint(CBTYPE cbType, PLUG_CB_BREAKPOINT* info);
void PluginHandleLoadDB(CBTYPE cbType, PLUG_CB_LOADSAVEDB* info);
void PluginHandleSaveDB(CBTYPE cbType, PLUG_CB_LOADSAVEDB* info);
