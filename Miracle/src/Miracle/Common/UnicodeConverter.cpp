#include <Miracle/Common/UnicodeConverter.hpp>

// UTF-8 encoding conversion:
// https://en.wikipedia.org/wiki/UTF-8#Encoding

namespace Miracle {
	std::u8string UnicodeConverter::toUtf8(const std::u32string_view& utf32) {
		auto result = std::u8string();

		for (auto& character : utf32) {
			auto& codepoint = reinterpret_cast<const unsigned int&>(character);

			if (codepoint <= 0x00007F) [[likely]] {
				result += codepoint;
			}
			else if (codepoint > 0x00007F && codepoint <= 0x0007FF) {
				result.reserve(2);

				result += 0b11000000 | (codepoint >> 6);
				result += 0b10000000 | (codepoint & 0b111111);
			}
			else if (codepoint > 0x0007FF && codepoint <= 0x00FFFF) {
				result.reserve(3);

				result += 0b11100000 | (codepoint >> 12);
				result += 0b10000000 | ((codepoint >> 6) & 0b111111);
				result += 0b10000000 | (codepoint & 0b111111);
			}
			else if (codepoint > 0x00FFFF && codepoint <= 0x10FFFF) {
				result.reserve(4);

				result += 0b11110000 | (codepoint >> 18);
				result += 0b10000000 | ((codepoint >> 12) & 0b111111);
				result += 0b10000000 | ((codepoint >> 6) & 0b111111);
				result += 0b10000000 | (codepoint & 0b111111);
			}
		}

		return result;
	}

	std::u32string UnicodeConverter::toUtf32(const std::u8string_view& utf8) {
		auto result = std::u32string();

		for (size_t i = 0; i < utf8.size();) {
			if (utf8[i] >> 7 == 0) [[likely]] {
				result += utf8[i];

				i++;
			}
			else if (utf8[i] >> 5 == 0b110) {
				result += (static_cast<char32_t>(utf8[i] & 0b11111) << 6)
					| static_cast<char32_t>(utf8[i + 1] & 0b111111);

				i += 2;
			}
			else if (utf8[i] >> 4 == 0b1110) {
				result += (static_cast<char32_t>(utf8[i] & 0b1111) << 12)
					| (static_cast<char32_t>(utf8[i + 1] & 0b111111) << 6)
					| static_cast<char32_t>(utf8[i + 2] & 0b111111);

				i += 3;
			}
			else if (utf8[i] >> 3 == 0b11110) {
				result += (static_cast<char32_t>(utf8[i] & 0b111) << 18)
					| (static_cast<char32_t>(utf8[i + 1] & 0b111111) << 12)
					| (static_cast<char32_t>(utf8[i + 2] & 0b111111) << 6)
					| static_cast<char32_t>(utf8[i + 3] & 0b111111);

				i += 4;
			}
			else {
				i++;
			}
		}

		return result;
	}
}
