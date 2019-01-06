#pragma once
#include "FilesStorageTraits.h"
#include <initializer_list>
namespace files_storage {
	template <	typename File, template<class, class...> class SequenceContainer,
		template <class, template<class, class...> class, class... > class StoragePolicy,
		class... Args
	>
		struct FilesStorage final : private StoragePolicy<File, SequenceContainer, Args...>
	{
		FilesStorage() = default;

		explicit FilesStorage(std::initializer_list<File> files) : StoragePolicy<File, SequenceContainer, Args...>(files)
		{}

		template<typename... ConstructorArgs>
		FilesStorage(ConstructorArgs&&... args) : StoragePolicy<File, SequenceContainer, Args...>(std::forward<ConstructorArgs>(args)...)
		{}

		_NODISCARD __forceinline constexpr const auto Store(SequenceContainer<File, Args...>& container) noexcept
		{
			static_assert(traits::is_file_name<typename std::remove_const<typename std::remove_reference<File>::type>::type>::value, "File doesn't meet file name concept");
			return StoragePolicy<File, SequenceContainer, Args...>::Store(container);
		}
		_NODISCARD __forceinline constexpr const auto Store(File const& file) noexcept
		{
			static_assert(traits::is_file_name<typename std::remove_const<typename std::remove_reference<decltype(file)>::type>::type>::value, "File doesn't meet file name concept");
			return StoragePolicy<File, SequenceContainer, Args...>::Store(file);
		}
		_NODISCARD __forceinline constexpr const auto Update(File const& old_file, File const& new_file) noexcept
		{
			static_assert(traits::is_file_name<typename std::remove_const<typename std::remove_reference<decltype(old_file)>::type>::type>::value, "File doesn't meet file name concept");
			return StoragePolicy<File, SequenceContainer, Args...>::Update(old_file, new_file);
		}
		_NODISCARD __forceinline constexpr const auto Retrieve(File const& file) const noexcept
		{
			static_assert(traits::is_file_name<typename std::remove_const<typename std::remove_reference<decltype(file)>::type>::type>::value, "File doesn't meet file name concept");
			return StoragePolicy<File, SequenceContainer, Args...>::Retrieve(file);
		}
		template<class... Criterias>
		_NODISCARD __forceinline constexpr const auto& RetrieveBy(Criterias&&... criterias)
		{
			return StoragePolicy<File, SequenceContainer, Args...>::RetrieveBy(std::forward<Criterias>(criterias)...);
		}
		_NODISCARD __forceinline constexpr const auto Delete(File const& FilePath) noexcept
		{
			static_assert(traits::is_file_name<typename std::remove_const<typename std::remove_reference<decltype(FilePath)>::type>::type>::value, "File doesn't meet file name concept");
			return StoragePolicy<File, SequenceContainer, Args...>::Delete(FilePath);
		}
		_NODISCARD __forceinline constexpr const auto Files(SequenceContainer<File, Args...>& container) const noexcept
		{
			static_assert(traits::is_file_name<typename std::remove_const<typename std::remove_reference<File>::type>::type>::value, "File doesn't meet file name concept");
			return StoragePolicy<File, SequenceContainer, Args...>::Files(container);
		}
		_NODISCARD __forceinline constexpr const auto& Files() const noexcept
		{
			static_assert(traits::is_file_name<typename std::remove_const<typename std::remove_reference<File>::type>::type>::value, "File doesn't meet file name concept");
			return StoragePolicy<File, SequenceContainer, Args...>::Files();
		}
	};
}