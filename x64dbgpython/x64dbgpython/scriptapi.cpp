#include <Windows.h>
#include "pybind11/embed.h"
#include "pybind11/stl_bind.h"
#include "pybind11/stl.h"

#include "pluginsdk/_plugins.h"

#include "pluginsdk/_scriptapi_argument.h"
#include "pluginsdk/_scriptapi_assembler.h"
#include "pluginsdk/_scriptapi_bookmark.h"
#include "pluginsdk/_scriptapi_comment.h"
#include "pluginsdk/_scriptapi_debug.h"
#include "pluginsdk/_scriptapi_flag.h"
#include "pluginsdk/_scriptapi_function.h"
#include "pluginsdk/_scriptapi_gui.h"
#include "pluginsdk/_scriptapi_label.h"
#include "pluginsdk/_scriptapi_memory.h"
#include "pluginsdk/_scriptapi_misc.h"
#include "pluginsdk/_scriptapi_module.h"
#include "pluginsdk/_scriptapi_pattern.h"
#include "pluginsdk/_scriptapi_register.h"
#include "pluginsdk/_scriptapi_stack.h"
#include "pluginsdk/_scriptapi_symbol.h"

using namespace Script;
namespace py = pybind11;

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

		pyArgumentInfo* GetInfo(duint addr)
		{
			Script::Argument::ArgumentInfo tmpInfo = { 0 };
			if (!Script::Argument::GetInfo(addr, &tmpInfo))
			{
				return nullptr;
			}
			else
			{
				pyArgumentInfo* retData = new pyArgumentInfo();
				retData->mod = tmpInfo.mod;
				retData->rvaStart = tmpInfo.rvaStart;
				retData->manual = tmpInfo.manual;
				retData->instructioncount = tmpInfo.instructioncount;
				return retData;
			}
		}

		std::vector<pyArgumentInfo>* GetList()
		{
			std::vector<pyArgumentInfo>* list = nullptr;
			ListOf(ArgumentInfo) tmpList = new ListInfo();

			if (Script::Argument::GetList(tmpList))
			{
				list = new std::vector<pyArgumentInfo>();
				std::vector<Script::Argument::ArgumentInfo> argumentInfoList;
				BridgeList<Script::Argument::ArgumentInfo>::ToVector(tmpList, argumentInfoList, true);
				for (auto it = argumentInfoList.begin(); it != argumentInfoList.end(); it++)
				{
					pyArgumentInfo item;
					item.instructioncount = it->instructioncount;
					item.manual = it->manual;
					item.mod = it->mod;
					item.rvaEnd = it->rvaEnd;
					item.rvaStart = it->rvaStart;
					list->push_back(item);
				}
			}

			delete tmpList;
			return list;
		}
	}

	namespace Assembler
	{
		py::bytes Assemble(duint addr, const char* instruction)
		{
			unsigned char dest[16] = { 0 };
			int size = 0;
			if (Script::Assembler::Assemble(addr, dest, &size, instruction))
				return py::bytes((char*)dest, size);
			else
				return nullptr;
		}

		py::bytes AssembleEx(duint addr, const char* instruction)
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
				return nullptr;
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
				_plugin_logprintf("AssembleEx error: %s\n", retError);
				return false;
			}
		}
	}

	namespace Bookmark
	{
		struct pyBookmarkInfo
		{
			std::string mod;
			duint rva;
			bool manual;
		};


		pyBookmarkInfo* GetInfo(duint addr)
		{
			Script::Bookmark::BookmarkInfo tmpInfo = { 0 };
			if (Script::Bookmark::GetInfo(addr, &tmpInfo))
			{
				pyBookmarkInfo* info = new pyBookmarkInfo();
				info->manual = tmpInfo.manual;
				info->mod = tmpInfo.mod;
				info->rva = tmpInfo.rva;
				return info;
			}
			else
				return nullptr;
		}

		std::vector<pyBookmarkInfo>* GetList()
		{
			std::vector<pyBookmarkInfo>* list = nullptr;
			ListOf(BookmarkInfo) tmpList = new ListInfo();
			if (Script::Bookmark::GetList(tmpList))
			{
				list = new std::vector<pyBookmarkInfo>();
				std::vector<Script::Bookmark::BookmarkInfo> bookmarkInfoList;
				BridgeList<Script::Bookmark::BookmarkInfo>::ToVector(tmpList, bookmarkInfoList, true);
				for (auto it = bookmarkInfoList.begin(); it != bookmarkInfoList.end(); it++)
				{
					pyBookmarkInfo item;
					item.manual = it->manual;
					item.mod = it->mod;
					item.rva = it->rva;
					list->push_back(item);
				}
			}

			delete tmpList;
			return list;
		}
	}

	namespace Comment
	{
		struct pyCommentInfo
		{
			std::string mod;
			duint rva;
			std::string text;
			bool manual;
		};

		std::string Get(duint addr)
		{
			char retText[MAX_COMMENT_SIZE] = { 0 };
			Script::Comment::Get(addr, retText);
			return retText;
		}

		pyCommentInfo* GetInfo(duint addr)
		{
			Script::Comment::CommentInfo tmpInfo = { 0 };
			if (Script::Comment::GetInfo(addr, &tmpInfo))
			{
				pyCommentInfo* info = new pyCommentInfo();
				info->manual = tmpInfo.manual;
				info->mod = tmpInfo.mod;
				info->rva = tmpInfo.rva;
				info->text = tmpInfo.text;
				return info;
			}
			else
				return nullptr;
		}

		std::vector<pyCommentInfo>* GetList()
		{
			std::vector<pyCommentInfo>* list = nullptr;
			ListOf(Script::Comment::CommentInfo) tmpList = new ListInfo();
			if (Script::Comment::GetList(tmpList))
			{
				list = new std::vector<pyCommentInfo>();
				std::vector<Script::Comment::CommentInfo> commentInfoList;
				BridgeList<Script::Comment::CommentInfo>::ToVector(tmpList, commentInfoList, true);
				for (auto it = commentInfoList.begin(); it != commentInfoList.end(); it++)
				{
					pyCommentInfo item;
					item.manual = it->manual;
					item.mod = it->mod;
					item.rva = it->rva;
					item.text = it->text;
					list->push_back(item);
				}
			}
			delete tmpList;
			return list;
		}
	}

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

		pyFunctionInfo* GetInfo(duint addr)
		{
			Script::Function::FunctionInfo tmpInfo = { 0 };
			if (Script::Function::GetInfo(addr, &tmpInfo))
			{
				pyFunctionInfo* info = new pyFunctionInfo();
				info->mod = tmpInfo.mod;
				info->rvaStart = tmpInfo.rvaStart;
				info->rvaEnd = tmpInfo.rvaEnd;
				info->manual = tmpInfo.manual;
				info->instructioncount = tmpInfo.instructioncount;
				return info;
			}
			else
				return nullptr;
		}

		std::vector<pyFunctionInfo>* GetList()
		{
			std::vector<pyFunctionInfo>* list = nullptr;
			ListOf(Script::Function::FunctionInfo) tmpList = new ListInfo();
			if (Script::Function::GetList(tmpList))
			{
				list = new std::vector<pyFunctionInfo>();
				std::vector<Script::Function::FunctionInfo> functionInfoList;
				BridgeList<Script::Function::FunctionInfo>::ToVector(tmpList, functionInfoList, true);
				for (auto it = functionInfoList.begin(); it != functionInfoList.end(); it++)
				{
					pyFunctionInfo item;
					item.instructioncount = it->instructioncount;
					item.manual = it->manual;
					item.mod = it->mod;
					item.rvaEnd = it->rvaEnd;
					item.rvaStart = it->rvaStart;
					list->push_back(item);
				}
			}
			delete tmpList;
			return list;
		}
	}

	namespace Gui
	{
		
		namespace Disassembly
		{
			std::vector<duint>* SelectionGet()
			{
				std::vector<duint>* ret = nullptr;
				duint start = 0;
				duint end = 0;
				if (Script::Gui::Disassembly::SelectionGet(&start, &end))
				{
					ret = new std::vector<duint>();
					ret->push_back(start);
					ret->push_back(end);
				}
				return ret;
			}
		}

		namespace Dump
		{
			std::vector<duint>* SelectionGet()
			{
				std::vector<duint>* ret = nullptr;
				duint start = 0;
				duint end = 0;
				if (Script::Gui::Dump::SelectionGet(&start, &end))
				{
					ret = new std::vector<duint>();
					ret->push_back(start);
					ret->push_back(end);
				}
				return ret;
			}
		}

		namespace Stack
		{
			std::vector<duint>* SelectionGet()
			{
				std::vector<duint>* ret = nullptr;
				duint start = 0;
				duint end = 0;
				if (Script::Gui::Stack::SelectionGet(&start, &end))
				{
					ret = new std::vector<duint>();
					ret->push_back(start);
					ret->push_back(end);
				}
				return ret;
			}
		}

		std::vector<duint>* SelectionGet(Script::Gui::Window window)
		{
			std::vector<duint>* ret = nullptr;
			duint start = 0;
			duint end = 0;
			if (Script::Gui::SelectionGet(window, &start, &end))
			{
				ret = new std::vector<duint>();
				ret->push_back(start);
				ret->push_back(end);
			}
			return ret;
		}
		

		std::string InputLine(const char* title)
		{
			char* text = new char[GUI_MAX_LINE_SIZE];
			if (!text)
				return "";

			ZeroMemory(text, GUI_MAX_LINE_SIZE);

			std::string s;
			Script::Gui::InputLine(title, text);
			s = text;

			delete[] text;
			return s;
		}

		duint InputValue(const char* title)
		{
			duint value = 0;
			if (Script::Gui::InputValue(title, &value))
				return value;
			else
				return 0;
		}
	}

	namespace Label
	{
		struct pyLabelInfo
		{
			std::string mod;
			duint rva;
			std::string text;
			bool manual;
		};

		std::string Get(duint addr)
		{
			char retText[MAX_LABEL_SIZE] = { 0 };
			Script::Label::Get(addr, retText);
			return retText;
		}

		pyLabelInfo* GetInfo(duint addr)
		{
			Script::Label::LabelInfo tmpInfo = { 0 };
			if (Script::Label::GetInfo(addr, &tmpInfo))
			{
				pyLabelInfo* info = new pyLabelInfo();
				info->manual = tmpInfo.manual;
				info->mod = tmpInfo.mod;
				info->rva = tmpInfo.rva;
				info->text = tmpInfo.text;
				return info;
			}
			else
				return nullptr;
		}

		std::vector<pyLabelInfo>* GetList()
		{
			std::vector<pyLabelInfo>* list = nullptr;
			ListOf(Script::Label::LabelInfo) tmpList = new ListInfo();
			if (Script::Label::GetList(tmpList))
			{
				list = new std::vector<pyLabelInfo>();
				std::vector<Script::Label::LabelInfo> labelInfoList;
				BridgeList<Script::Label::LabelInfo>::ToVector(tmpList, labelInfoList, true);
				for (auto it = labelInfoList.begin(); it != labelInfoList.end(); it++)
				{
					pyLabelInfo item;
					item.manual = it->manual;
					item.mod = it->mod;
					item.rva = it->rva;
					item.text = it->text;
					list->push_back(item);
				}
			}

			delete tmpList;
			return list;
		}
	}

	namespace Memory
	{
		py::bytes Read(duint addr, duint size)
		{
			unsigned char* readData = (unsigned char*)malloc(size);
			if (readData == nullptr)
				return nullptr;

			memset(readData, 0, size);
			duint sizeRead = 0;
			py::bytes ret;
			if (Script::Memory::Read(addr, readData, size, &sizeRead))
				ret = py::bytes((char*)readData, sizeRead);
			else
				ret = nullptr;
			
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

		pyModuleInfo* InfoFromAddr(duint addr)
		{
			Script::Module::ModuleInfo tmpInfo = { 0 };
			if (Script::Module::InfoFromAddr(addr, &tmpInfo))
			{
				pyModuleInfo* info = new pyModuleInfo();
				info->base = tmpInfo.base;
				info->entry = tmpInfo.entry;
				info->name = tmpInfo.name;
				info->path = tmpInfo.path;
				info->sectionCount = tmpInfo.sectionCount;
				info->size = tmpInfo.size;
				return info;
			}
			else
				return nullptr;
		}

		pyModuleInfo* InfoFromName(const char* name)
		{
			Script::Module::ModuleInfo tmpInfo = { 0 };
			if (Script::Module::InfoFromName(name, &tmpInfo))
			{
				pyModuleInfo* info = new pyModuleInfo();
				info->base = tmpInfo.base;
				info->entry = tmpInfo.entry;
				info->name = tmpInfo.name;
				info->path = tmpInfo.path;
				info->sectionCount = tmpInfo.sectionCount;
				info->size = tmpInfo.size;
				return info;
			}
			else
				return nullptr;
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

		pyModuleSectionInfo* SectionFromAddr(duint addr, int number)
		{
			Script::Module::ModuleSectionInfo tmpInfo = { 0 };
			if (Script::Module::SectionFromAddr(addr, number, &tmpInfo))
			{
				pyModuleSectionInfo* section = new pyModuleSectionInfo();
				section->addr = tmpInfo.addr;
				section->name = tmpInfo.name;
				section->size = tmpInfo.size;
				return section;
			}
			else
				return nullptr;
		}

		pyModuleSectionInfo* SectionFromName(const char* name, int number)
		{
			Script::Module::ModuleSectionInfo tmpInfo = { 0 };
			if (Script::Module::SectionFromName(name, number, &tmpInfo))
			{
				pyModuleSectionInfo* section = new pyModuleSectionInfo();
				section->addr = tmpInfo.addr;
				section->name = tmpInfo.name;
				section->size = tmpInfo.size;
			}
				return nullptr;
		}

		std::vector<pyModuleSectionInfo>* SectionListFromAddr(duint addr)
		{
			std::vector<pyModuleSectionInfo>* list = nullptr;
			ListOf(Script::Module::ModuleSectionInfo) tmpList = new ListInfo();
			if (Script::Module::SectionListFromAddr(addr, tmpList))
			{
				list = new std::vector<pyModuleSectionInfo>();
				std::vector<Script::Module::ModuleSectionInfo> moduleSectionInfoList;
				BridgeList<Script::Module::ModuleSectionInfo>::ToVector(tmpList, moduleSectionInfoList, true);
				for (auto it = moduleSectionInfoList.begin(); it != moduleSectionInfoList.end(); it++)
				{
					pyModuleSectionInfo item = { 0 };
					item.addr = it->addr;
					item.name = it->name;
					item.size = it->size;
					list->push_back(item);
				}
			}
			delete tmpList;
			return list;
		}

		std::vector<pyModuleSectionInfo>* SectionListFromName(const char* name)
		{
			std::vector<pyModuleSectionInfo>* list = nullptr;
			ListOf(Script::Module::ModuleSectionInfo) tmpList = new ListInfo();
			if (Script::Module::SectionListFromName(name, tmpList))
			{
				list = new std::vector<pyModuleSectionInfo>();
				std::vector<Script::Module::ModuleSectionInfo> moduleSectionInfoList;
				BridgeList<Script::Module::ModuleSectionInfo>::ToVector(tmpList, moduleSectionInfoList, true);
				for (auto it = moduleSectionInfoList.begin(); it != moduleSectionInfoList.end(); it++)
				{
					pyModuleSectionInfo item = { 0 };
					item.addr = it->addr;
					item.name = it->name;
					item.size = it->size;
					list->push_back(item);
				}
			}
			delete tmpList;
			return list;
		}

		pyModuleInfo* GetMainModuleInfo()
		{
			Script::Module::ModuleInfo tmpInfo = { 0 };
			if (Script::Module::GetMainModuleInfo(&tmpInfo))
			{
				pyModuleInfo* info = new pyModuleInfo();
				info->base = tmpInfo.base;
				info->entry = tmpInfo.entry;
				info->name = tmpInfo.name;
				info->path = tmpInfo.path;
				info->sectionCount = tmpInfo.sectionCount;
				info->size = tmpInfo.size;
				return info;
			}
			else
				return nullptr;
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

		std::vector<pyModuleSectionInfo>* GetMainModuleSectionList()
		{
			std::vector<pyModuleSectionInfo>* list = nullptr;
			ListOf(Script::Module::ModuleSectionInfo) tmpList = new ListInfo();
			if (Script::Module::GetMainModuleSectionList(tmpList))
			{
				list = new std::vector<pyModuleSectionInfo>();
				std::vector<Script::Module::ModuleSectionInfo> sectionList;
				BridgeList<Script::Module::ModuleSectionInfo>::ToVector(tmpList, sectionList, true);
				for (auto it = sectionList.begin(); it != sectionList.end(); it++)
				{
					pyModuleSectionInfo item;
					item.addr = it->addr;
					item.name = it->name;
					item.size = it->size;
					list->push_back(item);
				}
			}
			delete tmpList;
			return list;
		}

		std::vector<pyModuleInfo>* GetList()
		{
			std::vector<pyModuleInfo>* list = nullptr;
			ListOf(Script::Module::ModuleInfo) tmpList = new ListInfo();
			if (Script::Module::GetList(tmpList))
			{
				list = new std::vector<pyModuleInfo>();
				std::vector<Script::Module::ModuleInfo> moduleInfoList;
				BridgeList<Script::Module::ModuleInfo>::ToVector(tmpList, moduleInfoList, true);
				for (auto it = moduleInfoList.begin(); it != moduleInfoList.end(); it++)
				{
					pyModuleInfo item;
					item.base = it->base;
					item.entry = it->entry;
					item.name = it->name;
					item.path = it->path;
					item.sectionCount = it->sectionCount;
					item.size = it->size;
					list->push_back(item);
				}
			}
			delete tmpList;
			return list;
		}

		std::vector<pyModuleExport>* GetExports(const pyModuleInfo* mod)
		{
			std::vector<pyModuleExport>* list = nullptr;
			ListOf(Script::Module::ModuleExport) tmpList = new ListInfo();

			Script::Module::ModuleInfo tmpMod = { 0 };
			tmpMod.base = mod->base;
			tmpMod.entry = mod->entry;
			strcpy_s(tmpMod.name, mod->name.c_str());
			strcpy_s(tmpMod.path, mod->path.c_str());
			tmpMod.sectionCount = mod->sectionCount;
			tmpMod.size = mod->size;
			if (Script::Module::GetExports(&tmpMod, tmpList))
			{
				list = new std::vector<pyModuleExport>();
				std::vector<Script::Module::ModuleExport> moduleExportList;
				BridgeList<Script::Module::ModuleExport>::ToVector(tmpList, moduleExportList, true);
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
					list->push_back(item);
				}
			}

			delete tmpList;
			return list;
		}

		std::vector<pyModuleImport>* GetImports(const pyModuleInfo* mod)
		{
			std::vector<pyModuleImport>* list = nullptr;
			ListOf(Script::Module::ModuleImport) tmpList = new ListInfo();

			Script::Module::ModuleInfo tmpMod = { 0 };
			tmpMod.base = mod->base;
			tmpMod.entry = mod->entry;
			strcpy_s(tmpMod.name, mod->name.c_str());
			strcpy_s(tmpMod.path, mod->path.c_str());
			tmpMod.sectionCount = mod->sectionCount;
			tmpMod.size = mod->size;

			if (Script::Module::GetImports(&tmpMod, tmpList))
			{
				list = new std::vector<pyModuleImport>();
				std::vector<Script::Module::ModuleImport> moduleImportList;
				BridgeList<Script::Module::ModuleImport>::ToVector(tmpList, moduleImportList, true);
				for (auto it = moduleImportList.begin(); it != moduleImportList.end(); it++)
				{
					pyModuleImport item;
					item.iatRva = it->iatRva;
					item.iatVa = it->iatVa;
					item.name = it->name;
					item.ordinal = it->ordinal;
					item.undecoratedName = it->undecoratedName;
					list->push_back(item);
				}
			}
			delete tmpList;
			return list;
		}
	}

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

		std::vector<pySymbolInfo>* GetList()
		{
			std::vector<pySymbolInfo>* list = nullptr;
			ListOf(Script::Symbol::SymbolInfo) tmpList = new ListInfo();
			if (Script::Symbol::GetList(tmpList))
			{
				list = new std::vector<pySymbolInfo>();
				std::vector<Script::Symbol::SymbolInfo> symbolInfoList;
				BridgeList<Script::Symbol::SymbolInfo>::ToVector(tmpList, symbolInfoList, true);
				for (auto it = symbolInfoList.begin(); it != symbolInfoList.end(); it++)
				{
					pySymbolInfo item;
					item.manual = it->manual;
					item.mod = it->mod;
					item.name = it->name;
					item.rva = it->rva;
					item.type = it->type;
					list->push_back(item);
				}
			}

			delete tmpList;
			return list;
		}
	}
}

