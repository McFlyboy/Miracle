#include "FileAccess.hpp"

#include <filesystem>
#include <fstream>

#include <fmt/format.h>

namespace Miracle::Infrastructure::Persistance::FileSystem {
	FileAccess::FileAccess(Application::ILogger& logger) :
		m_logger(logger)
	{}

	std::vector<char> FileAccess::readFileAsBinary(const std::string_view& filePath) const {
		if (!std::filesystem::exists(filePath)) [[unlikely]] {
			m_logger.error(fmt::format("Could not find file {}", filePath));
			throw Application::FileAccessErrors::FileDoesNotExistError(filePath);
		}

		auto fileStream = std::ifstream(filePath.data(), std::ifstream::ate | std::ifstream::binary);

		if (!fileStream.is_open()) [[unlikely]] {
			m_logger.error(fmt::format("Failed to open file {}", filePath));
			throw Application::FileAccessErrors::UnableToOpenFileError(filePath);
		}

		auto fileSize = static_cast<size_t>(fileStream.tellg());
		auto buffer = std::vector<char>(fileSize);

		fileStream.seekg(0);
		fileStream.read(buffer.data(), fileSize);
		fileStream.close();

		return buffer;
	}
}
