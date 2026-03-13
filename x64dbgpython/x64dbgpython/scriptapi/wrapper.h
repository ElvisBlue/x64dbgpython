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

void init_wrapper_argument(py::module_& m);
void init_wrapper_assembler(py::module_& m);
void init_wrapper_bookmark(py::module_& m);
void init_wrapper_comment(py::module_& m);
void init_wrapper_debug(py::module_& m);
void init_wrapper_flag(py::module_& m);
void init_wrapper_function(py::module_& m);
void init_wrapper_gui(py::module_& m);
void init_wrapper_label(py::module_& m);
void init_wrapper_memory(py::module_& m);
void init_wrapper_misc(py::module_& m);
void init_wrapper_module(py::module_& m);
void init_wrapper_pattern(py::module_& m);
void init_wrapper_register(py::module_& m);
void init_wrapper_stack(py::module_& m);
void init_wrapper_symbol(py::module_& m);
