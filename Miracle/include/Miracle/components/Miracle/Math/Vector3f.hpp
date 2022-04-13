#pragma once

namespace Miracle::Math {
	struct Vector3f {
		union {
			float x = {};
			float r;
		};

		union {
			float y = {};
			float g;
		};

		union {
			float z = {};
			float b;
		};
	};
}
