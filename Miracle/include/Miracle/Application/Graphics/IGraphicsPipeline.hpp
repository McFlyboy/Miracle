#pragma once

#include <Miracle/Common/MiracleError.hpp>
#include "PushConstants.hpp"

namespace Miracle::Application {
	class IGraphicsPipeline {
	public:
		virtual ~IGraphicsPipeline() = default;

		virtual void bind() = 0;

		virtual void pushConstants(const PushConstants& constants) = 0;
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
