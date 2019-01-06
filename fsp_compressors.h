#pragma once
#include "bzip2compressor.h"
#include "gzip_compressor.h"
#include "lzma_compressor.h"
#include "zlib_compressor.h"

#include "fsp_compressions.h"
#include <map>
#include <algorithm>



namespace fsp::protocol::compressors 
{
	using namespace fsp::protocol::compressions;

	static const _STD map<compression, AbstractCompressor*, _STD less<> > compressors
	{
		{compression::bzip2,			new Bzip2Compressor()},
		{compression::gzip,				new GzipCompressor()},
		{compression::lzma,				new LzmaCompressor()},
		{compression::zlib,				new ZlibCompressor()}
	};

	_NODISCARD inline AbstractCompressor*	GetCompressorPointer(compression c) noexcept
	{
		if (auto&& iter = _STD find_if(compressors.begin(), compressors.end(), [&](const auto& pair) {return pair.first == c; });
			iter != compressors.end())
			return iter->second;
		return nullptr;
	}
}