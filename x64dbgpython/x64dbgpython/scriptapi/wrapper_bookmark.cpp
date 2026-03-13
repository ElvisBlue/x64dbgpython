#include "wrapper.h"

namespace PyWrapper
{
	namespace Bookmark
	{
		struct pyBookmarkInfo
		{
			std::string mod;
			duint rva;
			bool manual;
		};


		std::optional<pyBookmarkInfo> GetInfo(duint addr)
		{
			Script::Bookmark::BookmarkInfo tmpInfo = { 0 };
			if (Script::Bookmark::GetInfo(addr, &tmpInfo))
			{
				pyBookmarkInfo info;
				info.manual = tmpInfo.manual;
				info.mod = tmpInfo.mod;
				info.rva = tmpInfo.rva;
				return info;
			}
			else
				return std::nullopt;
		}

		std::optional<std::vector<pyBookmarkInfo>> GetList()
		{
			auto tmpList = std::make_unique<ListInfo>();
			if (Script::Bookmark::GetList(tmpList.get()))
			{
				std::vector<pyBookmarkInfo> list;
				std::vector<Script::Bookmark::BookmarkInfo> bookmarkInfoList;
				BridgeList<Script::Bookmark::BookmarkInfo>::ToVector(tmpList.get(), bookmarkInfoList, true);
				list.reserve(bookmarkInfoList.size());
				for (auto it = bookmarkInfoList.begin(); it != bookmarkInfoList.end(); it++)
				{
					pyBookmarkInfo item;
					item.manual = it->manual;
					item.mod = it->mod;
					item.rva = it->rva;
					list.push_back(item);
				}
				return list;
			}

			return std::nullopt;
		}
	}

}

void init_wrapper_bookmark(py::module_& m)
{
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


}
