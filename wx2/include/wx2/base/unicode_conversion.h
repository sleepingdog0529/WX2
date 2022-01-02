#pragma once
#include <string_view>

namespace wx2
{
	namespace detail
	{
        [[nodiscard]] std::size_t count_utf8_to_utf16(std::u8string_view utf8str);
	}

    [[nodiscard]] std::u16string utf8_to_utf16(std::u8string_view utf8str);
    //std::u32string ConvU8ToU32(std::string_view u8Str);

    //std::u8string ConvU16ToU8(std::u16string_view u16Str);
    //std::u32string ConvU16ToU32(const std::u16string& u16Str);

    //std::string ConvU32ToU8(std::u32string_view u32Str);
    //std::u16string ConvU32ToU16(std::u32string_view u32Str);
}