PYBIND11_EMBEDDED_MODULE(x64dbg, m)
{
	m.doc() = "Python module to wrap the x64dbg script api.";

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

	//Implement python module Assembler
	py::module mAssembler = m.def_submodule("Assembler", "x64dbg Assembler python script wrapper");
	mAssembler.def("Assemble", &PyWrapper::Assembler::Assemble);
	mAssembler.def("AssembleEx", &PyWrapper::Assembler::AssembleEx);
	mAssembler.def("AssembleMem", &Assembler::AssembleMem);
	mAssembler.def("AssembleMemEx", &PyWrapper::Assembler::AssembleMemEx);

	//Implement python module Bookmark
	//We ignore to implement SCRIPT_EXPORT bool Add(const ArgumentInfo* info);
	py::module mBookmark = m.def_submodule("Bookmark", "x64dbg Bookmark python script wrapper");

	py::class_<PyWrapper::Bookmark::pyBookmarkInfo>(mBookmark, "ArgumentInfo")
		.def(py::init<>())
		.def_readwrite("mod", &PyWrapper::Bookmark::pyBookmarkInfo::mod)
		.def_readwrite("rva", &PyWrapper::Bookmark::pyBookmarkInfo::rva)
		.def_readwrite("manual", &PyWrapper::Bookmark::pyBookmarkInfo::manual);

	mBookmark.def("Set", static_cast<bool (*)(duint, bool)>(&Bookmark::Set));
	mBookmark.def("Get", &Bookmark::Get);
	mBookmark.def("GetInfo", &PyWrapper::Bookmark::GetInfo);
	mBookmark.def("Delete", &Bookmark::Delete);
	mBookmark.def("DeleteRange", &Bookmark::DeleteRange);
	mBookmark.def("Clear", &Bookmark::Clear);
	mBookmark.def("GetList", &PyWrapper::Bookmark::GetList);

	//Implement module comment
	py::module mComment = m.def_submodule("Comment", "x64dbg Comment python script wrapper");

	py::class_<PyWrapper::Comment::pyCommentInfo>(mComment, "CommentInfo")
		.def(py::init<>())
		.def_readwrite("mod", &PyWrapper::Comment::pyCommentInfo::mod)
		.def_readwrite("rva", &PyWrapper::Comment::pyCommentInfo::rva)
		.def_readwrite("text", &PyWrapper::Comment::pyCommentInfo::text)
		.def_readwrite("manual", &PyWrapper::Comment::pyCommentInfo::manual);

	//We ignore implement SCRIPT_EXPORT bool Set(const CommentInfo* info);
	mComment.def("Set", static_cast<bool (*)(duint , const char* , bool)>(&Comment::Set));
	mComment.def("Get", &PyWrapper::Comment::Get);
	mComment.def("GetInfo", &PyWrapper::Comment::GetInfo);
	mComment.def("Delete", &Comment::Delete);
	mComment.def("DeleteRange", &Comment::DeleteRange);
	mComment.def("Clear", &Comment::Clear);
	mComment.def("GetList", &PyWrapper::Comment::GetList);

	//Implement module Debug
	py::module mDebug = m.def_submodule("Debug", "x64dbg Debug python script wrapper");

	py::enum_<Debug::HardwareType>(mDebug, "HardwareType")
		.value("HardwareAccess", Debug::HardwareType::HardwareAccess)
		.value("HardwareWrite", Debug::HardwareType::HardwareWrite)
		.value("HardwareExecute", Debug::HardwareType::HardwareExecute);

	mDebug.def("Wait", &Debug::Wait);
	mDebug.def("Run", &Debug::Run);
	mDebug.def("Pause", &Debug::Pause);
	mDebug.def("Stop", &Debug::Stop);
	mDebug.def("StepIn", &Debug::StepIn);
	mDebug.def("StepOver", &Debug::StepOver);
	mDebug.def("StepOut", &Debug::StepOut);
	mDebug.def("SetBreakpoint", &Debug::SetBreakpoint);
	mDebug.def("DeleteBreakpoint", &Debug::DeleteBreakpoint);
	mDebug.def("DisableBreakpoint", &Debug::DisableBreakpoint);
	mDebug.def("SetHardwareBreakpoint", &Debug::SetHardwareBreakpoint);
	mDebug.def("DeleteHardwareBreakpoint", &Debug::DeleteHardwareBreakpoint);

	//Implement module Flag
	py::module mFlag = m.def_submodule("Flag", "x64dbg Flag python script wrapper");

	py::enum_<Flag::FlagEnum>(mFlag, "FlagEnum")
		.value("ZF", Flag::FlagEnum::ZF)
		.value("OF", Flag::FlagEnum::OF)
		.value("CF", Flag::FlagEnum::CF)
		.value("PF", Flag::FlagEnum::PF)
		.value("SF", Flag::FlagEnum::SF)
		.value("TF", Flag::FlagEnum::TF)
		.value("AF", Flag::FlagEnum::AF)
		.value("DF", Flag::FlagEnum::DF)
		.value("IF", Flag::FlagEnum::IF);

	mFlag.def("Get", &Flag::Get);
	mFlag.def("Set", &Flag::Set);
	mFlag.def("GetZF", &Flag::GetZF);
	mFlag.def("SetZF", &Flag::SetZF);
	mFlag.def("GetOF", &Flag::GetOF);
	mFlag.def("SetOF", &Flag::SetOF);
	mFlag.def("GetCF", &Flag::GetCF);
	mFlag.def("SetCF", &Flag::SetCF);
	mFlag.def("GetPF", &Flag::GetPF);
	mFlag.def("SetPF", &Flag::SetPF);
	mFlag.def("GetSF", &Flag::GetSF);
	mFlag.def("SetSF", &Flag::SetSF);
	mFlag.def("GetTF", &Flag::GetTF);
	mFlag.def("SetTF", &Flag::SetTF);
	mFlag.def("GetAF", &Flag::GetAF);
	mFlag.def("SetAF", &Flag::SetAF);
	mFlag.def("GetDF", &Flag::GetDF);
	mFlag.def("SetDF", &Flag::SetDF);
	mFlag.def("GetIF", &Flag::GetIF);
	mFlag.def("SetIF", &Flag::SetIF);

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

	//Implement module Gui
	py::module mGui = m.def_submodule("Gui", "x64dbg Gui python script wrapper");

	py::enum_<Gui::Window>(mGui, "Window")
		.value("DisassemblyWindow", Gui::Window::DisassemblyWindow)
		.value("DumpWindow", Gui::Window::DumpWindow)
		.value("StackWindow", Gui::Window::StackWindow)
		.value("GraphWindow", Gui::Window::GraphWindow)
		.value("MemMapWindow", Gui::Window::MemMapWindow)
		.value("SymModWindow", Gui::Window::SymModWindow);

	//We ignore 3 function AddQWidgetTab, ShowQWidgetTab and CloseQWidgetTab
	mGui.def("SelectionGet", &PyWrapper::Gui::SelectionGet);
	mGui.def("SelectionSet", &Gui::SelectionSet);
	mGui.def("SelectionGetStart", &Gui::SelectionGetStart);
	mGui.def("SelectionGetEnd", &Gui::SelectionGetEnd);
	mGui.def("Message", &Gui::Message);
	mGui.def("MessageYesNo", &Gui::MessageYesNo);
	mGui.def("InputLine", &PyWrapper::Gui::InputLine);
	mGui.def("InputValue", &PyWrapper::Gui::InputValue);
	mGui.def("Refresh", &Gui::Refresh);

	py::module mDisassembly = mGui.def_submodule("Disassembly", "x64dbg Gui::Disassembly python script wrapper");
	mDisassembly.def("SelectionGet", &PyWrapper::Gui::Disassembly::SelectionGet);
	mDisassembly.def("SelectionSet", &Gui::Disassembly::SelectionSet);
	mDisassembly.def("SelectionGetStart", &Gui::Disassembly::SelectionGetStart);
	mDisassembly.def("SelectionGetEnd", &Gui::Disassembly::SelectionGetEnd);

	py::module mDump = mGui.def_submodule("Dump", "x64dbg Gui::Dump python script wrapper");
	mDump.def("SelectionGet", &PyWrapper::Gui::Dump::SelectionGet);
	mDump.def("SelectionSet", &Gui::Dump::SelectionSet);
	mDump.def("SelectionGetStart", &Gui::Dump::SelectionGetStart);
	mDump.def("SelectionGetEnd", &Gui::Dump::SelectionGetEnd);

	py::module mGuiStack = mGui.def_submodule("Stack", "x64dbg Gui::Stack python script wrapper");
	mGuiStack.def("SelectionGet", &PyWrapper::Gui::Stack::SelectionGet);
	mGuiStack.def("SelectionSet", &Gui::Stack::SelectionSet);
	mGuiStack.def("SelectionGetStart", &Gui::Stack::SelectionGetStart);
	mGuiStack.def("SelectionGetEnd", &Gui::Stack::SelectionGetEnd);

	py::module mGraph = mGui.def_submodule("Graph", "x64dbg Gui::Graph python script wrapper");
	mGraph.def("SelectionGetStart", &Gui::Graph::SelectionGetStart);

	py::module mMemMap = mGui.def_submodule("MemMap", "x64dbg Gui::MemMap python script wrapper");
	mMemMap.def("SelectionGetStart", &Gui::MemMap::SelectionGetStart);

	py::module mSymMod = mGui.def_submodule("SymMod", "x64dbg Gui::SymMod python script wrapper");
	mSymMod.def("SelectionGetStart", &Gui::SymMod::SelectionGetStart);

	//Implement module Label
	py::module mLabel = m.def_submodule("Label", "x64dbg Label python script wrapper");
	
	py::class_<PyWrapper::Label::pyLabelInfo>(mLabel, "LabelInfo")
		.def(py::init<>())
		.def_readwrite("mod", &PyWrapper::Label::pyLabelInfo::mod)
		.def_readwrite("rva", &PyWrapper::Label::pyLabelInfo::rva)
		.def_readwrite("text", &PyWrapper::Label::pyLabelInfo::text)
		.def_readwrite("manual", &PyWrapper::Label::pyLabelInfo::manual);

	//We ignore implement SCRIPT_EXPORT bool Set(const LabelInfo* info);
	mLabel.def("Set", static_cast<bool (*)(duint, const char*, bool)>(&Label::Set));
	mLabel.def("FromString", &Label::FromString);
	mLabel.def("Get", &PyWrapper::Label::Get);
	mLabel.def("GetInfo", &PyWrapper::Label::GetInfo);
	mLabel.def("Delete", &Label::Delete);
	mLabel.def("DeleteRange", &Label::DeleteRange);
	mLabel.def("Clear", &Label::Clear);
	mLabel.def("GetList", &PyWrapper::Label::GetList);

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

	//Implement module Misc
	py::module mMisc = m.def_submodule("Misc", "x64dbg Misc python script wrapper");

	mMisc.def("ParseExpression", &Misc::ParseExpression);
	mMisc.def("RemoteGetProcAddress", &Misc::RemoteGetProcAddress);
	mMisc.def("ResolveLabel", &Misc::ResolveLabel);
	mMisc.def("Alloc", &Misc::Alloc);
	mMisc.def("Free", &Misc::Free);

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

	//Implement module Pattern
	py::module mPattern = m.def_submodule("Pattern", "x64dbg Pattern python script wrapper");
	mPattern.def("Find", &Pattern::Find);
	mPattern.def("FindMem", &Pattern::FindMem);
	mPattern.def("Write", &Pattern::Write);
	mPattern.def("WriteMem", &Pattern::WriteMem);
	mPattern.def("SearchAndReplace", &Pattern::SearchAndReplace);
	mPattern.def("SearchAndReplaceMem", &Pattern::SearchAndReplaceMem);

	//Implement module Register
	py::module mRegister = m.def_submodule("Register", "x64dbg Register python script wrapper");

	py::enum_<Register::RegisterEnum>(mRegister, "RegisterEnum")
		.value("DR0", Register::RegisterEnum::DR0)
		.value("DR1", Register::RegisterEnum::DR1)
		.value("DR2", Register::RegisterEnum::DR2)
		.value("DR3", Register::RegisterEnum::DR3)
		.value("DR6", Register::RegisterEnum::DR6)
		.value("DR7", Register::RegisterEnum::DR7)
		.value("EAX", Register::RegisterEnum::EAX)
		.value("AX", Register::RegisterEnum::AX)
		.value("AH", Register::RegisterEnum::AH)
		.value("AL", Register::RegisterEnum::AL)
		.value("EBX", Register::RegisterEnum::EBX)
		.value("BX", Register::RegisterEnum::BX)
		.value("BH", Register::RegisterEnum::BH)
		.value("BL", Register::RegisterEnum::BL)
		.value("ECX", Register::RegisterEnum::ECX)
		.value("CX", Register::RegisterEnum::CX)
		.value("CH", Register::RegisterEnum::CH)
		.value("CL", Register::RegisterEnum::CL)
		.value("EDX", Register::RegisterEnum::EDX)
		.value("DX", Register::RegisterEnum::DX)
		.value("DH", Register::RegisterEnum::DH)
		.value("DL", Register::RegisterEnum::DL)
		.value("EDI", Register::RegisterEnum::EDI)
		.value("DI", Register::RegisterEnum::DI)
		.value("ESI", Register::RegisterEnum::ESI)
		.value("SI", Register::RegisterEnum::SI)
		.value("EBP", Register::RegisterEnum::EBP)
		.value("BP", Register::RegisterEnum::BP)
		.value("ESP", Register::RegisterEnum::ESP)
		.value("SP", Register::RegisterEnum::SP)
		.value("EIP", Register::RegisterEnum::EIP)

#ifdef _WIN64
		.value("RAX", Register::RegisterEnum::RAX)
		.value("RBX", Register::RegisterEnum::RBX)
		.value("RCX", Register::RegisterEnum::RCX)
		.value("RDX", Register::RegisterEnum::RDX)
		.value("RSI", Register::RegisterEnum::RSI)
		.value("SIL", Register::RegisterEnum::SIL)
		.value("RDI", Register::RegisterEnum::RDI)
		.value("DIL", Register::RegisterEnum::DIL)
		.value("RBP", Register::RegisterEnum::RBP)
		.value("BPL", Register::RegisterEnum::BPL)
		.value("RSP", Register::RegisterEnum::RSP)
		.value("SPL", Register::RegisterEnum::SPL)
		.value("RIP", Register::RegisterEnum::RIP)
		.value("R8", Register::RegisterEnum::R8)
		.value("R8D", Register::RegisterEnum::R8D)
		.value("R8W", Register::RegisterEnum::R8W)
		.value("R8B", Register::RegisterEnum::R8B)
		.value("R9", Register::RegisterEnum::R9)
		.value("R9D", Register::RegisterEnum::R9D)
		.value("R9W", Register::RegisterEnum::R9W)
		.value("R9B", Register::RegisterEnum::R9B)
		.value("R10", Register::RegisterEnum::R10)
		.value("R10D", Register::RegisterEnum::R10D)
		.value("R10W", Register::RegisterEnum::R10W)
		.value("R10B", Register::RegisterEnum::R10B)
		.value("R11", Register::RegisterEnum::R11)
		.value("R11D", Register::RegisterEnum::R11D)
		.value("R11W", Register::RegisterEnum::R11W)
		.value("R11B", Register::RegisterEnum::R11B)
		.value("R12", Register::RegisterEnum::R12)
		.value("R12D", Register::RegisterEnum::R12D)
		.value("R12W", Register::RegisterEnum::R12W)
		.value("R12B", Register::RegisterEnum::R12B)
		.value("R13", Register::RegisterEnum::R13)
		.value("R13D", Register::RegisterEnum::R13D)
		.value("RR13W", Register::RegisterEnum::R13W)
		.value("R13B", Register::RegisterEnum::R13B)
		.value("R14", Register::RegisterEnum::R14)
		.value("R14D", Register::RegisterEnum::R14D)
		.value("R14W", Register::RegisterEnum::R14W)
		.value("R14B", Register::RegisterEnum::R14B)
		.value("R15", Register::RegisterEnum::R15)
		.value("R15D", Register::RegisterEnum::R15D)
		.value("R15W", Register::RegisterEnum::R15W)
		.value("R15B", Register::RegisterEnum::R15B)
#endif
		.value("CIP", Register::RegisterEnum::CIP)
		.value("CSP", Register::RegisterEnum::CSP)
		.value("CAX", Register::RegisterEnum::CAX)
		.value("CBX", Register::RegisterEnum::CBX)
		.value("CCX", Register::RegisterEnum::CCX)
		.value("CDX", Register::RegisterEnum::CDX)
		.value("CDI", Register::RegisterEnum::CDI)
		.value("CSI", Register::RegisterEnum::CSI)
		.value("CBP", Register::RegisterEnum::CBP)
		.value("CFLAGS", Register::RegisterEnum::CFLAGS);

	mRegister.def("Get", &Register::Get);
	mRegister.def("Set", &Register::Set);
	mRegister.def("Size", &Register::Size);
	mRegister.def("GetDR0", &Register::GetDR0);
	mRegister.def("SetDR0", &Register::SetDR0);
	mRegister.def("GetDR1", &Register::GetDR1);
	mRegister.def("SetDR1", &Register::SetDR1);
	mRegister.def("GetDR2", &Register::GetDR2);
	mRegister.def("SetDR2", &Register::SetDR2);
	mRegister.def("GetDR3", &Register::GetDR3);
	mRegister.def("SetDR3", &Register::SetDR3);
	mRegister.def("GetDR6", &Register::GetDR6);
	mRegister.def("SetDR6", &Register::SetDR6);
	mRegister.def("GetDR7", &Register::GetDR7);
	mRegister.def("SetDR7", &Register::SetDR7);
	mRegister.def("GetEAX", &Register::GetEAX);
	mRegister.def("SetEAX", &Register::SetEAX);
	mRegister.def("GetAX", &Register::GetAX);
	mRegister.def("SetAX", &Register::SetAX);
	mRegister.def("GetAH", &Register::GetAH);
	mRegister.def("SetAH", &Register::SetAH);
	mRegister.def("GetAL", &Register::GetAL);
	mRegister.def("SetAL", &Register::SetAL);
	mRegister.def("GetEBX", &Register::GetEBX);
	mRegister.def("SetEBX", &Register::SetEBX);
	mRegister.def("GetBX", &Register::GetBX);
	mRegister.def("SetBX", &Register::SetBX);
	mRegister.def("GetBH", &Register::GetBH);
	mRegister.def("SetBH", &Register::SetBH);
	mRegister.def("GetBL", &Register::GetBL);
	mRegister.def("SetBL", &Register::SetBL);
	mRegister.def("GetECX", &Register::GetECX);
	mRegister.def("SetECX", &Register::SetECX);
	mRegister.def("GetCX", &Register::GetCX);
	mRegister.def("SetCX", &Register::SetCX);
	mRegister.def("GetCH", &Register::GetCH);
	mRegister.def("SetCH", &Register::SetCH);
	mRegister.def("GetCL", &Register::GetCL);
	mRegister.def("SetCL", &Register::SetCL);
	mRegister.def("GetEDX", &Register::GetEDX);
	mRegister.def("SetEDX", &Register::SetEDX);
	mRegister.def("GetDX", &Register::GetDX);
	mRegister.def("SetDX", &Register::SetDX);
	mRegister.def("GetDH", &Register::GetDH);
	mRegister.def("SetDH", &Register::SetDH);
	mRegister.def("GetDL", &Register::GetDL);
	mRegister.def("SetDL", &Register::SetDL);
	mRegister.def("GetEDI", &Register::GetEDI);
	mRegister.def("SetEDI", &Register::SetEDI);
	mRegister.def("GetDI", &Register::GetDI);
	mRegister.def("SetDI", &Register::SetDI);
	mRegister.def("GetESI", &Register::GetESI);
	mRegister.def("SetESI", &Register::SetESI);
	mRegister.def("GetSI", &Register::GetSI);
	mRegister.def("SetSI", &Register::SetSI);
	mRegister.def("GetEBP", &Register::GetEBP);
	mRegister.def("SetEBP", &Register::SetEBP);
	mRegister.def("GetBP", &Register::GetBP);
	mRegister.def("SetBP", &Register::SetBP);
	mRegister.def("GetESP", &Register::GetESP);
	mRegister.def("SetESP", &Register::SetESP);
	mRegister.def("GetSP", &Register::GetSP);
	mRegister.def("SetSP", &Register::SetSP);
	mRegister.def("GetEIP", &Register::GetEIP);
	mRegister.def("SetEIP", &Register::SetEIP);

#ifdef _WIN64
	mRegister.def("GetRAX", &Register::GetRAX);
	mRegister.def("SetRAX", &Register::SetRAX);
	mRegister.def("GetRBX", &Register::GetRBX);
	mRegister.def("SetRBX", &Register::SetRBX);
	mRegister.def("GetRCX", &Register::GetRCX);
	mRegister.def("SetRCX", &Register::SetRCX);
	mRegister.def("GetRDX", &Register::GetRDX);
	mRegister.def("SetRDX", &Register::SetRDX);
	mRegister.def("GetRSI", &Register::GetRSI);
	mRegister.def("SetRSI", &Register::SetRSI);
	mRegister.def("GetSIL", &Register::GetSIL);
	mRegister.def("SetSIL", &Register::SetSIL);
	mRegister.def("GetRDI", &Register::GetRDI);
	mRegister.def("SetRDI", &Register::SetRDI);
	mRegister.def("GetDIL", &Register::GetDIL);
	mRegister.def("SetDIL", &Register::SetDIL);
	mRegister.def("GetRBP", &Register::GetRBP);
	mRegister.def("SetRBP", &Register::SetRBP);
	mRegister.def("GetBPL", &Register::GetBPL);
	mRegister.def("SetBPL", &Register::SetBPL);
	mRegister.def("GetRSP", &Register::GetRSP);
	mRegister.def("SetRSP", &Register::SetRSP);
	mRegister.def("GetSPL", &Register::GetSPL);
	mRegister.def("SetSPL", &Register::SetSPL);
	mRegister.def("GetRIP", &Register::GetRIP);
	mRegister.def("SetRIP", &Register::SetRIP);
	mRegister.def("GetR8", &Register::GetR8);
	mRegister.def("SetR8", &Register::SetR8);
	mRegister.def("GetR8D", &Register::GetR8D);
	mRegister.def("SetR8D", &Register::SetR8D);
	mRegister.def("GetR8W", &Register::GetR8W);
	mRegister.def("SetR8W", &Register::SetR8W);
	mRegister.def("GetR8B", &Register::GetR8B);
	mRegister.def("SetR8B", &Register::SetR8B);
	mRegister.def("GetR9", &Register::GetR9);
	mRegister.def("SetR9", &Register::SetR9);
	mRegister.def("GetR9D", &Register::GetR9D);
	mRegister.def("SetR9D", &Register::SetR9D);
	mRegister.def("GetR9W", &Register::GetR9W);
	mRegister.def("SetR9W", &Register::SetR9W);
	mRegister.def("GetR9B", &Register::GetR9B);
	mRegister.def("SetR9B", &Register::SetR9B);
	mRegister.def("GetR10", &Register::GetR10);
	mRegister.def("SetR10", &Register::SetR10);
	mRegister.def("GetR10D", &Register::GetR10D);
	mRegister.def("SetR10D", &Register::SetR10D);
	mRegister.def("GetR10W", &Register::GetR10W);
	mRegister.def("SetR10W", &Register::SetR10W);
	mRegister.def("GetR10B", &Register::GetR10B);
	mRegister.def("SetR10B", &Register::SetR10B);
	mRegister.def("GetR11", &Register::GetR11);
	mRegister.def("SetR11", &Register::SetR11);
	mRegister.def("GetR11D", &Register::GetR11D);
	mRegister.def("SetR11D", &Register::SetR11D);
	mRegister.def("GetR11W", &Register::GetR11W);
	mRegister.def("SetR11W", &Register::SetR11W);
	mRegister.def("GetR11B", &Register::GetR11B);
	mRegister.def("SetR11B", &Register::SetR11B);
	mRegister.def("GetR12", &Register::GetR12);
	mRegister.def("SetR12", &Register::SetR12);
	mRegister.def("GetR12D", &Register::GetR12D);
	mRegister.def("SetR12D", &Register::SetR12D);
	mRegister.def("GetR12W", &Register::GetR12W);
	mRegister.def("SetR12W", &Register::SetR12W);
	mRegister.def("GetR12B", &Register::GetR12B);
	mRegister.def("SetR12B", &Register::SetR12B);
	mRegister.def("GetR13", &Register::GetR13);
	mRegister.def("SetR13", &Register::SetR13);
	mRegister.def("GetR13D", &Register::GetR13D);
	mRegister.def("SetR13D", &Register::SetR13D);
	mRegister.def("GetR13W", &Register::GetR13W);
	mRegister.def("SetR13W", &Register::SetR13W);
	mRegister.def("GetR13B", &Register::GetR13B);
	mRegister.def("SetR13B", &Register::SetR13B);
	mRegister.def("GetR14", &Register::GetR14);
	mRegister.def("SetR14", &Register::SetR14);
	mRegister.def("GetR14D", &Register::GetR14D);
	mRegister.def("SetR14D", &Register::SetR14D);
	mRegister.def("GetR14W", &Register::GetR14W);
	mRegister.def("SetR14W", &Register::SetR14W);
	mRegister.def("GetR14B", &Register::GetR14B);
	mRegister.def("SetR14B", &Register::SetR14B);
	mRegister.def("GetR15", &Register::GetR15);
	mRegister.def("SetR15", &Register::SetR15);
	mRegister.def("GetR15D", &Register::GetR15D);
	mRegister.def("SetR15D", &Register::SetR15D);
	mRegister.def("GetR15W", &Register::GetR15W);
	mRegister.def("SetR15W", &Register::SetR15W);
	mRegister.def("GetR15B", &Register::GetR15B);
	mRegister.def("SetR15B", &Register::SetR15B);

#endif
	mRegister.def("GetCAX", &Register::GetCAX);
	mRegister.def("SetCAX", &Register::SetCAX);
	mRegister.def("GetCBX", &Register::GetCBX);
	mRegister.def("SetCBX", &Register::SetCBX);
	mRegister.def("GetCCX", &Register::GetCCX);
	mRegister.def("SetCCX", &Register::SetCCX);
	mRegister.def("GetCDX", &Register::GetCDX);
	mRegister.def("SetCDX", &Register::SetCDX);
	mRegister.def("GetCDI", &Register::GetCDI);
	mRegister.def("SetCDI", &Register::SetCDI);
	mRegister.def("GetCSI", &Register::GetCSI);
	mRegister.def("SetCSI", &Register::SetCSI);
	mRegister.def("GetCBP", &Register::GetCBP);
	mRegister.def("SetCBP", &Register::SetCBP);
	mRegister.def("GetCSP", &Register::GetCSP);
	mRegister.def("SetCSP", &Register::SetCSP);
	mRegister.def("GetCIP", &Register::GetCIP);
	mRegister.def("SetCIP", &Register::SetCIP);
	mRegister.def("GetCFLAGS", &Register::GetCFLAGS);
	mRegister.def("SetCFLAGS", &Register::SetCFLAGS);

	//Implement module Stack
	py::module mStack = m.def_submodule("Stack", "x64dbg Stack python script wrapper");

	mStack.def("Pop", &Stack::Pop);
	mStack.def("Push", &Stack::Push);
	mStack.def("Peek", &Stack::Peek);

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