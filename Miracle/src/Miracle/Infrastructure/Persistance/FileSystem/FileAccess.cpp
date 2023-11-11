#include "FileAccess.hpp"

#include <filesystem>
#include <fstream>
#include <format>

namespace Miracle::Infrastructure::Persistance::FileSystem {
	FileAccess::FileAccess(Application::ILogger& logger) :
		m_logger(logger)
	{}

	std::vector<std::byte> FileAccess::readFileAsBinary(const std::filesystem::path& filePath) const {
		if (!std::filesystem::exists(filePath)) [[unlikely]] {
			m_logger.error(std::format("Could not find file {}", filePath.string()));
			throw Application::FileAccessErrors::FileDoesNotExistError(filePath);
		}

		auto fileStream = std::basic_ifstream<std::byte>(filePath, std::ifstream::binary);

		if (!fileStream.is_open()) [[unlikely]] {
			m_logger.error(std::format("Failed to open file {}", filePath.string()));
			throw Application::FileAccessErrors::UnableToOpenFileError(filePath);
		}

		auto fileSize = static_cast<size_t>(std::filesystem::file_size(filePath));
		auto buffer = std::vector<std::byte>(fileSize);

		fileStream.read(buffer.data(), fileSize);
		fileStream.close();

		return buffer;
	}
}
