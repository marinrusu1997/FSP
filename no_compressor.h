#pragma once
#include "abstract_compressor.h"

namespace fsp::protocol::compressors {
	struct NoCompressor final : public AbstractCompressor
	{
		void compress(const _STD string_view Str)  const override {}
		void decompress(const _STD string_view Str)  const override {}
	};
}