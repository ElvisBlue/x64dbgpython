#pragma once
#include "pluginmain.h"
#include <iostream>

//https://stackoverflow.com/questions/67784850/how-to-redirect-python-stdout-to-log-file-by-using-pybind-11-in-c
class CPystream
{
public:
    void write(const char* str)
    {
        _plugin_logprint(str);
    }

    void flush()
    {
        _plugin_logprint("\n");
    }
};