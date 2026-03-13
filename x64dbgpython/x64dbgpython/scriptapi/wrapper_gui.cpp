#include "wrapper.h"

namespace PyWrapper
{
	namespace Gui
	{
		
		namespace Disassembly
		{
			std::optional<std::vector<duint>> SelectionGet()
			{
				duint start = 0;
				duint end = 0;
				if (Script::Gui::Disassembly::SelectionGet(&start, &end))
				{
					std::vector<duint> ret;
					ret.push_back(start);
					ret.push_back(end);
					return ret;
				}
				return std::nullopt;
			}
		}

		namespace Dump
		{
			std::optional<std::vector<duint>> SelectionGet()
			{
				duint start = 0;
				duint end = 0;
				if (Script::Gui::Dump::SelectionGet(&start, &end))
				{
					std::vector<duint> ret;
					ret.push_back(start);
					ret.push_back(end);
					return ret;
				}
				return std::nullopt;
			}
		}

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

		std::optional<std::vector<duint>> SelectionGet(Script::Gui::Window window)
		{
			duint start = 0;
			duint end = 0;
			if (Script::Gui::SelectionGet(window, &start, &end))
			{
				std::vector<duint> ret;
				ret.push_back(start);
				ret.push_back(end);
				return ret;
			}
			return std::nullopt;
		}
		

		std::string InputLine(const char* title)
		{
			char* text = new char[GUI_MAX_LINE_SIZE];

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

}

void init_wrapper_gui(py::module_& m)
{
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


}
