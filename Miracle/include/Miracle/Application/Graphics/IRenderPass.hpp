#pragma once

#include <Miracle/Common/MiracleError.hpp>

namespace Miracle::Application {
	class IRenderPass {
	public:
		virtual ~IRenderPass() = default;
	};

	namespace RenderPassErrors {
		class CreationError : public RenderPassError {
		public:
			CreationError() : RenderPassError(
				RenderPassError::ErrorValue::creationError,
				"Failed to create render pass"
			) {}
		};
	}
}
