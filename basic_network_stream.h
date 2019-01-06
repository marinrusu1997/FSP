#pragma once
#include <boost/asio/strand.hpp>
#include <boost/asio/ip/tcp.hpp>

#include "basic_network_stream_concepts.h"
#include "mutable_buffer.h"
#include "msg_parser.h"
#include "fsp_responses.h"
#include "protocol.h"

template <typename Derived>
class basic_network_stream 
{
public:
	explicit basic_network_stream(boost::asio::ip::tcp::socket&& stream) :
		stream_(std::move(stream)),
#ifdef CALLER_LIFETIME_SECURE_WEAK_MULTITHREADED_CONTEXT
		strand_(stream_.get_io_service()),
#endif // CALLER_LIFETIME_SECURE
		read_buffer_(),
		parser_(),
		message_(std::move(std::make_unique<fsp::protocol::message::message>()))
	{}

	//template<class BufferType>
	void start_write(const std::string_view buffer)
	{
		static_assert (implements_write<Derived>::value, "Derived class doesnt implement write concept");
		if (static_cast<Derived*>(this)->write_guard()) {
			auto buff_ptr = std::make_unique<std::string>(buffer);
			async_send(std::move(buff_ptr), 0);
		}
	}

	void start_read()
	{
		static_assert (implements_read<Derived>::value, "Derived class doesnt implement read concept");
		if (static_cast<Derived*>(this)->read_guard())
			async_read();
	}

	auto& get_io_context() { return stream_.get_io_context(); }

protected:
	boost::asio::ip::tcp::socket					stream_;
#ifdef CALLER_LIFETIME_SECURE_WEAK_MULTITHREADED_CONTEXT
	boost::asio::io_context::strand					strand_;
#endif // CALLER_LIFETIME_SECURE
	mutable_buffer<char, static_cast<size_t>(5120)>	read_buffer_;
	fsp::protocol::message::parsers::message_parser parser_;
	fsp::protocol::message::message_ptr				message_;

	void async_send(
#ifdef CALLER_LIFETIME_SECURE_WEAK_MULTITHREADED_CONTEXT
		std::shared_ptr<std::string> buff_ptr,
#else
		std::unique_ptr<std::string> buff_ptr,
#endif
		const size_t offset)
	{

		assert(offset < buff_ptr->size());
		const auto data = buff_ptr->data() + offset;
		const auto size = buff_ptr->size() - offset;

		stream_.async_send(
			boost::asio::buffer(data, size),
#ifdef CALLER_LIFETIME_SECURE_WEAK_MULTITHREADED_CONTEXT
		strand_.wrap(
#endif
				[this, buff_ptr = std::move(buff_ptr), current_offset = offset
#ifdef CALLER_LIFETIME_SECURE_WEAK
				, self_weak = static_cast<Derived*>(this)->weak_from_this()
#endif // CALLER_LIFETIME_SECURE
#ifdef CALLER_LIFETIME_SECURE_STRONG
				, self_strong = static_cast<Derived*>(this)->shared_from_this()
#endif // CALLER_LIFETIME_SECURE_STRONG
				]
		(auto ec, auto bytes_transferred)
		{
#ifdef CALLER_LIFETIME_SECURE_WEAK
			if (!self_weak.expired())
			{
#endif // CALLER_LIFETIME_SECURE

				if (!ec)
				{
					/// Check if all data was stored in TCP internall buffer
					if (*const_cast<size_t*>(&current_offset) += bytes_transferred; current_offset != buff_ptr->size())
					{
						this->async_send(
#ifdef CALLER_LIFETIME_SECURE_WEAK_MULTITHREADED_CONTEXT
							std::move(*const_cast<std::shared_ptr<std::string>*>(&buff_ptr)),
#else
							std::move(*const_cast<std::unique_ptr<std::string>*>(&buff_ptr)),
#endif
							current_offset
						);
					}
					else {
						/// Call on post write handler
						static_cast<Derived*>(this)->write_action();
					}
				}
				else /// STUB TO DO PROPER ERROR HANDLING
				{
					static_cast<Derived*>(this)->write_error(ec, *const_cast<std::unique_ptr<std::string>*>(&buff_ptr));
				}
#ifdef CALLER_LIFETIME_SECURE_WEAK
			}
#endif // CALLER_LIFETIME_SECURE
		}
#ifdef CALLER_LIFETIME_SECURE_WEAK_MULTITHREADED_CONTEXT
		)
#endif
		);
	}

	void async_read()
	{
		namespace prsr = fsp::protocol::message::parsers;

		stream_.async_read_some(
			boost::asio::buffer(read_buffer_.buffer()),
#ifdef CALLER_LIFETIME_SECURE_WEAK_MULTITHREADED_CONTEXT
			strand_.wrap(
#endif
			[this
#ifdef CALLER_LIFETIME_SECURE_WEAK
			, self_weak = static_cast<Derived*>(this)->weak_from_this()
#endif // CALLER_LIFETIME_SECURE
#ifdef CALLER_LIFETIME_SECURE_STRONG
			, self = static_cast<Derived*>(this)->shared_from_this()
#endif // CALLER_LIFETIME_SECURE
			]
		(auto ec, auto bytes_transferred)
		{
#ifdef CALLER_LIFETIME_SECURE_WEAK
			if (!self_weak.expired())
			{
#endif // CALLER_LIFETIME_SECURE

			if (!ec)
			{
				static_cast<Derived*>(this)->read_prepare();

				read_buffer_.current_size(bytes_transferred);

				while (read_buffer_.can_read())
				{
					auto&&[result, iter] = parser_.parse(*message_, read_buffer_.begin_read_iter(), read_buffer_.end_read_iter());

					read_buffer_.consume(iter);

					if (result == prsr::message_parser::good) {
						static_cast<Derived*>(this)->read_action();
						message_.reset(new fsp::protocol::message::message());
					}
					else if (result == prsr::message_parser::bad) {
						start_write(fsp::protocol::message::builders::basic_reply
						(fsp::protocol::responses::SYNTACTIC_ERROR, fsp::protocol::CMD_ID_WHEN_NO_CMDID_PRESENT).data());
						message_->reset_header();
					}
					else if (result == prsr::message_parser::indeterminate) {
						static_cast<Derived*>(this)->read_indeterminate();
						async_read();
						return;
					}
				}

				static_cast<Derived*>(this)->read_done();
			}
			else
			{
				static_cast<Derived*>(this)->read_error(ec);
			}
#ifdef CALLER_LIFETIME_SECURE_WEAK
			}
#endif // CALLER_LIFETIME_SECURE
		}
#ifdef CALLER_LIFETIME_SECURE_WEAK_MULTITHREADED_CONTEXT
		)
#endif
	);
	}
};
