#include <wx2/base/unicode_conversion.h>

int main()
{
	auto u16 = wx2::utf8_to_utf16(u8"abc‚ ‚¢‚¤");
	
	return 0;
}