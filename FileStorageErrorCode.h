#pragma once
#include <inttypes.h>
#include <string_view>

namespace files_storage {
	enum class file_storage_ec : uint16_t
	{
		succes,
		no_available_space,
		update_failed,
		file_not_found,
		empty_storage,
		delete_failed,
		copy_files_failed
	};

	struct FileStorageErrorCode
	{
		explicit FileStorageErrorCode(file_storage_ec ec) : ec_(ec)
		{}

		[[nodiscard]] operator bool() const noexcept
		{
			return ec_ == file_storage_ec::succes ? false : true;
		}
		[[nodiscard]] bool operator!() const noexcept
		{
			return !this->operator bool();
		}
		[[nodiscard]] explicit operator std::string_view() const noexcept
		{
			switch (ec_)
			{
			case file_storage_ec::succes:
				return "Succes";
			case file_storage_ec::no_available_space:
				return "No Available Space For Storing Files";
			case file_storage_ec::update_failed:
				return "Filename Was Not Updated";
			case file_storage_ec::file_not_found:
				return "File Not Found";
			case file_storage_ec::empty_storage:
				return "Storage Is Empty";
			case file_storage_ec::delete_failed:
				return "File Was Not Deleted";
			case file_storage_ec::copy_files_failed:
				return "Files Wasn't Copied";
			default:
				return "UNKNOWN ERROR CODE";
			}
		}
		[[nodiscard]] const auto& erorr_code() const noexcept { return ec_; }
	private:
		file_storage_ec ec_;
	};
}