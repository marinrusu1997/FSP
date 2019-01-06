#ifndef FSP_MESSAGE_HEADER
#define FSP_MESSAGE_HEADER

#include <map>
#include <set>
#include <optional>
#include <memory>
#include <string>
#include <string_view>

#include <boost/algorithm/string.hpp>
#include <json.hpp>

#include "protocol.h"
#include "fsp_headers.h"
#include "fsp_content_type_formats.h"

#define APPEND_CMD(cmd) .append((cmd)).append((fsp::protocol::LINE_SEPARATOR))
#define APPEND_HDR(name,value) \
				.append((name)) \
				.append((fsp::protocol::HEADER_SEPARATOR)) \
				.append((value)) \
				.append((fsp::protocol::LINE_SEPARATOR))
#define APPEND_EOH .append((fsp::protocol::EOHEADER))

#define SET_CMD(msgstr,cmd)			(msgstr)APPEND_CMD((cmd))
#define SET_HDR(msgstr,name,value)	(msgstr)APPEND_HDR((name),(value))
#define SET_EOH(msgstr)				(msgstr)APPEND_EOH


namespace fsp::protocol::message 
{
	constexpr static const char * const NEW_LINE_STR		= "\n";

	struct message final : std::enable_shared_from_this<message>
	{
		message() = default;
		message(message const&) = delete;
		message& operator=(message const&) = delete;

		std::string&					body() noexcept { return body_; }
		std::string const&				body() const noexcept { return body_; }

		bool							have_header(std::string_view header) const { return headers_.find(header.data()) != headers_.end(); }
		auto&							operator[](std::string_view name) noexcept { return headers_[name.data()]; }

		auto&							command() noexcept { return this->command_; }
		auto&							command() const noexcept { return this->command_; }
		void							command(std::string_view cmd) noexcept { this->command_ = cmd; }
		
		const _STD optional<size_t>		body_size() const noexcept
		{
			if (auto&& iter = headers_.find(headers::ContentLength); iter != headers_.end())
			{
				try
				{
					return { std::stoi((*iter).second) };
				}
				catch (...) {
					return {};
				}
			}
			return { 0 };
		}

		void							reset_header()
		{
			command_.clear();
			headers_.clear();
		}

		void							clear()
		{
			command_.clear();
			headers_.clear();
			body_.clear();
		}

		auto							to_string() const
		{
			return header_to_string().append(body_);
		}

		template<typename ExpectedType>
		void							body(ExpectedType& expected_type) noexcept
		{
			try {
				if (body_.empty())
					return;

				if (headers_[fsp::protocol::headers::ContentType] == fsp::protocol::content_type_formats::JSON)
					nlohmann::json().parse(body_).get_to(expected_type);

				if (headers_[fsp::protocol::headers::ContentType] == fsp::protocol::content_type_formats::CSV_LIST)
					boost::split(expected_type, body_, [](auto c) {return c == ','; });
			}
			catch (...) {
			}
		}

		template<typename SequentialContainer>
		void							body(SequentialContainer& container, content_type_formats::content_type_format format) noexcept
		{
			try {
				if (container.empty())
					return;

				switch (format)
				{
				case fsp::protocol::content_type_formats::content_type_format::JSON:
				{
					body_ = nlohmann::json(container).dump();
					headers_[fsp::protocol::headers::ContentType] = fsp::protocol::content_type_formats::JSON;
					headers_[fsp::protocol::headers::ContentLength] = std::to_string(body_.size());
				}
				break;
				case fsp::protocol::content_type_formats::content_type_format::CSV_LIST:
				{
					for (const auto & v : container)
						body_.append(v).append(",");
					body_.pop_back();
					headers_[fsp::protocol::headers::ContentType] = fsp::protocol::content_type_formats::CSV_LIST;
					headers_[fsp::protocol::headers::ContentLength] = std::to_string(body_.size());
				}
				break;
				}
			}
			catch (...) {
			}
		}

		void							map_to_body(_STD map<_STD string, const _STD set<_STD string>*> const& users_files);

		void							body_to_map(_STD map<_STD string, _STD set<_STD string>>& users_files);
	private:
		_STD string						header_to_string() const;

		std::string							command_;
		std::map<std::string, std::string>	headers_;
		std::string							body_;
	};

