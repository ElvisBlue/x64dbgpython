#include "wrapper.h"

namespace PyWrapper
{
		namespace Stack
		{
			std::optional<std::vector<duint>> SelectionGet()
			{
				duint start = 0;
				duint end = 0;
				if (Script::Gui::Stack::SelectionGet(&start, &end))
				{
					std::vector<duint> ret;
					ret.push_back(start);
					ret.push_back(end);
					return ret;
				}
				return std::nullopt;
			}
		}

}

void InitWrapperStack(py::module_& m)
{
	//Implement module Stack
	py::module mStack = m.def_submodule("Stack", "x64dbg Stack python script wrapper");

	mStack.def("Pop", &Stack::Pop);
	mStack.def("Push", &Stack::Push);
	mStack.def("Peek", &Stack::Peek);


}
