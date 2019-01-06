#pragma once
#include <string_view>
#include <set>

namespace fsp::protocol::commands {
	// commands sent by client in any state
	constexpr static const char * const HEARTBEAT							= "HRTBT";
	// commands sent by client in connected state
	constexpr static const char * const SIGNIN								= "SIGIN";
	constexpr static const char * const SIGNUP								= "SIGUP";
	// commands sent by client in logged state
	constexpr static const char * const DELETE_ACCOUNT						= "DELAC";
	constexpr static const char * const LOGOUT								= "LGOUT";
	constexpr static const char * const PUSH_FILES							= "PUSHF";
	// commands sent usually by client in processing state
	constexpr static const char * const QUERRY								= "QUERY";

	// commands sent by server
	constexpr static const char * const REPLY								= "REPLY";
	constexpr static const char * const NOTIFICATION						= "NOTIF";

	static const std::map<_STD string_view, _STD string_view, _STD less<> > commands{
		{HEARTBEAT, "HEARTBEAT"},
		{SIGNIN, "SIGN IN"},
		{SIGNUP, "SIGN UP"},

		{DELETE_ACCOUNT, "DELETE ACCOUNT"},
		{LOGOUT, "LOGOUT"},
		{PUSH_FILES, "PUSH FILES"},
		
		{QUERRY, "QUERRY"},

		{REPLY, "REPLY TO COMMAND"},
		{NOTIFICATION,"NOTIFICATION THAT SOMETHING HAPPENED IN THE SYSTEM"}
	};

	_NODISCARD inline const bool IsCommandValid(const _STD string_view Command) noexcept
	{
		return commands.find(Command) != commands.end();
	};
}