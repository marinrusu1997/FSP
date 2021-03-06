#pragma once
#include "abstract_compressor.h"

namespace fsp::protocol::compressors {
	struct Bzip2Compressor final : public AbstractCompressor
	{
		_STD string compress(const _STD string_view Str)   const override;
		_STD string decompress(const _STD string_view Str) const override;
	};
}