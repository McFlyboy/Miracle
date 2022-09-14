#pragma once

namespace Miracle::Application {
	class IContextTarget {
	public:
		virtual ~IContextTarget() = default;

		inline virtual bool stateChanged() = 0;
	};
}
