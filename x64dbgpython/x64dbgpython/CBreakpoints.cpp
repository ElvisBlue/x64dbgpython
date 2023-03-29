#include "CBreakpoints.h"

CBreakpoint::CBreakpoint()
{

}

CBreakpoint::~CBreakpoint()
{

}

bool CBreakpoint::AddScript(char* script)
{
	this->script = script;
	return true;
}

std::string* CBreakpoint::GetScriptContent()
{
	return &this->script;
}