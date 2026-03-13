#include "wrapper.h"

void InitWrapperPattern(py::module_& m)
{
	//Implement module Pattern
	py::module mPattern = m.def_submodule("Pattern", "x64dbg Pattern python script wrapper");
	mPattern.def("Find", &Pattern::Find);
	mPattern.def("FindMem", &Pattern::FindMem);
	mPattern.def("Write", &Pattern::Write);
	mPattern.def("WriteMem", &Pattern::WriteMem);
	mPattern.def("SearchAndReplace", &Pattern::SearchAndReplace);
	mPattern.def("SearchAndReplaceMem", &Pattern::SearchAndReplaceMem);


}
