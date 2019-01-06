#pragma once

#define _PATH_TO_CONFIG_SETTINGS_FILE_  "settings.txt"

#include <map>
#include <forward_list>
#include <string>
#include <string_view>
#include <optional>

#include "KeysForConfigSettings.h"

namespace persistency {
	class ConfigSettings final
	{
	public:
		ConfigSettings() noexcept;
		~ConfigSettings();

		using key_type					= _STD string;
		using value_type				= _STD forward_list<key_type>;

		void										set(const _STD string_view key, const value_type&  values) noexcept;
		_NODISCARD const value_type&				try_get(const _STD string_view key, const value_type& defaults_) noexcept;
		size_t										remove(const _STD string_view key) noexcept;
		void										reset() noexcept;
	private:
		using comparator_type			= _STD less<>;
		
		_STD map<key_type,value_type, comparator_type >	settings_;
		bool											dirtyBit_;
	};
}

