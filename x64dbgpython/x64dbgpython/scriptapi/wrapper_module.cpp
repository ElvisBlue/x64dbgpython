#include "wrapper.h"

namespace PyWrapper
{
	namespace Module
	{
		struct pyModuleInfo
		{
			duint base;
			duint size;
			duint entry;
			int sectionCount;
			std::string name;
			std::string path;
		};

		struct pyModuleSectionInfo
		{
			duint addr;
			duint size;
			std::string name;
		};

		struct pyModuleExport
		{
			duint ordinal;
			duint rva;
			duint va;
			bool forwarded;
			std::string forwardName;
			std::string name;
			std::string undecoratedName;
		};

		struct pyModuleImport
		{
			duint iatRva;
			duint iatVa;
			duint ordinal; //equal to -1 if imported by name
			std::string name;
			std::string undecoratedName;
		};

		std::optional<pyModuleInfo> InfoFromAddr(duint addr)
		{
			Script::Module::ModuleInfo tmpInfo = { 0 };
			if (Script::Module::InfoFromAddr(addr, &tmpInfo))
			{
				pyModuleInfo info;
				info.base = tmpInfo.base;
				info.entry = tmpInfo.entry;
				info.name = tmpInfo.name;
				info.path = tmpInfo.path;
				info.sectionCount = tmpInfo.sectionCount;
				info.size = tmpInfo.size;
				return info;
			}
			else
				return std::nullopt;
		}

		std::optional<pyModuleInfo> InfoFromName(const char* name)
		{
			Script::Module::ModuleInfo tmpInfo = { 0 };
			if (Script::Module::InfoFromName(name, &tmpInfo))
			{
				pyModuleInfo info;
				info.base = tmpInfo.base;
				info.entry = tmpInfo.entry;
				info.name = tmpInfo.name;
				info.path = tmpInfo.path;
				info.sectionCount = tmpInfo.sectionCount;
				info.size = tmpInfo.size;
				return info;
			}
			else
				return std::nullopt;
		}

		std::string NameFromAddr(duint addr)
		{
			char retName[MAX_MODULE_SIZE] = { 0 };
			if (Script::Module::NameFromAddr(addr, retName))
				return retName;
			else
				return "";
		}

		std::string PathFromAddr(duint addr)
		{
			char retPath[MAX_PATH] = {0};
			if (Script::Module::PathFromAddr(addr, retPath))
				return retPath;
			else
				return "";
		}

		std::string PathFromName(const char* name)
		{
			char retPath[MAX_PATH] = { 0 };
			if (Script::Module::PathFromName(name, retPath))
				return retPath;
			else
				return "";
		}

		std::optional<pyModuleSectionInfo> SectionFromAddr(duint addr, int number)
		{
			Script::Module::ModuleSectionInfo tmpInfo = { 0 };
			if (Script::Module::SectionFromAddr(addr, number, &tmpInfo))
			{
				pyModuleSectionInfo section;
				section.addr = tmpInfo.addr;
				section.name = tmpInfo.name;
				section.size = tmpInfo.size;
				return section;
			}
			else
				return std::nullopt;
		}

		std::optional<pyModuleSectionInfo> SectionFromName(const char* name, int number)
		{
			Script::Module::ModuleSectionInfo tmpInfo = { 0 };
			if (Script::Module::SectionFromName(name, number, &tmpInfo))
			{
				pyModuleSectionInfo section;
				section.addr = tmpInfo.addr;
				section.name = tmpInfo.name;
				section.size = tmpInfo.size;
				return section;
			}
			return std::nullopt;
		}

		std::optional<std::vector<pyModuleSectionInfo>> SectionListFromAddr(duint addr)
		{
			auto tmpList = std::make_unique<ListInfo>();
			if (Script::Module::SectionListFromAddr(addr, tmpList.get()))
			{
				std::vector<pyModuleSectionInfo> list;
				std::vector<Script::Module::ModuleSectionInfo> moduleSectionInfoList;
				BridgeList<Script::Module::ModuleSectionInfo>::ToVector(tmpList.get(), moduleSectionInfoList, true);
				list.reserve(moduleSectionInfoList.size());
				for (auto it = moduleSectionInfoList.begin(); it != moduleSectionInfoList.end(); it++)
				{
					pyModuleSectionInfo item = { 0 };
					item.addr = it->addr;
					item.name = it->name;
					item.size = it->size;
					list.push_back(item);
				}
				return list;
			}
			return std::nullopt;
		}

		std::optional<std::vector<pyModuleSectionInfo>> SectionListFromName(const char* name)
		{
			auto tmpList = std::make_unique<ListInfo>();
			if (Script::Module::SectionListFromName(name, tmpList.get()))
			{
				std::vector<pyModuleSectionInfo> list;
				std::vector<Script::Module::ModuleSectionInfo> moduleSectionInfoList;
				BridgeList<Script::Module::ModuleSectionInfo>::ToVector(tmpList.get(), moduleSectionInfoList, true);
				list.reserve(moduleSectionInfoList.size());
				for (auto it = moduleSectionInfoList.begin(); it != moduleSectionInfoList.end(); it++)
				{
					pyModuleSectionInfo item = { 0 };
					item.addr = it->addr;
					item.name = it->name;
					item.size = it->size;
					list.push_back(item);
				}
				return list;
			}
			return std::nullopt;
		}

