#pragma once

namespace Miracle::Graphics {
	class IGraphicsEngine {
	protected:
		IGraphicsEngine() = default;

	public:
		IGraphicsEngine(const IGraphicsEngine&) = delete;

		virtual ~IGraphicsEngine() = default;
	};
}
