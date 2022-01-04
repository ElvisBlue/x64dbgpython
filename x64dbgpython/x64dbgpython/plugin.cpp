

#include "plugin.h"
#include "pybind11\embed.h"
#include "pybind11\iostream.h"
#include "pystream.h"

namespace py = pybind11;
py::scoped_interpreter* pGuard = nullptr;
CPystream pPystream;

//Function define
bool OpenFileDialog(char Buffer[MAX_PATH]);

enum menu_entry
{
    MENU_RUN_SCRIPT,
    MENU_ABOUT
};

void PluginHandleMenuCommand(CBTYPE cbType, PLUG_CB_MENUENTRY* info)
{
    switch (info->hEntry)
    {
    case menu_entry::MENU_RUN_SCRIPT:
        char fileBuffer[MAX_PATH];
        if (OpenFileDialog(fileBuffer))
        {
            try
            {
                py::object scope = py::module_::import("__main__").attr("__dict__");
                py::eval_file(fileBuffer, scope);
            }
            catch (py::error_already_set& e)
            {
                _plugin_logprint(e.what());
            }
        }
        else
            MessageBox(NULL, L"Failed to open python script file", L"Error", MB_OK | MB_ICONEXCLAMATION);
        break;
    case menu_entry::MENU_ABOUT:
        MessageBoxA(g_hwndDlg, "x64dbg Python by Elvis", PLUGIN_NAME " Plugin", MB_ICONINFORMATION);
        break;
    default:
        break;
    }
}

bool PythonCommandExecute(const char* cmd)
{
    if (cmd)
    {
        try
        {
            py::exec(cmd);
        }
        catch (py::error_already_set& e)
        {
            _plugin_logprint(e.what());
        }
        return true;
    }
    return false;
}

//Initialize your plugin data here.
bool pluginInit(PLUG_INITSTRUCT* initStruct)
{

    //Register python script language
    SCRIPTTYPEINFO info;
    strcpy_s(info.name, "Python3");
    info.id = 0;
    info.execute = PythonCommandExecute;
    info.completeCommand = nullptr;
    GuiRegisterScriptLanguage(&info);

    //start the interpreter and keep it alive
    pGuard = new py::scoped_interpreter{};

    //Redirect output to x64dbg log windows
    py::module::import("sys_pystream");
    py::module::import("sys").attr("stdout") = pPystream;
    py::module::import("sys").attr("stderr") = pPystream;


    //py::scoped_ostream_redirect stream(_plugin_logprint, py_stdout);
    return true;
}

//Deinitialize your plugin data here.
void pluginStop()
{
    if (pGuard)
    {
        delete pGuard;
        pGuard = nullptr;
    }
}

//Do GUI/Menu related things here.
void pluginSetup()
{
    _plugin_menuaddentry(g_hMenu, menu_entry::MENU_RUN_SCRIPT, "&Run Script");
    _plugin_menuaddentry(g_hMenu, menu_entry::MENU_ABOUT, "&About");
}

bool OpenFileDialog(char Buffer[MAX_PATH])
{
    OPENFILENAMEA sOpenFileName = { 0 };
    const char szFilterString[] = "Python files\0*.py\0\0";
    const char szDialogTitle[] = "Select script file...";
    sOpenFileName.lStructSize = sizeof(sOpenFileName);
    sOpenFileName.lpstrFilter = szFilterString;
    sOpenFileName.nFilterIndex = 1;
    sOpenFileName.lpstrFile = Buffer;
    sOpenFileName.nMaxFile = MAX_PATH;
    sOpenFileName.Flags = OFN_PATHMUSTEXIST | OFN_FILEMUSTEXIST;
    sOpenFileName.lpstrTitle = szDialogTitle;
    sOpenFileName.hwndOwner = GuiGetWindowHandle();
    return (FALSE != GetOpenFileNameA(&sOpenFileName));
}

PYBIND11_EMBEDDED_MODULE(x64dbg, m) 
{
    // `m` is a `py::module_` which is used to bind functions and classes
    m.def("log", _plugin_logprint);
}

PYBIND11_EMBEDDED_MODULE(sys_pystream, module)
{
    py::class_<CPystream>(module, "Writer")
        .def("write", &CPystream::write)
        .def("flush", &CPystream::flush);
}