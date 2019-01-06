#pragma once

#include <string_view>
#include <filesystem>

namespace files_storage::traits {
	template <typename File> struct file_name_traits {
		static bool const value = false;
	};
	template <> struct file_name_traits <std::string_view> {
		static bool const value = true;
	};
	template <> struct file_name_traits <std::string> {
		static bool const value = true;
	};
	template <> struct file_name_traits <std::filesystem::path> {
		static bool const value = true;
	};

	template<typename T>
	struct is_file_name {
		static constexpr bool const value = file_name_traits<T>::value;
	};
}