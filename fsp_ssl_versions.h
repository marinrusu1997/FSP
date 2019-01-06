#pragma once
#include <map>
#include <string_view>
#include <algorithm>

namespace fsp::protocol::ssl_versions
{
	enum class version : int8_t
	{
		sslv2,
		sslv3,
		sslv23
	};

	static const _STD map<version,_STD string_view, _STD less<> > versions_
	{
		{version::sslv2,"sslv2"},
		{version::sslv23,"sslv23"},
		{version::sslv3,"sslv3"}
	};

	_NODISCARD inline const _STD string_view Version(version version) noexcept
	{
		return _STD find_if(versions_.begin(), versions_.end(), [&](const auto& pair) {return pair.first == version; })->second;
	}

	_NODISCARD inline const version Version(const _STD string_view version) noexcept
	{
		return _STD find_if(versions_.begin(), versions_.end(), [&](const auto& pair) {return pair.second == version; })->first;
	}

	_NODISCARD inline const bool IsSslVersionValid(const _STD string_view version) noexcept
	{
		return _STD find_if(versions_.begin(), versions_.end(), [&](const auto& pair) {return pair.second == version; }) != versions_.end();
	}
}