		std::optional<pyModuleInfo> GetMainModuleInfo()
		{
			Script::Module::ModuleInfo tmpInfo = { 0 };
			if (Script::Module::GetMainModuleInfo(&tmpInfo))
			{
				pyModuleInfo info;
				info.base = tmpInfo.base;
				info.entry = tmpInfo.entry;
				info.name = tmpInfo.name;
				info.path = tmpInfo.path;
				info.sectionCount = tmpInfo.sectionCount;
				info.size = tmpInfo.size;
				return info;
			}
			else
				return std::nullopt;
		}

		std::string GetMainModuleName()
		{
			char retName[MAX_MODULE_SIZE] = { 0 };
			Script::Module::GetMainModuleName(retName);
			return retName;
		}

		std::string GetMainModulePath()
		{
			char retPath[MAX_PATH] = { 0 };
			Script::Module::GetMainModulePath(retPath);
			return retPath;
		}

		std::optional<std::vector<pyModuleSectionInfo>> GetMainModuleSectionList()
		{
			auto tmpList = std::make_unique<ListInfo>();
			if (Script::Module::GetMainModuleSectionList(tmpList.get()))
			{
				std::vector<pyModuleSectionInfo> list;
				std::vector<Script::Module::ModuleSectionInfo> sectionList;
				BridgeList<Script::Module::ModuleSectionInfo>::ToVector(tmpList.get(), sectionList, true);
				list.reserve(sectionList.size());
				for (auto it = sectionList.begin(); it != sectionList.end(); it++)
				{
					pyModuleSectionInfo item;
					item.addr = it->addr;
					item.name = it->name;
					item.size = it->size;
					list.push_back(item);
				}
				return list;
			}
			return std::nullopt;
		}

		std::optional<std::vector<pyModuleInfo>> GetList()
		{
			auto tmpList = std::make_unique<ListInfo>();
			if (Script::Module::GetList(tmpList.get()))
			{
				std::vector<pyModuleInfo> list;
				std::vector<Script::Module::ModuleInfo> moduleInfoList;
				BridgeList<Script::Module::ModuleInfo>::ToVector(tmpList.get(), moduleInfoList, true);
				list.reserve(moduleInfoList.size());
				for (auto it = moduleInfoList.begin(); it != moduleInfoList.end(); it++)
				{
					pyModuleInfo item;
					item.base = it->base;
					item.entry = it->entry;
					item.name = it->name;
					item.path = it->path;
					item.sectionCount = it->sectionCount;
					item.size = it->size;
					list.push_back(item);
				}
				return list;
			}
			return std::nullopt;
		}

		std::optional<std::vector<pyModuleExport>> GetExports(const pyModuleInfo* mod)
		{
			auto tmpList = std::make_unique<ListInfo>();

			Script::Module::ModuleInfo tmpMod = { 0 };
			tmpMod.base = mod->base;
			tmpMod.entry = mod->entry;
			strcpy_s(tmpMod.name, mod->name.c_str());
			strcpy_s(tmpMod.path, mod->path.c_str());
			tmpMod.sectionCount = mod->sectionCount;
			tmpMod.size = mod->size;
			if (Script::Module::GetExports(&tmpMod, tmpList.get()))
			{
				std::vector<pyModuleExport> list;
				std::vector<Script::Module::ModuleExport> moduleExportList;
				BridgeList<Script::Module::ModuleExport>::ToVector(tmpList.get(), moduleExportList, true);
				list.reserve(moduleExportList.size());
				for (auto it = moduleExportList.begin(); it != moduleExportList.end(); it++)
				{
					pyModuleExport item;
					item.forwarded = it->forwarded;
					item.forwardName = it->forwardName;
					item.name = it->name;
					item.ordinal = it->ordinal;
					item.rva = it->rva;
					item.undecoratedName = it->undecoratedName;
					item.va = it->va;
					list.push_back(item);
				}
				return list;
			}

			return std::nullopt;
		}

		std::optional<std::vector<pyModuleImport>> GetImports(const pyModuleInfo* mod)
		{
			auto tmpList = std::make_unique<ListInfo>();

			Script::Module::ModuleInfo tmpMod = { 0 };
			tmpMod.base = mod->base;
			tmpMod.entry = mod->entry;
			strcpy_s(tmpMod.name, mod->name.c_str());
			strcpy_s(tmpMod.path, mod->path.c_str());
			tmpMod.sectionCount = mod->sectionCount;
			tmpMod.size = mod->size;

			if (Script::Module::GetImports(&tmpMod, tmpList.get()))
			{
				std::vector<pyModuleImport> list;
				std::vector<Script::Module::ModuleImport> moduleImportList;
				BridgeList<Script::Module::ModuleImport>::ToVector(tmpList.get(), moduleImportList, true);
				list.reserve(moduleImportList.size());
				for (auto it = moduleImportList.begin(); it != moduleImportList.end(); it++)
				{
					pyModuleImport item;
					item.iatRva = it->iatRva;
					item.iatVa = it->iatVa;
					item.name = it->name;
					item.ordinal = it->ordinal;
					item.undecoratedName = it->undecoratedName;
					list.push_back(item);
				}
				return list;
			}
			return std::nullopt;
		}
	}

}

