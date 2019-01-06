//
//  base64 encoding and decoding with C++.
//  Version: 1.01.00
//

#ifndef BASE64_H_C0CE2A47_D10E_42C9_A27C_C883944E704A
#define BASE64_H_C0CE2A47_D10E_42C9_A27C_C883944E704A

#include <string_view>

std::string base64_encode(const std::string_view s);
std::string base64_decode(const std::string_view s);

#endif /* BASE64_H_C0CE2A47_D10E_42C9_A27C_C883944E704A */
