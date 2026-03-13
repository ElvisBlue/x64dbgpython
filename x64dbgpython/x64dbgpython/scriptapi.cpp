#include "scriptapi/wrapper.h"

PYBIND11_EMBEDDED_MODULE(x64dbg, m)
{
	m.doc() = "Python module to wrap the x64dbg script api.";
	init_wrapper_argument(m);
	init_wrapper_assembler(m);
	init_wrapper_bookmark(m);
	init_wrapper_comment(m);
	init_wrapper_debug(m);
	init_wrapper_flag(m);
	init_wrapper_function(m);
	init_wrapper_gui(m);
	init_wrapper_label(m);
	init_wrapper_memory(m);
	init_wrapper_misc(m);
	init_wrapper_module(m);
	init_wrapper_pattern(m);
	init_wrapper_register(m);
	init_wrapper_stack(m);
	init_wrapper_symbol(m);
}
