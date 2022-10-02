#pragma once

#include <vector>
#include <string>
#include <string_view>

#include <Miracle/Common/MiracleError.hpp>

namespace Miracle::Application {
	class IFileAccess {
	public:
		virtual ~IFileAccess() = default;

		virtual std::vector<char> readFileAsBinary(const std::string_view& filePath) const = 0;
	};

	namespace FileAccessErrors {
		class FileDoesNotExistError : public FileAccessError {
		public:
			FileDoesNotExistError(const std::string_view& filePath) : FileAccessError(
				FileAccessError::ErrorValue::fileDoesNotExistError,
				(std::string("File not found: ") + filePath.data()).c_str()
			) {}
		};

		class UnableToOpenFileError : public FileAccessError {
		public:
			UnableToOpenFileError(const std::string_view& filePath) : FileAccessError(
				FileAccessError::ErrorValue::unableToOpenFileError,
				(std::string("Could not open file: ") + filePath.data()).c_str()
			) {}
		};
	}
}
