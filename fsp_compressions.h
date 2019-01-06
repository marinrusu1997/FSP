#pragma once
#include <map>
#include <string_view>
#include <algorithm>
#include <forward_list>
#include <boost/algorithm/string/split.hpp>

#include "protocol.h"

namespace fsp::protocol::compressions
{
	enum class compression : uint8_t
	{
		bzip2,
		gzip,
		lzma,
		zlib,
		nocompression
	};

	constexpr static const char * const BZIP2			= "bzip2";
	constexpr static const char * const GZIP			= "gzip";
	constexpr static const char * const LZMA			= "lzma";
	constexpr static const char * const ZLIB			= "zlib";
	constexpr static const char * const NO_COMPRESSION	= "nocompression";

	static const _STD map<compression, _STD string_view> compressions
	{
		{compression::bzip2,			BZIP2},
		{compression::gzip,				GZIP},
		{compression::lzma,				LZMA},
		{compression::zlib,				ZLIB},
		{compression::nocompression,	NO_COMPRESSION}
	};

	_NODISCARD inline const bool Compression(const _STD string_view Str, compression& c) noexcept
	{
		bool succes = false;
		if (auto&& iter = _STD find_if(compressions.begin(), compressions.end(), [&](const auto& pair) {return pair.second == Str; }); iter != compressions.end())
		{
			c = iter->first;
			succes = true;
		}
		return succes;
	}

	_NODISCARD inline const _STD forward_list<fsp::protocol::compressions::compression> CSVCompressions(const _STD string_view Compressions)
	{
		_STD forward_list<_STD string> CompressionsParts;
		boost::split(CompressionsParts, Compressions, [](const auto c) {return c == fsp::protocol::CSV_DELIMITER_CHR; });
		CompressionsParts.remove_if([](const auto& Compression) {return Compression.empty(); });
		_STD forward_list<fsp::protocol::compressions::compression> CompressionsEnum;
		for (const auto& Token : CompressionsParts)
		{
			fsp::protocol::compressions::compression c;
			if (fsp::protocol::compressions::Compression(Token, c))
				CompressionsEnum.push_front(c);
		}
		return CompressionsEnum;
	}

	_NODISCARD inline const _STD string_view Compression(compression c) noexcept
	{
		return _STD find_if(compressions.begin(), compressions.end(), [&](const auto& pair) {return pair.first == c; })->second;
	}

	_NODISCARD inline const bool IsCompressionTypeValid(_STD string_view c) noexcept
	{
		return _STD find_if(compressions.begin(), compressions.end(), [&](const auto& pair) {return pair.second == c; })
			!= compressions.end();
	}
}
