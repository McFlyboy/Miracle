#pragma once

#include <memory>

#include "IVertexBuffer.hpp"
#include "IIndexBuffer.hpp"

namespace Miracle::Application {
	struct MeshBuffers {
		std::unique_ptr<IVertexBuffer> vertexBuffer;
		std::unique_ptr<IIndexBuffer> indexBuffer;
	};
}
