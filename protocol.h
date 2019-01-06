#pragma once
#include <inttypes.h>

namespace fsp::protocol {
	constexpr static const char			COMPRESSED_CHUNK_HEADER_LENGTH	= 6;
	constexpr static const uintmax_t	MAX_COMPRESSED_SEGMENT_LENGTH	= 999999;
	constexpr static const char			SENDER_RECEIVER_POSITION_DWNLD	= 0;
	constexpr static const char			DOWNLOAD_TRANSACTION_ID_POS		= 1;
	constexpr static const char	* const	SENDER_CODE_STR					= "S";
	constexpr static const char	* const RECEIVER_CODE_STR				= "R";
	constexpr static const char	* const START_TRANSMISSION_BYTE			= "0";
	constexpr static const char * const DOWNLOAD_TRANSACTION_DELIM_STR	= " ";
	constexpr static const char			DOWNLOAD_TRANSACTION_DELIM_CHR	= ' ';
	constexpr static const char * const CSV_DELIMITER_STR				= ",";
	constexpr static const char			CSV_DELIMITER_CHR				= ',';
	constexpr static const char			PATH_SEPARATOR_CHR				= '/';
	constexpr static const char * const PATH_SEPARATOR_STR				= "/";
	constexpr static const char * const PATH_ROOT						= "/";
	constexpr static const char * const LINE_SEPARATOR					= "\r\n";
	constexpr static const char * const HEADER_SEPARATOR				= ":";
	constexpr static const char * const EOHEADER						= "\r\n";
	constexpr static const char * const CMD_ID_WHEN_NO_CMDID_PRESENT	= "0";
	constexpr static const char * const HEARTBEAT_MESSAGE				= "HRTBT""\r\n""\r\n";
}