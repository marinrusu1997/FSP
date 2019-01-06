#pragma once

#include <string_view>
#include <map>

/*
	Response type: xyz => every response is unique
		values for x:
			1 -> Positive Preliminary reply
			2 -> Positive Completion reply
			3 -> Transient Negative Completion reply
			4 -> Permanent Negative Completion reply
			5 -> Headers Format
		values for y:
			0 -> Control and Data connections
			1 -> Authentication and Accounting
			2 -> Files
			3 -> Body Format
		values for z:
			arbitrary
	Special responses:
		000 => Syntactic errors
		001 => Internal server error
*/
namespace fsp::protocol::responses {
	constexpr static const char * const SYNTACTIC_ERROR							= "000";
	constexpr static const char * const INTERNAL_SERVER_ERR						= "001";

	constexpr static const char * const SIGNIN_CMD_ACCEPTED						= "110";
	constexpr static const char * const SIGNUP_CMD_ACCEPTED						= "111";
	constexpr static const char * const DELETE_ACCOUNT_CMD_ACCEPTED				= "112";
	constexpr static const char * const LOGOUT_CMD_ACCEPTED						= "113";

	constexpr static const char * const PUSH_FILES_CMD_ACCEPTED					= "120";
	constexpr static const char * const QUERRY_CMD_ACCEPTED						= "121";
	constexpr static const char * const DOWNLOAD_FILE_QUERRY_APPROVED			= "122";

	constexpr static const char * const LOGIN_SUCCESSFULL						= "210";
	constexpr static const char * const REGISTERING_SUCCESSFULL					= "211";
	constexpr static const char * const DELETING_ACCOUNT_SUCCESSFULL			= "212";

	constexpr static const char * const PUSH_FILES_SUCCESS						= "220";
	constexpr static const char * const ADD_FILE_SUCCESSFULL					= "221";
	constexpr static const char * const REMOVE_FILE_SUCCESSFULL					= "222";
	constexpr static const char * const RENAME_FILE_SUCCESSFULL					= "223";

	constexpr static const char * const INVALID_CMD_WHILE_IN_CONN_STATE			= "300";
	constexpr static const char * const INVALID_CMD_WHILE_IN_LOGGED_STATE		= "301";
	constexpr static const char * const INVALID_CMD_WHILE_IN_PROCESSING_STATE	= "302";

	constexpr static const char * const NICKNAME_INCORRECT						= "310";
	constexpr static const char * const PASSWORD_INCORRECT						= "311";

	constexpr static const char * const REMOVE_FILE_FAILED						= "320";
	constexpr static const char * const RENAME_FILE_FAILED						= "321";

	constexpr static const char * const BAD_CMD_IN_REPLY						= "400";
	constexpr static const char * const BAD_NOTIFICATION_TYPE_MESSAGE_FORMAT	= "401";
	constexpr static const char * const INVALID_SSL_VERSION						= "402";
	constexpr static const char * const SSL_VERSION_INCOMPATIBLE				= "403";

	constexpr static const char * const REMOVE_ACCOUNT_FAILED					= "410";
	constexpr static const char * const USER_ALREADY_LOGGED						= "411";
	constexpr static const char * const USER_ALREADY_REGISTERED					= "412";

	constexpr static const char * const DOWNLOAD_DENY							= "420";

	constexpr static const char * const CONTENT_TYPE_UNSUPPORTED				= "430";

	constexpr static const char * const CMDID_HDR_REQUIRED						= "500";
	constexpr static const char * const RPL_CODE_HDR_REQUIRED					= "501";
	constexpr static const char * const QUERRY_TYPE_HDR_REQUIRED				= "502";
	constexpr static const char * const NOTIFICATION_TYPE_HDR_REQUIRED			= "503";
	constexpr static const char * const SSL_VERSION_HDR_REQUIRED				= "504";

	constexpr static const char * const NICKNAME_HDR_REQUIRED					= "510";
	constexpr static const char * const PASSWORD_HDR_REQUIRED					= "511";

	constexpr static const char * const FILE_NAME_HDR_REQUIRED					= "520";
	constexpr static const char * const FILE_NEW_NAME_HDR_REQUIRED				= "521";
	constexpr static const char * const FILE_TYPE_HDR_REQUIRED					= "522";
	constexpr static const char * const FILE_OWNER_HDR_REQUIRED					= "523";
	constexpr static const char * const FILE_SIZE_HDR_REQUIRED					= "524";

	constexpr static const char * const CONTENT_TYPE_HDR_REQUIRED = "530";


