#include "wrapper.h"

namespace PyWrapper
{
	namespace Misc
	{
		duint ParseExpression(std::string expression)
		{
			duint value = 0;
			Script::Misc::ParseExpression(expression.c_str(), &value);
			return value;
		}
	}

}

void init_wrapper_misc(py::module_& m)
{
	//Implement module Misc
	py::module mMisc = m.def_submodule("Misc", "x64dbg Misc python script wrapper");

	mMisc.def("ParseExpression", &PyWrapper::Misc::ParseExpression);
	mMisc.def("RemoteGetProcAddress", &Misc::RemoteGetProcAddress);
	mMisc.def("ResolveLabel", &Misc::ResolveLabel);
	mMisc.def("Alloc", &Misc::Alloc);
	mMisc.def("Free", &Misc::Free);


}
