#pragma once

namespace Miracle::Application {
	class IContextTarget {
	public:
		virtual ~IContextTarget() = default;

		virtual bool isSizeChanged() = 0;

		virtual bool isCurrentlyPresentable() const = 0;
	};
}
