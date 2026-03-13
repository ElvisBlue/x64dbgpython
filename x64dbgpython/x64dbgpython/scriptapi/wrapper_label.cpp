#include "wrapper.h"

namespace PyWrapper
{
	namespace Label
	{
		struct pyLabelInfo
		{
			std::string mod;
			duint rva;
			std::string text;
			bool manual;
		};

		duint FromString(std::string label)
		{
			duint value = 0;
			Script::Label::FromString(label.c_str(), &value);
			return value;
		}

		std::string Get(duint addr)
		{
			char retText[MAX_LABEL_SIZE] = { 0 };
			Script::Label::Get(addr, retText);
			return retText;
		}

		std::optional<pyLabelInfo> GetInfo(duint addr)
		{
			Script::Label::LabelInfo tmpInfo = { 0 };
			if (Script::Label::GetInfo(addr, &tmpInfo))
			{
				pyLabelInfo info;
				info.manual = tmpInfo.manual;
				info.mod = tmpInfo.mod;
				info.rva = tmpInfo.rva;
				info.text = tmpInfo.text;
				return info;
			}
			else
				return std::nullopt;
		}

		std::optional<std::vector<pyLabelInfo>> GetList()
		{
			auto tmpList = std::make_unique<ListInfo>();
			if (Script::Label::GetList(tmpList.get()))
			{
				std::vector<pyLabelInfo> list;
				std::vector<Script::Label::LabelInfo> labelInfoList;
				BridgeList<Script::Label::LabelInfo>::ToVector(tmpList.get(), labelInfoList, true);
				list.reserve(labelInfoList.size());
				for (auto it = labelInfoList.begin(); it != labelInfoList.end(); it++)
				{
					pyLabelInfo item;
					item.manual = it->manual;
					item.mod = it->mod;
					item.rva = it->rva;
					item.text = it->text;
					list.push_back(item);
				}
				return list;
			}

			return std::nullopt;
		}
	}

}

void InitWrapperLabel(py::module_& m)
{
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
	mLabel.def("FromString", &PyWrapper::Label::FromString);
	mLabel.def("Get", &PyWrapper::Label::Get);
	mLabel.def("GetInfo", &PyWrapper::Label::GetInfo);
	mLabel.def("Delete", &Label::Delete);
	mLabel.def("DeleteRange", &Label::DeleteRange);
	mLabel.def("Clear", &Label::Clear);
	mLabel.def("GetList", &PyWrapper::Label::GetList);


}
