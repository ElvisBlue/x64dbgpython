#include "scriptapi/wrapper.h"

PYBIND11_EMBEDDED_MODULE(x64dbg, m)
{
	m.doc() = "Python module to wrap the x64dbg script api.";
	InitWrapperArgument(m);
	InitWrapperAssembler(m);
	InitWrapperBookmark(m);
	InitWrapperComment(m);
	InitWrapperDebug(m);
	InitWrapperFlag(m);
	InitWrapperFunction(m);
	InitWrapperGui(m);
	InitWrapperLabel(m);
	InitWrapperMemory(m);
	InitWrapperMisc(m);
	InitWrapperModule(m);
	InitWrapperPattern(m);
	InitWrapperRegister(m);
	InitWrapperStack(m);
	InitWrapperSymbol(m);
}
