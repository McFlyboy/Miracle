#pragma once

#include "Angle.hpp"

namespace Miracle {
	template<Angle TAngle>
	struct EulerAngles {
		TAngle xAngle = {};
		TAngle yAngle = {};
		TAngle zAngle = {};
	};
}
