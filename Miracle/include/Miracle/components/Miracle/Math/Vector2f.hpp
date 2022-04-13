#pragma once

namespace Miracle::Math {
	struct Vector2f {
		union {
			float x = {};
			float r;
		};

		union {
			float y = {};
			float g;
		};
	};
}
