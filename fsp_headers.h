#pragma once
#include <string_view>
#include <set>

namespace fsp::protocol::headers {

	constexpr static const char * const Nickname				= "Nickname";
	constexpr static const char * const Password				= "Password";

	constexpr static const char * const ReplyCode				= "Reply-Code";
	constexpr static const char * const CommandId				= "Command-Id";

	constexpr static const char * const QuerryType				= "Querry-Type";
	constexpr static const char * const NotificationType		= "Notif-Type";

	constexpr static const char * const ContentLength			= "Content-Length";
	constexpr static const char * const ContentType				= "Content-Type";
	
	constexpr static const char * const FileName				= "File-Name";
	constexpr static const char * const FileNewName				= "File-NewName";
	constexpr static const char * const FileType				= "File-Type";
	constexpr static const char * const FileOwner				= "File-Owner";
	constexpr static const char * const FileSize				= "File-Size";

	constexpr static const char * const SslVersion				= "Ssl-Version";
	constexpr static const char * const Compressions			= "Compressions";

	constexpr static const char * const	DownloadTransactionID	= "Transaction-ID";
	constexpr static const char * const DownloadPort			= "Dwnld-Port";

	static const std::set<std::string_view> headers{
		Nickname,
		Password,

		ReplyCode,
		CommandId,

		QuerryType,
		NotificationType,

		ContentLength,
		ContentType,
		
		FileName,
		FileNewName,
		FileType,
		FileOwner,
		FileSize,

		SslVersion,
		Compressions,
		DownloadTransactionID,
		DownloadPort
	};

	_NODISCARD inline const bool IsHeaderValid(const _STD string_view Header) noexcept
	{
		return headers.find(Header) != headers.end();
	}
}