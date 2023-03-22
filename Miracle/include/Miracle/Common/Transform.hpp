#pragma once

#include "Math/Matrix4.hpp"

namespace Miracle {
	class Transform {
	private:
		Matrix4 m_transformation;

	public:
		Transform(
			const Vector3& translation,
			const Vector3& scale
		);

		inline const Matrix4& getTransformation() const { return m_transformation; }

		inline Matrix4& getTransformation() { return m_transformation; }
	};
}
