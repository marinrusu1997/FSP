#pragma once
#include <string_view>
#include "base64.h"

namespace fsp::protocol::compressors {
	struct AbstractCompressor
	{
		virtual _STD string compress(const _STD string_view Str) const = 0;
		virtual _STD string decompress(const _STD string_view Str) const = 0;
	};
}