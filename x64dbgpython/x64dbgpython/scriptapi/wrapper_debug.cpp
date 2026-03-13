#include "wrapper.h"

void init_wrapper_debug(py::module_& m)
{
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


}
