#pragma once

#include <vector>

#include <Miracle/Common/Models/Mesh.hpp>

namespace Miracle {
	struct RendererConfig {
		std::vector<Mesh> meshes = {};
	};
}
