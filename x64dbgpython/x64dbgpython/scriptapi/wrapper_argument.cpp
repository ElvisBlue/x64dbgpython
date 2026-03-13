#include "wrapper.h"

namespace PyWrapper
{
	namespace Argument
	{
		struct pyArgumentInfo
		{
			std::string mod;
			duint rvaStart;
			duint rvaEnd;
			bool manual;
			duint instructioncount;
		};

		std::optional<pyArgumentInfo> GetInfo(duint addr)
		{
			Script::Argument::ArgumentInfo tmpInfo = { 0 };
			if (!Script::Argument::GetInfo(addr, &tmpInfo))
			{
				return std::nullopt;
			}
			else
			{
				pyArgumentInfo retData;
				retData.mod = tmpInfo.mod;
				retData.rvaStart = tmpInfo.rvaStart;
				retData.manual = tmpInfo.manual;
				retData.instructioncount = tmpInfo.instructioncount;
				return retData;
			}
		}

		std::optional<std::vector<pyArgumentInfo>> GetList()
		{
			auto tmpList = std::make_unique<ListInfo>();

			if (Script::Argument::GetList(tmpList.get()))
			{
				std::vector<pyArgumentInfo> list;
				std::vector<Script::Argument::ArgumentInfo> argumentInfoList;
				BridgeList<Script::Argument::ArgumentInfo>::ToVector(tmpList.get(), argumentInfoList, true);
				list.reserve(argumentInfoList.size());
				for (auto it = argumentInfoList.begin(); it != argumentInfoList.end(); it++)
				{
					pyArgumentInfo item;
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

void InitWrapperArgument(py::module_& m)
{
	//Implement python module Argument
	py::module mArgument = m.def_submodule("Argument", "x64dbg Argument python script wrapper");

	py::class_<PyWrapper::Argument::pyArgumentInfo>(mArgument, "ArgumentInfo")
		.def(py::init<>())
		.def_readwrite("mod", &PyWrapper::Argument::pyArgumentInfo::mod)
		.def_readwrite("rvaStart", &PyWrapper::Argument::pyArgumentInfo::rvaStart)
		.def_readwrite("rvaEnd", &PyWrapper::Argument::pyArgumentInfo::rvaEnd)
		.def_readwrite("manual", &PyWrapper::Argument::pyArgumentInfo::manual)
		.def_readwrite("instructioncount", &PyWrapper::Argument::pyArgumentInfo::instructioncount);

	//Because python does not support function overload,
	//so we ignore function SCRIPT_EXPORT bool Add(const ArgumentInfo* info)
	mArgument.def("Add", static_cast<bool (*)(duint, duint, bool, duint)>(&Argument::Add));
	mArgument.def("Get", &Argument::Get);
	mArgument.def("GetInfo", &PyWrapper::Argument::GetInfo);
	mArgument.def("Overlaps", &Argument::Overlaps);
	mArgument.def("Delete", &Argument::Delete);
	mArgument.def("DeleteRange", &Argument::DeleteRange);
	mArgument.def("Clear", &Argument::Clear);
	mArgument.def("GetList", &PyWrapper::Argument::GetList);


}
