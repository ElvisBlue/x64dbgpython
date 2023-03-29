#include "resource.h"
#include "BreakpointDlg.h"
#include "plugin.h"

std::vector<CBreakpoint*> g_BreakpointList;

//Plugin breakpoint callback handle
void PluginHandleBreakpoint(CBTYPE cbType, PLUG_CB_BREAKPOINT* info)
{
	return;
}

INT_PTR CALLBACK BreakpointDlgProc(HWND hwndDlg, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	switch (uMsg)
	{
	case WM_CLOSE:
	{
		EndDialog(hwndDlg, 0);
	}
	default:
		break;
	}
	return false;
}