#include "gzip_compressor.h"

#include <sstream>
#include <boost/iostreams/filtering_stream.hpp>
#include <boost/iostreams/filter/gzip.hpp>
#include <boost/iostreams/copy.hpp>
#include <string>

using namespace fsp::protocol::compressors;

_STD string GzipCompressor::compress(const _STD string_view Str)  const
{
	std::stringstream sstr;
	boost::iostreams::filtering_ostream os;
	os.push(boost::iostreams::gzip_compressor{});
	os.push(sstr);

	os << Str;
	os.pop();

	return base64_encode(sstr.str());
}
_STD string GzipCompressor::decompress(const _STD string_view Str)  const
{
	std::stringstream compressed_decoded;
	std::stringstream decompressed;

	/** first decode  then decompress **/
	compressed_decoded << base64_decode(Str);

	boost::iostreams::filtering_streambuf<boost::iostreams::input> in;
	in.push(boost::iostreams::gzip_decompressor{});
	in.push(compressed_decoded);
	boost::iostreams::copy(in, decompressed);
	return decompressed.str();
}