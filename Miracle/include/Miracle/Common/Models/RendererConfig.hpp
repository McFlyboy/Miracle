#pragma once

#include <vector>

#include "SwapchainConfig.hpp"
#include "Mesh.hpp"

namespace Miracle {
	struct RendererConfig {
		SwapchainConfig swapchainConfig = {};
		std::vector<Mesh> meshes = {};
	};
}
