#pragma once
#include "FileStorageErrorCode.h"
#include <algorithm>
#include <iterator>

namespace files_storage::policies {
	template<typename File, template<class, class...> class SequenceContainer, class... Args>
	struct RAMStoragePolicy
	{
		RAMStoragePolicy() = default;

		explicit RAMStoragePolicy(std::initializer_list<File> initial_files) : files_{ initial_files }
		{}

		RAMStoragePolicy(RAMStoragePolicy const&) = delete;
		RAMStoragePolicy& operator=(RAMStoragePolicy const&) = delete;

		_NODISCARD const auto Store(SequenceContainer<File, Args...>& source) noexcept
		{
			try {
				std::move(source.begin(), source.end(), std::insert_iterator(files_, files_.end()));
				source.clear();
				return FileStorageErrorCode{ file_storage_ec::succes };
			}
			catch (...) {
				return FileStorageErrorCode{ file_storage_ec::no_available_space };
			}
		}
		_NODISCARD inline const auto Store(File const& file) noexcept
		{
			try {
				std::generate_n(std::inserter(files_, files_.begin()), 1, [&]() -> File {return { std::move(file) }; });
				return FileStorageErrorCode{ file_storage_ec::succes };
			}
			catch (...) {
				return FileStorageErrorCode{ file_storage_ec::no_available_space };
			}
		}
		_NODISCARD const auto Update(File const& old_file, File const& new_file) noexcept
		{
			try {
				if (const auto& iter = RetrieveIterator(old_file); iter != files_.end())
				{
					files_.erase(iter);
					return Store(new_file);
				}
				return FileStorageErrorCode{ file_storage_ec::file_not_found };
			}
			catch (...) {
				return FileStorageErrorCode{ file_storage_ec::update_failed };
			}
		}
		_NODISCARD inline constexpr const auto Retrieve(File const& file) const noexcept
		{
			return RetrieveIterator(file) != files_.end() ? FileStorageErrorCode{ file_storage_ec::succes } : FileStorageErrorCode{ file_storage_ec::file_not_found };
		}
		_NODISCARD const auto Delete(File const& File) noexcept
		{
			try {
				if (const auto& iter = RetrieveIterator(File); iter != files_.end()) {
					files_.erase(iter);
					return FileStorageErrorCode{ file_storage_ec::succes };
				}
				return FileStorageErrorCode{ file_storage_ec::file_not_found };
			}
			catch (...) {
				return FileStorageErrorCode{ file_storage_ec::delete_failed };
			}
		}
		_NODISCARD const auto RelativeDelete(File const& RelativePath, SequenceContainer<File, Args...>& DeletedFiles) noexcept
		{
			try {

			}
			catch (...) {
				return FileStorageErrorCode{ file_storage_ec::delete_failed };
			}
		}
		_NODISCARD const auto Files(SequenceContainer<File, Args...>& storage) const noexcept
		{
			try {
				std::copy(files_.begin(), files_.end(), std::inserter(storage, storage.begin()));
				return FileStorageErrorCode{ file_storage_ec::succes };
			}
			catch (...) {
				return FileStorageErrorCode{ file_storage_ec::copy_files_failed };
			}
		}
		_NODISCARD const auto& Files() const noexcept
		{
			return files_;
		}
	private:
		_NODISCARD inline constexpr const auto RetrieveIterator(File const& file) const noexcept
		{
			return std::find_if(files_.begin(), files_.end(), [&](const auto& current_file) {return current_file == file; });
		}

		SequenceContainer<File, Args...> files_;
	};
}