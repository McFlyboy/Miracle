#pragma once

#include <cstddef>
#include <vector>
#include <string>
#include <filesystem>

#include <Miracle/Common/MiracleError.hpp>

namespace Miracle::Application {
	class IFileAccess {
	public:
		virtual ~IFileAccess() = default;

		virtual std::vector<std::byte> readFileAsBinary(const std::filesystem::path& filePath) const = 0;
	};

	namespace FileAccessErrors {
		class FileDoesNotExistError : public FileAccessError {
		public:
			FileDoesNotExistError(const std::filesystem::path& filePath) : FileAccessError(
				FileAccessError::ErrorValue::fileDoesNotExistError,
				std::string("File not found: ") + filePath.string()
			) {}
		};

		class UnableToOpenFileError : public FileAccessError {
		public:
			UnableToOpenFileError(const std::filesystem::path& filePath) : FileAccessError(
				FileAccessError::ErrorValue::unableToOpenFileError,
				std::string("Could not open file: ") + filePath.string()
			) {}
		};
	}
}
