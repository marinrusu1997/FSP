#pragma once
#include <set>
#include <string_view>

namespace fsp::protocol::headers::querries
{
	constexpr static const char * const DOWNLOAD_FILE	= "downld";
	constexpr static const char * const ADD_FILE		= "addfile";
	constexpr static const char * const REMOVE_PATH		= "rmvpath";
	constexpr static const char * const RENAME_PATH		= "rnmpath";

	static const _STD set<_STD string_view, _STD less<>> querries
	{
		DOWNLOAD_FILE,
		ADD_FILE,
		REMOVE_PATH,
		RENAME_PATH
	};

	_NODISCARD inline const bool IsQuerryTypeValid(_STD string_view querry) noexcept
	{
		return querries.find(querry) != querries.end();
	}
}