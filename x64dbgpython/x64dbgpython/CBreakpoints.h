#pragma once
#include <Windows.h>
#include <string>


class CBreakpoint
{
public:
	CBreakpoint();
	~CBreakpoint();
	bool AddScript(char*);
	std::string* GetScriptContent();
private:
	std::string script;
};