#pragma once

namespace Miracle {
	class Behaviour {
	public:
		virtual ~Behaviour() = default;

		virtual void start() {}

		virtual void update() {}
	};
}
