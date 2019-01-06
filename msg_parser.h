#pragma once
#include <tuple>
#include <cctype>
#include "message.h"

namespace fsp::protocol::message::parsers {

	class message_parser final
	{
	public:
		/// Construct ready to parse message parser which parses header and appends body
		message_parser() noexcept;

		message_parser(message_parser const&) = delete;
		message_parser& operator=(const message_parser&) = delete;

		/// Result of parse.
		enum result_type { good, bad, indeterminate };

		/// Parse some data. The enum return value is good when a complete request has
		/// been parsed, bad if the data is invalid, indeterminate when more data is
		/// required. The InputIterator return value indicates how much of the input
		/// has been consumed.
		template <typename InputIterator>
		std::tuple<result_type, InputIterator> parse(fsp::protocol::message::message& req,
			InputIterator begin, InputIterator end) noexcept
		{
			if (!expecting_body) // we still have to parse header
			{
				auto result = header_parser_.parse(req, begin, end);
				switch (result)
				{
				case indeterminate:
					return std::make_tuple(indeterminate, begin);
				case bad: {
					this->reset();
					return std::make_tuple(bad, begin);
				}
				case good: {
					auto optional_body_size = req.body_size();
					if (!optional_body_size.has_value()) { //bad format Content-Length header
						this->reset();
						return std::make_tuple(bad, begin);
					}
					if ((this->body_size = optional_body_size.value()) == 0) { //done processing message
						this->reset();
						return std::make_tuple(good, begin);
					}
					if (this->body_size > 0) {
						if (begin == end) { //there is no more data available in this range
							expecting_body = true;
							return std::make_tuple(indeterminate, begin);
						}
						if (begin != end) { //we still have some available data in this range that we can consume
							size_t available = end - begin;
							if (available >= this->body_size) { //we have enough data
								req.body().append(begin, begin + this->body_size);
								begin += this->body_size;
								this->reset();
								return std::make_tuple(good, begin);
							}
							else { 
								expecting_body = true; // we dont have enough data, so we expect more
								req.body().append(begin, end); //append all data that we have so far
								this->body_size -= available; //update requred body size
								return std::make_tuple(indeterminate, end); //we need more data
							}
						}
					}
				}
				default: {
					this->reset();
					return std::make_tuple(bad, begin);
				}
				}
			}
			else { //if we are there we need more data to complete body
				size_t bytes_received = end - begin;
				if (body_size == bytes_received) { // we got all required data
					req.body().append(begin, end); //append all data
					this->reset();
					return std::make_tuple(good, end); //we consumed all the bytes till the end iterator, we are done
				}
				else if ((int)(body_size - bytes_received) > 0) { // we got fewer bytes than expected
					body_size -= bytes_received; // update new body_size bytes expected
					req.body().append(begin, end); //append all data
					return std::make_tuple(indeterminate, end); // we need more data
				}
				else if ((int)(body_size - bytes_received) < 0) { //we got too many bytes
					req.body().append(begin, begin + body_size); //append only needed amount of bytes
					begin += body_size;
					this->reset();
					return std::make_tuple(good, begin );
				}
			}
		}
	private:
		/// Helper class for parsing header 
		class header_parser
		{
		public:
			/// Construct ready to parse the request header method.
			header_parser() noexcept;

			/// Reset to initial parser state.
			void reset() noexcept;

			/// Parse some data. The enum return value is good when a complete request has
			/// been parsed, bad if the data is invalid, indeterminate when more data is
			/// required. The InputIterator return value indicates how much of the input
			/// has been consumed.
			template <typename InputIterator>
			result_type parse(fsp::protocol::message::message& req, InputIterator& begin, InputIterator& end) noexcept
			{
				while (begin != end) // loop through bytes received
				{
					result_type result = consume(req, *begin++); //consume one byte and advance iterator
					if (result == result_type::bad || result == result_type::good)
						return result;
				}
				return indeterminate; // we need more data to parse header
			}

		private:
			/// Handle the next character of input.
			result_type consume(fsp::protocol::message::message& req, char input) noexcept;

			/// Check if a byte is an HTTP character.
			static bool is_char(int c);

			/// Check if a byte is an HTTP control character.
			static bool is_ctl(int c);

			/// Check if a byte is defined as an HTTP tspecial character.
			static bool is_tspecial(int c);

			/// Check if a byte is a digit.
			static bool is_digit(int c);

			/// The current state of the parser.
			enum class state : int8_t
			{
				command_start,
				command,
				new_line,
				transition,
				header_name,
				expecting_value,
				header_value,
				end
			};

			state state_;

			/// Holds intermediate header name
			std::string header_name;

			/// Holds intermediate header value
			std::string header_value;
		}header_parser_;

		/// Reset to initial state
		void reset() noexcept;

		/// We have to append and body?
		bool	expecting_body;

		/// Size of the body
		size_t  body_size;
	};
}
