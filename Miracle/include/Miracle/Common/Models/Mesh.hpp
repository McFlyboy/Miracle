#pragma once

#include <vector>

#include "Vertex.hpp"
#include "Face.hpp"

namespace Miracle {
	struct Mesh {
		std::vector<Vertex> vertices = {};
		std::vector<Face> faces = {};
	};
}
