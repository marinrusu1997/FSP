#pragma once
#include <set>
#include <string_view>

namespace fsp::protocol::headers::notifications
{
	constexpr static const char * const USER_LOGGED_IN					= "usrin";
	constexpr static const char * const USER_LOGGED_OUT					= "usrot";
	constexpr static const char * const USER_REGISTERED_FILES			= "regfl";
	constexpr static const char * const FILES_OF_OTHER_USERS			= "flusr";
	constexpr static const char * const USER_ADDED_NEW_FILE				= "addfl";
	constexpr static const char * const USER_DELETED_FILE				= "rmvfl";
	constexpr static const char * const USER_RENAMED_FILE				= "rnmfl";
	constexpr static const char * const USER_REQUESTED_FILE_DOWNLOAD	= "dldfl";

	static const _STD set<_STD string_view, _STD less<>> notifications
	{
		USER_LOGGED_IN,
		USER_LOGGED_OUT,
		USER_REGISTERED_FILES,
		FILES_OF_OTHER_USERS,
		USER_ADDED_NEW_FILE,
		USER_DELETED_FILE,
		USER_RENAMED_FILE,
		USER_REQUESTED_FILE_DOWNLOAD
	};

	_NODISCARD inline const bool IsNotificationTypeValid(_STD string_view notification) noexcept
	{
		return notifications.find(notification) != notifications.end();
	}
}