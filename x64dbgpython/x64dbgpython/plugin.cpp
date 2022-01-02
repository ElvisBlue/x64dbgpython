#include "plugin.h"

//Initialize your plugin data here.
bool PythonCommandExecuteCallback(const char* cmd)
{
    if (cmd)
    {
        return true;
    }
    return false;
}


bool pluginInit(PLUG_INITSTRUCT* initStruct)
{
    //Register python script language
    SCRIPTTYPEINFO info;
    strcpy_s(info.name, "Python");
    info.id = 0;
    info.execute = PythonCommandExecuteCallback;
    info.completeCommand = nullptr;
    GuiRegisterScriptLanguage(&info);
    return true; //Return false to cancel loading the plugin.
}

//Deinitialize your plugin data here.
void pluginStop()
{
}

//Do GUI/Menu related things here.
void pluginSetup()
{
}
