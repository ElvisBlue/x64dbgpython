#include "plugin.h"
#include "pybind11\embed.h"
#include "pybind11\iostream.h"
#include "pystream.h"

namespace py = pybind11;
CPystream pPystream;
bool g_IsScriptRunning = false;
HANDLE g_hThread = NULL;

//Function define
bool OpenFileDialog(char*, size_t);

enum menu_entry
{
    MENU_RUN_SCRIPT,
    MENU_ABOUT
};

void __stdcall PyExecuteFileThread(char* fileBuffer)
{
    g_IsScriptRunning = true;
    try
    {
        py::object scope = py::module_::import("__main__").attr("__dict__");
        py::eval_file(fileBuffer, scope);
    }
    catch (py::error_already_set& e)
    {
        _plugin_logprint(e.what());
        g_IsScriptRunning = false;
        free(fileBuffer);
    }
    g_IsScriptRunning = false;

    //fileBuffer is dynamic memory allocated by malloc
    //Need to be free before exit thread
    free(fileBuffer);
}

void PluginHandleMenuCommand(CBTYPE cbType, PLUG_CB_MENUENTRY* info)
{
    switch (info->hEntry)
    {
    case menu_entry::MENU_RUN_SCRIPT:
    {
        char* fileBuffer = (char*)malloc(MAX_PATH);
        if (fileBuffer)
        {
            memset(fileBuffer, 0, MAX_PATH);
            if (g_IsScriptRunning)
            {
                MessageBoxA(g_hwndDlg, "Another script is running", PLUGIN_NAME, MB_OK | MB_ICONEXCLAMATION);
                break;
            }
            if (OpenFileDialog(fileBuffer, sizeof(fileBuffer)))
            {
                g_hThread = CreateThread(NULL, NULL, (LPTHREAD_START_ROUTINE)PyExecuteFileThread, fileBuffer, NULL, NULL);
            }
        }
        break;
    }
    case menu_entry::MENU_ABOUT:
        MessageBoxA(g_hwndDlg,  PLUGIN_NAME" by Elvis\n"
                                "Warning: This plugin contains a lot of bugs", PLUGIN_NAME, MB_ICONINFORMATION);
        break;
    default:
        break;
    }
}

void __stdcall PyCommandExecuteThread(char* cmd)
{
    g_IsScriptRunning = true;
    try
    {
        py::exec(cmd);
    }
    catch (py::error_already_set& e)
    {
        _plugin_logprint(e.what());
        g_IsScriptRunning = false;
        free(cmd);
    }
    g_IsScriptRunning = false;

    //cmd is dynamic memory allocated by malloc
    //Need to be free before exit thread
    free(cmd);
}

bool PythonCommandExecute(const char* cmd)
{
    if (cmd)
    {
        size_t cmdLength = strlen(cmd);
        char* newCmdBuffer = (char*)malloc(cmdLength + 1);
        if (newCmdBuffer)
        {
            memset(newCmdBuffer, 0, cmdLength + 1);
            strcpy_s(newCmdBuffer, cmdLength + 1, cmd);
            g_hThread = CreateThread(NULL, NULL, (LPTHREAD_START_ROUTINE)PyCommandExecuteThread, (LPVOID)newCmdBuffer, NULL, NULL);
            return true;
        }
        else
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

    //start the interpreter
    py::initialize_interpreter();

    //Redirect output to x64dbg log windows
    py::module::import("sys_pystream");
    py::module::import("sys").attr("stdout") = pPystream;
    py::module::import("sys").attr("stderr") = pPystream;

    return true;
}

//Deinitialize your plugin data here.
void pluginStop()
{
    py::finalize_interpreter();
}

//Do GUI/Menu related things here.
void pluginSetup()
{
    _plugin_menuaddentry(g_hMenu, menu_entry::MENU_RUN_SCRIPT, "&Run Script");
    _plugin_menuaddseparator(g_hMenu);
    _plugin_menuaddentry(g_hMenu, menu_entry::MENU_ABOUT, "&About");

    //Set hotkey
    _plugin_menuentrysethotkey(g_pluginHandle, menu_entry::MENU_RUN_SCRIPT, "Alt+F7");
}

bool OpenFileDialog(char* buffer, size_t bufferSize)
{
    OPENFILENAMEA sOpenFileName = { 0 };
    ZeroMemory(buffer, bufferSize);

    sOpenFileName.lStructSize = sizeof(sOpenFileName);
    sOpenFileName.lpstrFilter = "Python files\0*.py\0All\0*.*\0";
    sOpenFileName.nFilterIndex = 1;
    sOpenFileName.lpstrFile = buffer;
    sOpenFileName.nMaxFile = MAX_PATH;
    sOpenFileName.Flags = OFN_PATHMUSTEXIST | OFN_FILEMUSTEXIST;
    sOpenFileName.lpstrTitle = "Select script file...";
    sOpenFileName.lpstrFileTitle = NULL;
    sOpenFileName.nMaxFileTitle = 0;
    sOpenFileName.lpstrInitialDir = NULL;
    sOpenFileName.hwndOwner = g_hwndDlg;
    return (FALSE != GetOpenFileNameA(&sOpenFileName));
}

PYBIND11_EMBEDDED_MODULE(sys_pystream, module)
{
    py::class_<CPystream>(module, "Writer")
        .def("write", &CPystream::write)
        .def("flush", &CPystream::flush);
}