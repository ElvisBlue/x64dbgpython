#include "wrapper.h"

namespace PyWrapper
{
	namespace Function
	{
		struct pyFunctionInfo
		{
			std::string mod;
			duint rvaStart;
			duint rvaEnd;
			bool manual;
			duint instructioncount;
		};

		std::optional<pyFunctionInfo> GetInfo(duint addr)
		{
			Script::Function::FunctionInfo tmpInfo = { 0 };
			if (Script::Function::GetInfo(addr, &tmpInfo))
			{
				pyFunctionInfo info;
				info.mod = tmpInfo.mod;
				info.rvaStart = tmpInfo.rvaStart;
				info.rvaEnd = tmpInfo.rvaEnd;
				info.manual = tmpInfo.manual;
				info.instructioncount = tmpInfo.instructioncount;
				return info;
			}
			else
				return std::nullopt;
		}

		std::optional<std::vector<pyFunctionInfo>> GetList()
		{
			auto tmpList = std::make_unique<ListInfo>();
			if (Script::Function::GetList(tmpList.get()))
			{
				std::vector<pyFunctionInfo> list;
				std::vector<Script::Function::FunctionInfo> functionInfoList;
				BridgeList<Script::Function::FunctionInfo>::ToVector(tmpList.get(), functionInfoList, true);
				list.reserve(functionInfoList.size());
				for (auto it = functionInfoList.begin(); it != functionInfoList.end(); it++)
				{
					pyFunctionInfo item;
					item.instructioncount = it->instructioncount;
					item.manual = it->manual;
					item.mod = it->mod;
					item.rvaEnd = it->rvaEnd;
					item.rvaStart = it->rvaStart;
					list.push_back(item);
				}
				return list;
			}
			return std::nullopt;
		}
	}

}

void init_wrapper_function(py::module_& m)
{
	//Implement module Function
	py::module mFunction = m.def_submodule("Function", "x64dbg Function python script wrapper");

	py::class_<PyWrapper::Function::pyFunctionInfo>(mFunction, "FunctionInfo")
		.def(py::init<>())
		.def_readwrite("mod", &PyWrapper::Function::pyFunctionInfo::mod)
		.def_readwrite("rvaStart", &PyWrapper::Function::pyFunctionInfo::rvaStart)
		.def_readwrite("rvaEnd", &PyWrapper::Function::pyFunctionInfo::rvaEnd)
		.def_readwrite("manual", &PyWrapper::Function::pyFunctionInfo::manual)
		.def_readwrite("instructioncount", &PyWrapper::Function::pyFunctionInfo::instructioncount);
	
	//We ignore implement SCRIPT_EXPORT bool Add(const FunctionInfo* info);
	mFunction.def("Add", static_cast<bool (*)(duint, duint, bool, duint)>(&Function::Add));
	mFunction.def("Get", &Function::Get);
	mFunction.def("GetInfo", &PyWrapper::Function::GetInfo);
	mFunction.def("Overlaps", &Function::Overlaps);
	mFunction.def("Delete", &Function::Delete);
	//We ignore implement SCRIPT_EXPORT void DeleteRange(duint start, duint end);
	mFunction.def("DeleteRange", static_cast<void (*)(duint, duint, bool)>(&Function::DeleteRange));
	mFunction.def("Clear", &Function::Clear);
	mFunction.def("GetList", &PyWrapper::Function::GetList);


}