	static const std::map<std::string_view, std::string_view> responses{
		{SYNTACTIC_ERROR , "Header is not valid, having syntactic errors"},
		{INTERNAL_SERVER_ERR,"Internal server error, try later"},

		{SIGNIN_CMD_ACCEPTED, "Login command was accepted by server, processing..."},
		{SIGNUP_CMD_ACCEPTED, "Register command was accepted by server, processing..."},
		{DELETE_ACCOUNT_CMD_ACCEPTED, "Delete account command was accepted by server, processing..."},
		{LOGOUT_CMD_ACCEPTED, "Logout command was accepted by server, clossing connection..."},

		{PUSH_FILES_CMD_ACCEPTED, "Push Files command was accepted by server, processing..."},
		{QUERRY_CMD_ACCEPTED, "Querry command accepted, processing..."},
		{DOWNLOAD_FILE_QUERRY_APPROVED, "Querry for file downloading from file owner approved, waiting for delivering"},

		{LOGIN_SUCCESSFULL,"Login success, expecting for file list"},
		{REGISTERING_SUCCESSFULL,"Registering successfull, expecting for file list"},
		{DELETING_ACCOUNT_SUCCESSFULL, "Account was deleting successful, you can still interract with other users in this session"},

		{PUSH_FILES_SUCCESS, "Files were added successfully into server side Virtual File System"},
		{ADD_FILE_SUCCESSFULL, "File added successfully to server side Virtual File System"},
		{REMOVE_FILE_SUCCESSFULL, "File was removed successfully from server side Virtual File System"},
		{RENAME_FILE_SUCCESSFULL, "File was renamed successfully from server side Virtual File System"},

		{INVALID_CMD_WHILE_IN_CONN_STATE,"Invalid command while in connected state, server is waiting for SIGNUP or SIGNIN command"},
		{INVALID_CMD_WHILE_IN_LOGGED_STATE,"Invalid command while in logged state"},
		{INVALID_CMD_WHILE_IN_PROCESSING_STATE, "Invalid command while in processing state"},

		{NICKNAME_INCORRECT,"Nickname is not correct"},
		{PASSWORD_INCORRECT,"Password is not correct"},

		{REMOVE_FILE_FAILED, "File was not removed, most probably path was not correct, try later with another path"},
		{RENAME_FILE_FAILED,"File was not renamed, most probably path was not correct, try later with another path"},

		{BAD_CMD_IN_REPLY,"Invalid command id was received in the response to pushed command"},
		{BAD_NOTIFICATION_TYPE_MESSAGE_FORMAT, "Notification type message has invalid format"},
		{INVALID_SSL_VERSION, "SSL Version is not valid"},
		{SSL_VERSION_INCOMPATIBLE, "SSL Versions are incompatible"},

		{REMOVE_ACCOUNT_FAILED, "Failed to remove account, this account doesn't exist anymore"},
		{USER_ALREADY_LOGGED, "User with this nickname is logged already"},
		{USER_ALREADY_REGISTERED,"Nickname is not unique,supply another nickname"},

		{DOWNLOAD_DENY,"File download was denied"},

		{CONTENT_TYPE_UNSUPPORTED,"Specified content type is unsupported by server,please try another format"},

		{CMDID_HDR_REQUIRED,"Command-Id header is missing"},
		{RPL_CODE_HDR_REQUIRED,"Reply-Code header is missing, every REPLY command should be accompanied by this header"},
		{QUERRY_TYPE_HDR_REQUIRED, "Querry command should be accompanied by Querry-Type header"},
		{NOTIFICATION_TYPE_HDR_REQUIRED, "Notification type header is required for NOTIFICATION COMMAND"},

		{NICKNAME_HDR_REQUIRED,"Authentication command requires Nicnkname header"},
		{PASSWORD_HDR_REQUIRED,"Authentication command requires Password header"},

		{FILE_NAME_HDR_REQUIRED, "Querry associated with files must have File-Name header"},
		{FILE_NEW_NAME_HDR_REQUIRED, "Querry associated with file renaming must contain File-NewName header"},
		{FILE_TYPE_HDR_REQUIRED, "Querry associated with files must have File-Type header"},
		{FILE_OWNER_HDR_REQUIRED,"Notification associated with files must have File-Owner header"},
		{FILE_SIZE_HDR_REQUIRED, "Download querry response required File-Size header"},
				
		{CONTENT_TYPE_HDR_REQUIRED, "If message have body, then Content-Type header should be present"}	
	};
}