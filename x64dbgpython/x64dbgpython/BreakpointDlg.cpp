#include <map>
#include <string>
#include "resource1.h"
#include "BreakpointDlg.h"
#include "plugin.h"
#include "pybind11\embed.h"

namespace py = pybind11;

std::map<duint, std::string> g_BreakpointList;

duint GetBpAtIndex(int index)
{
	BPMAP bpList;
	DbgGetBpList(bp_none, &bpList);
	return bpList.bp[index].addr;
}

//Plugin breakpoint callback handle
void PluginHandleBreakpoint(CBTYPE cbType, PLUG_CB_BREAKPOINT* info)
{
	duint breakAddr = info->breakpoint->addr;
	std::map<duint, std::string>::const_iterator iter = g_BreakpointList.find(breakAddr);
	if (iter != g_BreakpointList.end())
	{
		try
		{
			py::exec(iter->second.c_str());
		}
		catch (py::error_already_set& e)
		{
			_plugin_logprint(e.what());
		}
	}
}

INT_PTR CALLBACK BreakpointDlgProc(HWND hwndDlg, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	switch (uMsg)
	{
		case WM_CLOSE:
		{
			EndDialog(hwndDlg, 0);
			break;
		}
		case WM_INITDIALOG:
		{
			HWND bpListHwnd = GetDlgItem(hwndDlg, IDC_BPLIST);
			BPMAP bpList;
			DbgGetBpList(bp_none, &bpList);

			for (int i = 0; i < bpList.count; i++)
			{
				wchar_t bpAddrText[20];

				if (g_BreakpointList.find(bpList.bp[i].addr) != g_BreakpointList.end())
					swprintf(bpAddrText, sizeof(bpAddrText) / 2, L"%p*", (void*)bpList.bp[i].addr);
				else
					swprintf(bpAddrText, sizeof(bpAddrText) / 2, L"%p", (void*)bpList.bp[i].addr);
				SendMessage(bpListHwnd, LB_ADDSTRING, NULL, (LPARAM)bpAddrText);
			}
			break;
		}
		case WM_COMMAND:
		{
			if (LOWORD(wParam) == IDC_BPLIST)
			{
				if (HIWORD(wParam) == LBN_DBLCLK)
				{
					HWND bpListHwnd = GetDlgItem(hwndDlg, LOWORD(wParam));
					int pos = (int)SendMessage(bpListHwnd, LB_GETCURSEL, 0, 0);
					if (pos != -1)
					{
						duint bpAddr = GetBpAtIndex(pos);
						char dCommand[20];
						sprintf_s(dCommand, "d %p", (void*)bpAddr);
						DbgCmdExecDirect(dCommand);
					}
				}
				else if (HIWORD(wParam) == LBN_SELCHANGE)
				{
					HWND bpListHwnd = GetDlgItem(hwndDlg, LOWORD(wParam));
					int pos = (int)SendMessage(bpListHwnd, LB_GETCURSEL, 0, 0);
					if (pos != -1)
					{
						duint bpAddr = GetBpAtIndex(pos);
						std::map<duint, std::string>::const_iterator iter = g_BreakpointList.find(bpAddr);
						if (iter != g_BreakpointList.end())
						{
							SetDlgItemTextA(hwndDlg, IDC_SCRIPTCONTENT, iter->second.c_str());
						}
						else
						{
							SetDlgItemTextA(hwndDlg, IDC_SCRIPTCONTENT, "");
						}
					}
				}
			}
			else if (LOWORD(wParam) == IDC_EDITSCRIPT)
			{
				if (HIWORD(wParam) == BN_CLICKED)
				{
					HWND bpListHwnd = GetDlgItem(hwndDlg, IDC_BPLIST);
					int pos = (int)SendMessage(bpListHwnd, LB_GETCURSEL, 0, 0);
					if (pos != -1)
					{
						DialogBoxParam(g_dllInstance, MAKEINTRESOURCE(IDD_SCRIPTEDITOR), hwndDlg, ScriptEditorProc, (LPARAM)pos);
						duint bpAddr = GetBpAtIndex(pos);
						std::map<duint, std::string>::const_iterator iter = g_BreakpointList.find(bpAddr);
						if (iter != g_BreakpointList.end())
						{
							SetDlgItemTextA(hwndDlg, IDC_SCRIPTCONTENT, iter->second.c_str());
						}
						else
						{
							SetDlgItemTextA(hwndDlg, IDC_SCRIPTCONTENT, "");
						}
					}
				}
			}
			break;
		}
		default:
			break;
	}
	return false;
}

int g_CurrentBpSelection;

INT_PTR CALLBACK ScriptEditorProc(HWND hwndDlg, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	switch (uMsg)
	{
		case WM_INITDIALOG:
		{
			g_CurrentBpSelection = (int)lParam;
			duint bpAddr = GetBpAtIndex(g_CurrentBpSelection);;
			std::map<duint, std::string>::const_iterator iter = g_BreakpointList.find(bpAddr);
			if (iter != g_BreakpointList.end())
			{
				SetDlgItemTextA(hwndDlg, IDC_SCRIPT, iter->second.c_str());
			}
			break;
		}
		case WM_CLOSE:
		{
			EndDialog(hwndDlg, 0);
			break;
		}
		case WM_COMMAND:
		{
			if (LOWORD(wParam) == IDC_SAVESCRIPT)
			{
				if (HIWORD(wParam) == BN_CLICKED)
				{
					duint bpAddr = GetBpAtIndex(g_CurrentBpSelection);

					HWND scriptTextHwnd = GetDlgItem(hwndDlg, IDC_SCRIPT);
					unsigned int scriptLength = GetWindowTextLengthA(scriptTextHwnd);
					char* scriptContent = new char[scriptLength + 2];
					GetDlgItemTextA(hwndDlg, IDC_SCRIPT, scriptContent, scriptLength + 1);

					std::map<duint, std::string>::const_iterator iter = g_BreakpointList.find(bpAddr);
					if (iter != g_BreakpointList.end())
					{
						g_BreakpointList.erase(iter);
					}

					if (scriptLength)
						g_BreakpointList.insert({ bpAddr, std::string(scriptContent) });

					delete[] scriptContent;

					EndDialog(hwndDlg, 0);
				}
			}
		}
		default:
			break;
	}

	return false;
}