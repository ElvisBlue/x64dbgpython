#include "wrapper.h"

namespace PyWrapper
{
	namespace Assembler
	{
		py::object Assemble(duint addr, const char* instruction)
		{
			unsigned char dest[16] = { 0 };
			int size = 0;
			if (Script::Assembler::Assemble(addr, dest, &size, instruction))
				return py::bytes((char*)dest, size);
			else
				return py::none();
		}

		py::object AssembleEx(duint addr, const char* instruction)
		{
			//Crash. Dunno why??
			//TODO: Check this
			char retError[MAX_ERROR_SIZE] = {0};
			unsigned char dest[16] = { 0 };
			int size = 0;
			if (Script::Assembler::AssembleEx(addr, dest, &size, instruction, retError))
				return py::bytes((char*)dest, size);
			else
			{
				_plugin_logprintf("AssembleEx error: %s\n", retError);
				return py::none();
			}
		}

		bool AssembleMemEx(duint addr, const char* instruction, bool fillnop)
		{
			char retError[MAX_ERROR_SIZE] = {0};
			int size = 0;
			if (Script::Assembler::AssembleMemEx(addr, instruction, &size, retError, fillnop))
				return true;
			else
			{
				_plugin_logprintf("AssembleMemEx error: %s\n", retError);
				return false;
			}
		}
	}

}

void InitWrapperAssembler(py::module_& m)
{
	//Implement python module Assembler
	py::module mAssembler = m.def_submodule("Assembler", "x64dbg Assembler python script wrapper");
	mAssembler.def("Assemble", &PyWrapper::Assembler::Assemble);
	mAssembler.def("AssembleEx", &PyWrapper::Assembler::AssembleEx);
	mAssembler.def("AssembleMem", &Assembler::AssembleMem);
	mAssembler.def("AssembleMemEx", &PyWrapper::Assembler::AssembleMemEx);


}
