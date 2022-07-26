#pragma once

#include <Miracle/Common/MiracleError.hpp>

namespace Miracle::Application {
	class IMultimediaFramework {
	public:
		virtual ~IMultimediaFramework() = default;

		virtual void processEvents() = 0;
	};

	namespace MultimediaFrameworkErrors {
		class MultimediaFrameworkInitError : public MultimediaFrameworkError {
		public:
			MultimediaFrameworkInitError() : MultimediaFrameworkError(
				MultimediaFrameworkError::ErrorValue::initError,
				"Failed to initialize underlying multimedia framework"
			) {}
		};
	}
}
