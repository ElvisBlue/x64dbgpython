#include "wrapper.h"

void InitWrapperFlag(py::module_& m)
{
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


}
