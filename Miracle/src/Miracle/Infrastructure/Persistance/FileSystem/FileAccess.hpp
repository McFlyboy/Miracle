#pragma once

#include <Miracle/Application/IFileAccess.hpp>
#include <Miracle/Application/ILogger.hpp>

namespace Miracle::Infrastructure::Persistance::FileSystem {
	class FileAccess : public Application::IFileAccess {
	private:
		Application::ILogger& m_logger;

	public:
		FileAccess(Application::ILogger& logger);

		virtual std::vector<char> readFileAsBinary(const std::string_view& filePath) const override;
	};
}
