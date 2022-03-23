#pragma once

#include <vector>
#include <variant>

#include <Miracle/MiracleError.hpp>

namespace Miracle::Io {
	class ResourceLoader {
	public:
		std::variant<MiracleError, std::vector<char>> loadFileBinary(const char* filename) const;
	};
}
