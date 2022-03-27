#include "ResourceLoader.hpp"

#include <fstream>
#include <utility>

#include <fmt/format.h>

#include <Miracle/components/Miracle/Diagnostics/Logger.hpp>

using namespace Miracle::Diagnostics;

namespace Miracle::Io {
	std::variant<MiracleError, std::vector<char>> ResourceLoader::loadFileBinary(const char* filename) const {
		auto filepath = fmt::format("resources/{}", filename);

		auto file = std::ifstream(filepath, std::ios::binary | std::ios::ate);

		if (!file.is_open()) {
			Logger::error(fmt::format("Failed to read resource file: {}!", filename));
			return MiracleError::ResourceFileReadError;
		}

		auto buffer = std::vector<char>(file.tellg());

		file.seekg(0);
		file.read(buffer.data(), buffer.size());

		file.close();

		Logger::info(fmt::format("Resource file loaded: {}", filename));
		return std::move(buffer);
	}
}
