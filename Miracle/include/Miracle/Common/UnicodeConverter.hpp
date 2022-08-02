#pragma once

#include <string>
#include <string_view>

namespace Miracle {
	class UnicodeConverter {
	public:
		UnicodeConverter() = delete;

		static inline std::u8string_view toUtf8(const std::string_view& ascii) {
			return std::u8string_view(
				reinterpret_cast<const char8_t*>(ascii.data()),
				ascii.size()
			);
		}

		static std::u8string toUtf8(const std::u32string_view& utf32);

		static std::u32string toUtf32(const std::u8string_view& utf8);
	};
}
