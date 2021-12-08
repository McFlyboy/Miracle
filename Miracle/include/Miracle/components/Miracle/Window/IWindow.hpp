#pragma once

namespace Miracle::Window {
	class MIRACLE_API IWindow {
	public:
		virtual void update() const = 0;
		virtual bool shouldClose() const = 0;
	};
}
