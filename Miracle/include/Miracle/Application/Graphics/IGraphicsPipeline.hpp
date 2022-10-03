#pragma once

#include <Miracle/Common/MiracleError.hpp>

namespace Miracle::Application {
	class IGraphicsPipeline {
	public:
		virtual ~IGraphicsPipeline() = default;
	};

	namespace GraphicsPipelineErrors {
		class CreationError : public GraphicsPipelineError {
		public:
			CreationError() : GraphicsPipelineError(
				GraphicsPipelineError::ErrorValue::creationError,
				"Failed to create graphics pipeline"
			) {}
		};
	}
}