	using message_ptr = std::unique_ptr<message>;

	using request = message;
	using request_ptr = message_ptr;

	using reply = message;
	using reply_ptr = message_ptr;

	namespace builders {
		struct StringMessageBuilder final
		{
			StringMessageBuilder&	SetCommand(const _STD string_view Command)
			{
				message_.append(Command).append(fsp::protocol::LINE_SEPARATOR);
				return *this;
			}
			StringMessageBuilder&	SetHeader(const _STD string_view Header, const _STD string_view Value)
			{
				message_
					.append(Header)
					.append(fsp::protocol::HEADER_SEPARATOR)
					.append(Value)
					.append(fsp::protocol::LINE_SEPARATOR);
				return *this;
			}
			StringMessageBuilder&	SetEndOfProtocolHeader()
			{
				message_.append(fsp::protocol::EOHEADER);
				return *this;
			}
			StringMessageBuilder&	SetBody(const _STD string_view Body)
			{
				message_.append(Body);
				return *this;
			}
			const _STD string&		Build() const noexcept
			{
				return message_;
			}
			_STD string&			Build() noexcept
			{
				return message_;
			}
		private:
			_STD string message_;
		};

		struct StringGenericMessage
		{		
			StringGenericMessage(StringGenericMessage const&) = delete;
			StringGenericMessage& operator=(StringGenericMessage const&) = delete;

			StringGenericMessage&		SetAditionalHeaders(const _STD string_view Header, const _STD string_view Value);
			StringGenericMessage&		SetEndOfProtocolHeader();
			StringGenericMessage&		SetBody(const _STD string_view Body);
			const _STD string&			build() const noexcept;
			_STD string&				build() noexcept;
		protected:
			StringGenericMessage() = default;

			_STD string message_;
		};

		struct StringReplyMessage final : StringGenericMessage
		{
			StringReplyMessage();
			StringReplyMessage&			SetReplyCode(const _STD string_view ReplyCode);
			StringReplyMessage&			SetRequestID(const _STD string_view RequestID);
		};

		struct StringNotificationMessage final : StringGenericMessage
		{
			StringNotificationMessage();
			StringNotificationMessage&	SetNotificationType(const _STD string_view NotificationType);
		};

		struct StringQuerryMessage final : StringGenericMessage
		{
			StringQuerryMessage();
			StringQuerryMessage&		SetRequestID(const _STD string_view RequestID);
			StringQuerryMessage&		SetQuerryType(const _STD string_view QuerryType);
		};

		template<typename SequentialContainer>
		_STD string	build_body(SequentialContainer const& container, content_type_formats::content_type_format format) noexcept
		{
			_STD string body;
			try {
				if (container.empty())
					return body;

				switch (format)
				{
				case fsp::protocol::content_type_formats::content_type_format::JSON:
				{
					body = nlohmann::json(container).dump();
				}
				break;
				case fsp::protocol::content_type_formats::content_type_format::CSV_LIST:
				{
					for (const auto & v : container)
						body.append(v).append(CSV_DELIMITER_STR);
					body.pop_back();
				}
				break;
				}
				
				return body;
			}
			catch (...) {
				return body;
			}
		}

		template <class StringType,
			template<class, class...> class AssociativeContainer,
			template<class, class...> class SequentialContainer,
			class... AssocArgs,
			class... SeqArgs>
		StringType	build_body(AssociativeContainer<StringType, SequentialContainer<StringType, SeqArgs...>, AssocArgs...> const& FileSystemsOfUsers)
		{
			using namespace fsp::protocol::headers;
			using namespace fsp::protocol::content_type_formats;
			StringType Body;

			if (FileSystemsOfUsers.empty())
				return Body;

			for (const auto&[User, Paths] : FileSystemsOfUsers)
			{
				Body.append(User);
				for (const auto& Path : Paths)
					Body.append(CSV_DELIMITER_STR).append(Path);
				Body.append(NEW_LINE_STR);
			}

			return Body;
		}

		const _STD string	build_body(_STD map<_STD string, const _STD set<_STD string>*> const& FilesOfUsers, content_type_formats::content_type_format format = content_type_formats::content_type_format::CSV_MAP);
	}
}

#endif