#include "lzma_compressor.h"

#include <sstream>
#include <boost/iostreams/filtering_stream.hpp>
#include <boost/iostreams/filter/lzma.hpp>
#include <boost/iostreams/copy.hpp>
#include <string>

using namespace fsp::protocol::compressors;

_STD string LzmaCompressor::compress(const _STD string_view Str)  const
{
	assert(0);

	return "";
}
_STD string LzmaCompressor::decompress(const _STD string_view Str)  const
{
	assert(0);

	return "";
}