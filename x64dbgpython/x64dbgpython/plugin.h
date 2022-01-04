#pragma once

#include "pluginmain.h"

//functions
bool pluginInit(PLUG_INITSTRUCT* initStruct);
void pluginStop();
void pluginSetup();

void PluginHandleMenuCommand(CBTYPE cbType, PLUG_CB_MENUENTRY* info);