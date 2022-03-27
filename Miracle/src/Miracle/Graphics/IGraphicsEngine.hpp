#pragma once

#include <optional>

#include <Miracle/MiracleError.hpp>

namespace Miracle::Graphics {
	class IGraphicsEngine {
	protected:
		IGraphicsEngine() = default;

	public:
		IGraphicsEngine(const IGraphicsEngine&) = delete;

		virtual ~IGraphicsEngine() = default;

		virtual std::optional<MiracleError> render() = 0;

		virtual std::optional<MiracleError> waitForExecutionToFinish() = 0;
	};
}
