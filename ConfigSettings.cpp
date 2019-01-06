#include "ConfigSettings.h"
#include <json.hpp>
#include <fstream>
#include <sstream>

// for convenience
using json = nlohmann::json;
using namespace persistency;

ConfigSettings::ConfigSettings() noexcept : dirtyBit_(false)
{
	if (std::ifstream is(_PATH_TO_CONFIG_SETTINGS_FILE_); is.is_open())
	{
		std::stringstream buffer;
		buffer << is.rdbuf();
		json().parse(buffer.str()).get_to(settings_);
	}
}

ConfigSettings::~ConfigSettings()
{
	if (dirtyBit_)
		if (std::ofstream os(_PATH_TO_CONFIG_SETTINGS_FILE_, std::ios::trunc); os.is_open())
			os << json(settings_).dump(4);
}

void				ConfigSettings::set(const _STD string_view key, const value_type&  values) noexcept
{
	dirtyBit_ = true;
	settings_.emplace(key.data(), values);
}

_NODISCARD auto	ConfigSettings::try_get(const _STD string_view key, const value_type& defaults_) noexcept -> const value_type&
{
	if (auto iter = settings_.find(key); iter != settings_.end())
		return iter->second;
	set(key, defaults_);
	return defaults_;
}

size_t ConfigSettings::remove(const _STD string_view key) noexcept
{
	dirtyBit_ = true;
	return settings_.erase(key.data());
}

void			ConfigSettings::reset() noexcept
{
	dirtyBit_ = true;
	settings_.clear();
}
