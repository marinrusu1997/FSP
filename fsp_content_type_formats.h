#pragma once
#include <inttypes.h>
#include <string_view>

namespace fsp::protocol::content_type_formats {
	enum class content_type_format : int8_t
	{
		JSON,
		CSV_LIST,
		CSV_MAP
	};

	constexpr static const char * const JSON			= "application/json";
	constexpr static const char * const CSV_LIST		= "text/csvl";
	constexpr static const char * const CSV_MAP			= "text/csvm";

	_NODISCARD inline const bool isValidContentTypeFormat(_STD string_view format) noexcept
	{
		if (format != JSON && format != CSV_LIST && format != CSV_MAP)
			return false;
		return true;
	}
}