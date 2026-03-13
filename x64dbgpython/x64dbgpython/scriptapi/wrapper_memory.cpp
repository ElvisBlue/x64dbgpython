#include "wrapper.h"

namespace PyWrapper
{
	namespace Memory
	{
		py::object Read(duint addr, duint size)
		{
			unsigned char* readData = (unsigned char*)malloc(size);
			if (readData == nullptr)
				return py::none();

			memset(readData, 0, size);
			duint sizeRead = 0;
			py::object ret;
			if (Script::Memory::Read(addr, readData, size, &sizeRead))
				ret = py::bytes((char*)readData, sizeRead);
			else
				ret = py::none();
			
			free(readData);
			return ret;
		}

		duint Write(duint addr, std::string data)
		{
			duint sizeWritten = 0;
			if (Script::Memory::Write(addr, data.c_str(), data.size(), &sizeWritten))
			{
				return sizeWritten;
			}
			else
				return 0;
		}
	}

}

void InitWrapperMemory(py::module_& m)
{
	//Implement module Memory
	py::module mMemory = m.def_submodule("Memory", "x64dbg Memory python script wrapper");

	mMemory.def("Read", &PyWrapper::Memory::Read);
	mMemory.def("Write", &PyWrapper::Memory::Write);
	mMemory.def("IsValidPtr", &Memory::IsValidPtr);
	mMemory.def("RemoteAlloc", &Memory::RemoteAlloc);
	mMemory.def("RemoteFree", &Memory::RemoteFree);
	mMemory.def("GetProtect", &Memory::GetProtect);
	mMemory.def("GetBase", &Memory::GetBase);
	mMemory.def("GetSize", &Memory::GetSize);
	mMemory.def("ReadByte", &Memory::ReadByte);
	mMemory.def("WriteByte", &Memory::WriteByte);
	mMemory.def("ReadWord", &Memory::ReadWord);
	mMemory.def("WriteWord", &Memory::WriteWord);
	mMemory.def("ReadDword", &Memory::ReadDword);
	mMemory.def("WriteDword", &Memory::WriteDword);
	mMemory.def("ReadQword", &Memory::ReadQword);
	mMemory.def("WriteQword", &Memory::WriteQword);
	mMemory.def("ReadPtr", &Memory::ReadPtr);
	mMemory.def("WritePtr", &Memory::WritePtr);


}
