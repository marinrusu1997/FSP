#pragma once

namespace persistency::keys::common
{	
	constexpr static const char * const SERV_PORT		= "SERV_PORT";
	constexpr static const char * const	DOWNLOAD_PORT	= "DOWNLOAD_PORT";
}

namespace persistency::keys::client
{
	constexpr static const char * const SERV_ADDR_IPV4	= "SERV_ADDR_IPV4";
}

namespace persistency::keys::server
{
	constexpr static const char * const SEC_TO_W8_ON_INCOMPLETE_MSG = "SEC_W_INCOMPL_MSG";
}