void init_wrapper_module(py::module_& m)
{
	//Implement module Module
	py::module mModule = m.def_submodule("Module", "x64dbg Module python script wrapper");

	py::class_<PyWrapper::Module::pyModuleInfo>(mModule, "ModuleInfo")
		.def(py::init<>())
		.def_readwrite("base", &PyWrapper::Module::pyModuleInfo::base)
		.def_readwrite("size", &PyWrapper::Module::pyModuleInfo::size)
		.def_readwrite("entry", &PyWrapper::Module::pyModuleInfo::entry)
		.def_readwrite("sectionCount", &PyWrapper::Module::pyModuleInfo::sectionCount)
		.def_readwrite("name", &PyWrapper::Module::pyModuleInfo::name)
		.def_readwrite("path", &PyWrapper::Module::pyModuleInfo::path);

	py::class_<PyWrapper::Module::pyModuleSectionInfo>(mModule, "ModuleSectionInfo")
		.def(py::init<>())
		.def_readwrite("addr", &PyWrapper::Module::pyModuleSectionInfo::addr)
		.def_readwrite("size", &PyWrapper::Module::pyModuleSectionInfo::size)
		.def_readwrite("name", &PyWrapper::Module::pyModuleSectionInfo::name);

	py::class_<PyWrapper::Module::pyModuleExport>(mModule, "ModuleExport")
		.def(py::init<>())
		.def_readwrite("ordinal", &PyWrapper::Module::pyModuleExport::ordinal)
		.def_readwrite("rva", &PyWrapper::Module::pyModuleExport::rva)
		.def_readwrite("va", &PyWrapper::Module::pyModuleExport::va)
		.def_readwrite("forwarded", &PyWrapper::Module::pyModuleExport::forwarded)
		.def_readwrite("forwardName", &PyWrapper::Module::pyModuleExport::forwardName)
		.def_readwrite("name", &PyWrapper::Module::pyModuleExport::name)
		.def_readwrite("undecoratedName", &PyWrapper::Module::pyModuleExport::undecoratedName);

	py::class_<PyWrapper::Module::pyModuleImport>(mModule, "ModuleImport")
		.def(py::init<>())
		.def_readwrite("iatRva", &PyWrapper::Module::pyModuleImport::iatRva)
		.def_readwrite("iatVa", &PyWrapper::Module::pyModuleImport::iatVa)
		.def_readwrite("ordinal", &PyWrapper::Module::pyModuleImport::ordinal)
		.def_readwrite("name", &PyWrapper::Module::pyModuleImport::name)
		.def_readwrite("undecoratedName", &PyWrapper::Module::pyModuleImport::undecoratedName);

	mModule.def("InfoFromAddr", &PyWrapper::Module::InfoFromAddr);
	mModule.def("InfoFromName", &PyWrapper::Module::InfoFromName);
	mModule.def("BaseFromAddr", &Module::BaseFromAddr);
	mModule.def("BaseFromName", &Module::BaseFromName);
	mModule.def("SizeFromAddr", &Module::SizeFromAddr);
	mModule.def("SizeFromName", &Module::SizeFromName);
	mModule.def("NameFromAddr", &PyWrapper::Module::NameFromAddr);
	mModule.def("PathFromAddr", &PyWrapper::Module::PathFromAddr);
	mModule.def("EntryFromAddr", &Module::EntryFromAddr);
	mModule.def("EntryFromName", &Module::EntryFromName);
	mModule.def("SectionCountFromAddr", &Module::SectionCountFromAddr);
	mModule.def("SectionCountFromName", &Module::SectionCountFromName);
	mModule.def("SectionFromAddr", &PyWrapper::Module::SectionFromAddr);
	mModule.def("SectionFromName", &PyWrapper::Module::SectionFromName);
	mModule.def("SectionListFromAddr", &PyWrapper::Module::SectionListFromAddr);
	mModule.def("SectionListFromName", &PyWrapper::Module::SectionListFromName);
	mModule.def("GetMainModuleInfo", &PyWrapper::Module::GetMainModuleInfo);
	mModule.def("GetMainModuleBase", &Module::GetMainModuleBase);
	mModule.def("GetMainModuleSize", &Module::GetMainModuleSize);
	mModule.def("GetMainModuleEntry", &Module::GetMainModuleEntry);
	mModule.def("GetMainModuleSectionCount", &Module::GetMainModuleSectionCount);
	mModule.def("GetMainModuleName", &PyWrapper::Module::GetMainModuleName);
	mModule.def("GetMainModuleSectionList", &PyWrapper::Module::GetMainModuleSectionList);
	mModule.def("GetList", &PyWrapper::Module::GetList);
	mModule.def("GetExports", &PyWrapper::Module::GetExports);
	mModule.def("GetImports", &PyWrapper::Module::GetImports);


}
