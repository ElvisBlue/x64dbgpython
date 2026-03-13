#include "wrapper.h"

namespace PyWrapper
{
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

		std::optional<pyCommentInfo> GetInfo(duint addr)
		{
			Script::Comment::CommentInfo tmpInfo = { 0 };
			if (Script::Comment::GetInfo(addr, &tmpInfo))
			{
				pyCommentInfo info;
				info.manual = tmpInfo.manual;
				info.mod = tmpInfo.mod;
				info.rva = tmpInfo.rva;
				info.text = tmpInfo.text;
				return info;
			}
			else
				return std::nullopt;
		}

		std::optional<std::vector<pyCommentInfo>> GetList()
		{
			auto tmpList = std::make_unique<ListInfo>();
			if (Script::Comment::GetList(tmpList.get()))
			{
				std::vector<pyCommentInfo> list;
				std::vector<Script::Comment::CommentInfo> commentInfoList;
				BridgeList<Script::Comment::CommentInfo>::ToVector(tmpList.get(), commentInfoList, true);
				list.reserve(commentInfoList.size());
				for (auto it = commentInfoList.begin(); it != commentInfoList.end(); it++)
				{
					pyCommentInfo item;
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

void InitWrapperComment(py::module_& m)
{
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


}
