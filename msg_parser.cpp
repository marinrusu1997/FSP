#include "msg_parser.h"
#include "fsp_commands.h"
#include <cctype>

namespace fsp::protocol::message::parsers {

	message_parser::header_parser::header_parser() noexcept
		: state_(state::command_start) 
	{}

	void message_parser::header_parser::reset() noexcept
	{
		this->state_ = state::command_start;
		this->header_name.clear();
		this->header_value.clear();
	}

	message_parser::result_type message_parser::header_parser::consume(fsp::protocol::message::message& req, char input) noexcept
	{ 
		if (input < -1) //control test before for passing this to std::isupper, ischar and so on, they will fail with assert if input is not in range c >= -1 && c <= 255
			return bad;
		switch (state_)
		{
		case message_parser::header_parser::state::command_start:
			if (!std::isupper(input)) //every command begins with uppercase letter
				return bad;
			else
			{
				state_ = state::command; //go to state wehere command is read
				req.command().push_back(input); //add first letter to string
				return indeterminate; //need more input chars
			}
		case message_parser::header_parser::state::command:
			if (input == '\r') // this means that header is incoming or request is finished
			{
				if (!protocol::commands::IsCommandValid(req.command()))
					return bad; //command is not valid
				state_ = state::new_line;
				return indeterminate;
			}
			else if (!std::isupper(input))
				return bad; //All command is un upper case
			else {
				req.command().push_back(input);
				return indeterminate;
			}
		case message_parser::header_parser::state::new_line:
			if (input != '\n')
			{
				return bad;
			}
			else {
				state_ = state::transition;
				return indeterminate;
			}
		case message_parser::header_parser::state::transition:
			if (input == '\r')
			{
				state_ = state::end;
				return indeterminate;
			}
			else if (std::isupper(input)) {
				state_ = state::header_name;
				header_name.push_back(input);
				return indeterminate;
			}
			else
				return bad;
		case message_parser::header_parser::state::header_name:
			if (input == ':')
			{
				if (!protocol::headers::IsHeaderValid(header_name))
					return bad;
				state_ = state::expecting_value;
				return indeterminate;
			}
			else if (std::islower(input) || std::isupper(input) || input == '-') {
				header_name.push_back(input);
				return indeterminate;
			}
			else
				return bad;
		case message_parser::header_parser::state::expecting_value:
			//if (std::isupper(input) || std::islower(input) || std::isdigit(input) || input == '/')
			//{
				header_value.push_back(input);
				state_ = state::header_value;
				return indeterminate;
			//}
			//else {
			//	return bad;
			//}
		case message_parser::header_parser::state::header_value:
			if (input == '\r')
			{
				req[header_name] = header_value;
				header_name.clear();
				header_value.clear();
				state_ = state::new_line;
				return indeterminate;
			}
			else /*if (std::isdigit(input) || std::islower(input) || input == '/')*/
			{
				header_value.push_back(input);
				return indeterminate;
			}
			//else
			//	return bad;
		case message_parser::header_parser::state::end:
			return (input == '\n') ? good : bad;
		default:
			return bad;
		}
	}

	bool message_parser::header_parser::is_char(int c)
	{
		return c >= 0 && c <= 127;
	}

	bool message_parser::header_parser::is_ctl(int c)
	{
		return (c >= 0 && c <= 31) || (c == 127);
	}

	bool message_parser::header_parser::is_tspecial(int c)
	{
		switch (c)
		{
		case '(': case ')': case '<': case '>': case '@':
		case ',': case ';': case ':': case '\\': case '"':
		case '/': case '[': case ']': case '?': case '=':
		case '{': case '}': case ' ': case '\t':
			return true;
		default:
			return false;
		}
	}

	bool message_parser::header_parser::is_digit(int c)
	{
		return c >= '0' && c <= '9';
	}

	message_parser::message_parser() noexcept : header_parser_(), expecting_body(false), body_size(0)
	{}

	void message_parser::reset() noexcept
	{
		header_parser_.reset();
		expecting_body = false;
		body_size = 0;
	}
}