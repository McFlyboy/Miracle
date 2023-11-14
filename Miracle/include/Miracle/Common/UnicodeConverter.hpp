#pragma once

#include <cstdint>
#include <string>
#include <string_view>

// UTF-8 encoding:
// https://en.wikipedia.org/wiki/UTF-8#Encoding

namespace Miracle {
	class UnicodeConverter {
	public:
		UnicodeConverter() = delete;

		static std::u8string_view toUtf8(const std::string_view& ascii) {
			return std::u8string_view(
				reinterpret_cast<const char8_t*>(ascii.data()),
				ascii.size()
			);
		}

		static constexpr std::u8string toUtf8(uint32_t codepoint) {
			auto result = std::u8string();

			if (codepoint <= 0x7Fu) [[likely]] {
				result += codepoint;
			}
			else if (codepoint > 0x7Fu && codepoint <= 0x07FFu) {
				result.reserve(2);

				result += 0b11000000u | (codepoint >> 6u);
				result += 0b10000000u | (codepoint & 0b111111u);
			}
			else if (codepoint > 0x07FFu && codepoint <= 0xFFFFu) {
				result.reserve(3);

				result += 0b11100000u | (codepoint >> 12u);
				result += 0b10000000u | ((codepoint >> 6) & 0b111111u);
				result += 0b10000000u | (codepoint & 0b111111u);
			}
			else if (codepoint > 0xFFFFu && codepoint <= 0x10FFFFu) {
				result.reserve(4);

				result += 0b11110000u | (codepoint >> 18u);
				result += 0b10000000u | ((codepoint >> 12u) & 0b111111u);
				result += 0b10000000u | ((codepoint >> 6u) & 0b111111u);
				result += 0b10000000u | (codepoint & 0b111111u);
			}

			return result;
		}
	};
}
