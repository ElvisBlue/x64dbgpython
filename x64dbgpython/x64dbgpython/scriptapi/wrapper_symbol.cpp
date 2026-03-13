#include "wrapper.h"

namespace PyWrapper
{
	namespace Symbol
	{
		struct pySymbolInfo
		{
			std::string mod;
			duint rva;
			std::string name;
			bool manual;
			Script::Symbol::SymbolType type;
		};

		std::optional<std::vector<pySymbolInfo>> GetList()
		{
			auto tmpList = std::make_unique<ListInfo>();
			if (Script::Symbol::GetList(tmpList.get()))
			{
				std::vector<pySymbolInfo> list;
				std::vector<Script::Symbol::SymbolInfo> symbolInfoList;
				BridgeList<Script::Symbol::SymbolInfo>::ToVector(tmpList.get(), symbolInfoList, true);
				list.reserve(symbolInfoList.size());
				for (auto it = symbolInfoList.begin(); it != symbolInfoList.end(); it++)
				{
					pySymbolInfo item;
					item.manual = it->manual;
					item.mod = it->mod;
					item.name = it->name;
					item.rva = it->rva;
					item.type = it->type;
					list.push_back(item);
				}
				return list;
			}

			return std::nullopt;
		}
	}

}

void init_wrapper_symbol(py::module_& m)
{
	//Implement module Symbol
	py::module mSymbol = m.def_submodule("Symbol", "x64dbg Symbol python script wrapper");

	py::enum_<Symbol::SymbolType>(mSymbol, "SymbolType")
		.value("Function", Symbol::SymbolType::Function)
		.value("Import", Symbol::SymbolType::Import)
		.value("Export", Symbol::SymbolType::Export);

	py::class_<PyWrapper::Symbol::pySymbolInfo>(mSymbol, "SymbolInfo")
		.def(py::init<>())
		.def_readwrite("mod", &PyWrapper::Symbol::pySymbolInfo::mod)
		.def_readwrite("rva", &PyWrapper::Symbol::pySymbolInfo::rva)
		.def_readwrite("name", &PyWrapper::Symbol::pySymbolInfo::name)
		.def_readwrite("manual", &PyWrapper::Symbol::pySymbolInfo::manual)
		.def_readwrite("type", &PyWrapper::Symbol::pySymbolInfo::type);

	mSymbol.def("GetList", &PyWrapper::Symbol::GetList);

}
