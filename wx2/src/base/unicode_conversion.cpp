#include "wx2/base/unicode_conversion.h"
#include <stdexcept>
#include <vector>

// LOW_3BITS 0x7
// LOW_4BITS 0xF
// LOW_5BITS 0x1F
// LOW_6BITS 0x3F
// BIT4 0x8
// BIT5 0x10
// BIT6 0x20
// BIT7 0x40
// BIT8 0x80
// L_SURROGATE_START 0xDC00
// L_SURROGATE_END 0xDFFF
// H_SURROGATE_START 0xD800
// H_SURROGATE_END 0xDBFF
// SURROGATE_PAIR_START 0x10000

namespace wx2
{
	namespace detail
	{
        std::size_t count_utf8_to_utf16(const std::u8string_view utf8str)
        {
            const std::size_t size = utf8str.size();
            auto const data = reinterpret_cast<const char*>(utf8str.data());
            std::size_t result = size;

            for (size_t index = 0; index < size;)
            {
                if (data[index] >= 0)
                {
                    while ((++index < size) && (data[index] >= 0))

                    if (index >= size) break;
                }

                // start special handling for multi-byte code points
                const char c = data[index++];

                if ((c & 0x40) == 0)
                {
                    throw std::range_error("UTF-8 string character can never start with 10xxxxxx");
                }
                else if ((c & 0x20) == 0) // 2 byte character, 0x80 to 0x7FF
                {
                    if (index == size)
                    {
                        throw std::range_error("UTF-8 string is missing bytes in character");
                    }

                    const char c2 = data[index++];
                    if ((c2 & 0xC0) != 0x80)
                    {
                        throw std::range_error("UTF-8 continuation byte is missing leading bit mask");
                    }

                    // can't require surrogates for 7FF
                    --result;
                }
                else if ((c & 0x10) == 0) // 3 byte character, 0x800 to 0xFFFF
                {
                    if (size - index < 2)
                    {
                        throw std::range_error("UTF-8 string is missing bytes in character");
                    }

                    const char c2 = data[index++];
                    const char c3 = data[index++];
                    if (((c2 | c3) & 0xC0) != 0x80)
                    {
                        throw std::range_error("UTF-8 continuation byte is missing leading bit mask");
                    }

                    result -= 2;
                }
                else if ((c & 0x8) == 0) // 4 byte character, 0x10000 to 0x10FFFF
                {
                    if (size - index < 3)
                    {
                        throw std::range_error("UTF-8 string is missing bytes in character");
                    }

                    const char c2 = data[index++];
                    const char c3 = data[index++];
                    const char c4 = data[index++];
                    if (((c2 | c3 | c4) & 0xC0) != 0x80)
                    {
                        throw std::range_error("UTF-8 continuation byte is missing leading bit mask");
                    }

                    const uint32_t codePoint =
                        ((c  & 0b00000111) << 18) | 
                        ((c2 & 0b00111111) << 12) | 
                        ((c3 & 0b00111111) << 6 ) | 
                        ((c4 & 0b00111111) << 0 );
                    result -= (3 - (codePoint >= 0x10000));
                }
                else
                {
                    throw std::range_error("UTF-8 string has invalid Unicode code point");
                }
            }

            return result;
        }
	}

    std::u16string utf8_to_utf16(const std::u8string_view utf8str)
    {
        // Save repeated heap allocations, use the length of resulting sequence.
        const std::size_t src_size = utf8str.size();
        auto const src_data = reinterpret_cast<const char*>(utf8str.data());
        std::u16string dest(detail::count_utf8_to_utf16(utf8str), L'\0');
        std::u16string::value_type* const dest_data = &dest[0];
        std::size_t dest_index = 0;

        for (std::size_t index = 0; index < src_size; ++index)
        {
            char src = src_data[index];
            switch (src & 0xF0)
            {
            case 0xF0: // 4 byte character, 0x10000 to 0x10FFFF
            {
                const char c2 = src_data[++index];
                const char c3 = src_data[++index];
                const char c4 = src_data[++index];
                uint32_t codePoint =
                    ((src & 0b00000111) << 18) | 
                    ((c2  & 0b00111111) << 12) | 
                    ((c3  & 0b00111111) << 6 ) |
                    ((c4  & 0b00111111) << 0 );
                if (codePoint >= 0x10000)
                {
                    // In UTF-16 U+10000 to U+10FFFF are represented as two 16-bit code units, surrogate pairs.
                    //  - 0x10000 is subtracted from the code point
                    //  - high surrogate is 0xD800 added to the top ten bits
                    //  - low surrogate is 0xDC00 added to the low ten bits
                    codePoint -= 0x10000;
                    dest_data[dest_index++] = static_cast<std::u16string::value_type>((codePoint >> 10) | 0xD800);
                    dest_data[dest_index++] = static_cast<std::u16string::value_type>((codePoint & 0x3FF) | 0xDC00);
                }
                else
                {
                    // In UTF-16 U+0000 to U+D7FF and U+E000 to U+FFFF are represented exactly as the Unicode code point
                    // value. U+D800 to U+DFFF are not valid characters, for simplicity we assume they are not present
                    // but will encode them if encountered.
                    dest_data[dest_index++] = static_cast<std::u16string::value_type>(codePoint);
                }
            }
            break;
            case 0xE0: // 3 byte character, 0x800 to 0xFFFF
            {
                const char c2 = src_data[++index];
                const char c3 = src_data[++index];
                dest_data[dest_index++] = static_cast<std::u16string::value_type>(
                    ((src & 0b00001111) << 12) | 
                    ((c2  & 0b00111111) << 6 ) | 
                    ((c3  & 0b00111111) << 0 ));
            }
            break;
            case 0xD0: // 2 byte character, 0x80 to 0x7FF
            case 0xC0:
            {
                const char c2 = src_data[++index];
                dest_data[dest_index++] = static_cast<std::u16string::value_type>(
                    ((src & 0b00011111) << 6) | 
                    ((c2  & 0b00111111) << 0));
            }
            break;
            default: // single byte character, 0x0 to 0x7F
                // try to use a fast inner loop for following single byte characters,
                // since they are quite probable
                do
                {
                    dest_data[dest_index++] = static_cast<std::u16string::value_type>(src_data[index++]);
                } while (index < src_size && src_data[index] > 0);
                // adjust index since it will be incremented by the for loop
                --index;
            }
        }
        return dest;
    }
}
