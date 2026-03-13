#pragma once
#include <Windows.h>
#include <optional>
#include <memory>
#include "pybind11/embed.h"
#include "pybind11/stl_bind.h"
#include "pybind11/stl.h"

#include "../pluginsdk/_plugins.h"
#include "../pluginsdk/_scriptapi_argument.h"
#include "../pluginsdk/_scriptapi_assembler.h"
#include "../pluginsdk/_scriptapi_bookmark.h"
#include "../pluginsdk/_scriptapi_comment.h"
#include "../pluginsdk/_scriptapi_debug.h"
#include "../pluginsdk/_scriptapi_flag.h"
#include "../pluginsdk/_scriptapi_function.h"
#include "../pluginsdk/_scriptapi_gui.h"
#include "../pluginsdk/_scriptapi_label.h"
#include "../pluginsdk/_scriptapi_memory.h"
#include "../pluginsdk/_scriptapi_misc.h"
#include "../pluginsdk/_scriptapi_module.h"
#include "../pluginsdk/_scriptapi_pattern.h"
#include "../pluginsdk/_scriptapi_register.h"
#include "../pluginsdk/_scriptapi_stack.h"
#include "../pluginsdk/_scriptapi_symbol.h"

using namespace Script;
namespace py = pybind11;

void InitWrapperArgument(py::module_& m);
void InitWrapperAssembler(py::module_& m);
void InitWrapperBookmark(py::module_& m);
void InitWrapperComment(py::module_& m);
void InitWrapperDebug(py::module_& m);
void InitWrapperFlag(py::module_& m);
void InitWrapperFunction(py::module_& m);
void InitWrapperGui(py::module_& m);
void InitWrapperLabel(py::module_& m);
void InitWrapperMemory(py::module_& m);
void InitWrapperMisc(py::module_& m);
void InitWrapperModule(py::module_& m);
void InitWrapperPattern(py::module_& m);
void InitWrapperRegister(py::module_& m);
void InitWrapperStack(py::module_& m);
void InitWrapperSymbol(py::module_& m);
