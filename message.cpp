#include "message.h"
#include "fsp_commands.h"
#include <sstream>

using namespace fsp::protocol::message;


_STD string						message::header_to_string() const
{
	_STD string request_;
	SET_CMD(request_, this->command_);
	for (auto&[name, value] : this->headers_) {
		SET_HDR(request_, name, value);
	}
	SET_EOH(request_);
	return request_;
}

void							message::map_to_body(_STD map<_STD string, const _STD set<_STD string>*> const& users_files)
{
	using namespace fsp::protocol::headers;
	using namespace fsp::protocol::content_type_formats;

	if (users_files.empty())
		return;

	for (const auto&[user, files] : users_files)
	{
		body_.append(user);
		for (const auto& file : *files)
			body_.append(CSV_DELIMITER_STR).append(file);
		body_.append(NEW_LINE_STR);
	}

	headers_[ContentType]	= CSV_MAP;
	headers_[ContentLength] = _STD to_string(body_.size());
}

const _STD string				builders::build_body(_STD map<_STD string, const _STD set<_STD string>*> const& FilesOfUsers, fsp::protocol::content_type_formats::content_type_format Format)
{
	using namespace fsp::protocol::headers;
	using namespace fsp::protocol::content_type_formats;
	_STD string Body;

	if (Format != content_type_format::CSV_MAP)
		throw std::exception("Content type format for build body with map of users files is not implemented");

	if (FilesOfUsers.empty())
		return Body;

	for (const auto&[User, Files] : FilesOfUsers)
	{
		Body.append(User);
		for (const auto& file : *Files)
			Body.append(CSV_DELIMITER_STR).append(file);
		Body.append(NEW_LINE_STR);
	}

	return Body;
}

void							message::body_to_map(_STD map<_STD string, _STD set<_STD string>>& users_files)
{
	if (body_.empty())
		return;

	constexpr const uint8_t USERNAME_INDEX		= 0;
	constexpr const uint8_t TOKENS_MINIM_NUMBER = 1;

	std::istringstream iss(body_);
	std::string line;
	std::vector<std::string> results;
	while (std::getline(iss, line))
	{
		boost::split(results, line, [](char c) {return c == CSV_DELIMITER_CHR; });

		assert(results.size() >= TOKENS_MINIM_NUMBER);

		auto & set = users_files[std::move(results[USERNAME_INDEX])];

		for (size_t i = TOKENS_MINIM_NUMBER; i < results.size(); i++)
			set.emplace(std::move(results[i]));

		results.clear();
	}
}


builders::StringGenericMessage& builders::StringGenericMessage::SetAditionalHeaders(const _STD string_view Header, const _STD string_view Value)
{
	message_
		.append(Header)
		.append(fsp::protocol::HEADER_SEPARATOR)
		.append(Value)
		.append(fsp::protocol::LINE_SEPARATOR);
	return *this;
}

builders::StringGenericMessage&	builders::StringGenericMessage::SetEndOfProtocolHeader()
{
	message_.append(fsp::protocol::EOHEADER);
	return *this;
}

builders::StringGenericMessage&	builders::StringGenericMessage::SetBody(const _STD string_view Body)
{
	message_.append(Body);
	return *this;
}

const _STD string&				builders::StringGenericMessage::build() const noexcept
{
	return message_;
}

_STD string&					builders::StringGenericMessage::build() noexcept
{
	return message_;
}


builders::StringReplyMessage::StringReplyMessage()
{
	message_.append(fsp::protocol::commands::REPLY).append(fsp::protocol::LINE_SEPARATOR);
}

builders::StringReplyMessage&	builders::StringReplyMessage::SetReplyCode(const _STD string_view ReplyCode)
{
	message_
		.append(fsp::protocol::headers::ReplyCode)
		.append(fsp::protocol::HEADER_SEPARATOR)
		.append(ReplyCode)
		.append(fsp::protocol::LINE_SEPARATOR);
	return *this;
}

builders::StringReplyMessage&	builders::StringReplyMessage::SetRequestID(const _STD string_view RequestID)
{
	message_
		.append(fsp::protocol::headers::CommandId)
		.append(fsp::protocol::HEADER_SEPARATOR)
		.append(RequestID)
		.append(fsp::protocol::LINE_SEPARATOR);
	return *this;
}


builders::StringNotificationMessage::StringNotificationMessage()
{
	message_.append(fsp::protocol::commands::NOTIFICATION).append(fsp::protocol::LINE_SEPARATOR);
}

builders::StringNotificationMessage&	builders::StringNotificationMessage::SetNotificationType(const _STD string_view NotificationType)
{
	message_
		.append(fsp::protocol::headers::NotificationType)
		.append(fsp::protocol::HEADER_SEPARATOR)
		.append(NotificationType)
		.append(fsp::protocol::LINE_SEPARATOR);
	return *this;
}


builders::StringQuerryMessage::StringQuerryMessage()
{
	message_.append(fsp::protocol::commands::QUERRY).append(fsp::protocol::LINE_SEPARATOR);
}

builders::StringQuerryMessage&	builders::StringQuerryMessage::SetRequestID(const _STD string_view RequestID)
{
	message_
		.append(fsp::protocol::headers::CommandId)
		.append(fsp::protocol::HEADER_SEPARATOR)
		.append(RequestID)
		.append(fsp::protocol::LINE_SEPARATOR);
	return *this;
}

builders::StringQuerryMessage&	builders::StringQuerryMessage::SetQuerryType(const _STD string_view QuerryType)
{
	message_
		.append(fsp::protocol::headers::QuerryType)
		.append(fsp::protocol::HEADER_SEPARATOR)
		.append(QuerryType)
		.append(fsp::protocol::LINE_SEPARATOR);
	return *this;
}