#include <map>
#include <string>
#include "resource1.h"
#include "BreakpointDlg.h"
#include "plugin.h"
#include "pybind11\embed.h"

namespace py = pybind11;

std::map<duint, std::string> g_BreakpointList;

//Plugin breakpoint callback handle
void PluginHandleBreakpoint(CBTYPE cbType, PLUG_CB_BREAKPOINT* info)
{
	duint breakAddr = info->breakpoint->addr;
	std::map<duint, std::string>::const_iterator iter = g_BreakpointList.find(breakAddr);
	if (iter != g_BreakpointList.end())
	{
		py::exec(iter->second.c_str());
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
					BPMAP bpList;
					DbgGetBpList(bp_none, &bpList);
					duint bpAddr = bpList.bp[pos].addr;
					char dCommand[20];
					sprintf_s(dCommand, "d %p", (void*)bpAddr);
					DbgCmdExecDirect(dCommand);
				}
			}
			else if (HIWORD(wParam) == LBN_SELCHANGE)
			{

			}
		}
		break;
	}
	default:
		break;
	}
	return false;
}