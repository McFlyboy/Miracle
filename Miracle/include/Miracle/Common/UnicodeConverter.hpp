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

		static constexpr std::u8string_view toUtf8(const std::string_view& ascii) {
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

		static constexpr std::u8string toUtf8(const std::u32string_view& utf32) {
			auto result = std::u8string();

			for (auto& codepoint : utf32) {
				if (codepoint <= 0x7F) [[likely]] {
					result += codepoint;
				}
				else if (codepoint > 0x7F && codepoint <= 0x07FF) {
					result.reserve(2);

					result += 0b11000000 | (codepoint >> 6);
					result += 0b10000000 | (codepoint & 0b111111);
				}
				else if (codepoint > 0x07FF && codepoint <= 0xFFFF) {
					result.reserve(3);

					result += 0b11100000 | (codepoint >> 12);
					result += 0b10000000 | ((codepoint >> 6) & 0b111111);
					result += 0b10000000 | (codepoint & 0b111111);
				}
				else if (codepoint > 0xFFFF && codepoint <= 0x10FFFF) {
					result.reserve(4);

					result += 0b11110000 | (codepoint >> 18);
					result += 0b10000000 | ((codepoint >> 12) & 0b111111);
					result += 0b10000000 | ((codepoint >> 6) & 0b111111);
					result += 0b10000000 | (codepoint & 0b111111);
				}
			}

			return result;
		}

		static constexpr std::u32string toUtf32(const std::u8string_view& utf8) {
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
	};
}
