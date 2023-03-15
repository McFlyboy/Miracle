#pragma once

#include "Math/Vector3.hpp"
#include "Math/Matrix4.hpp"

namespace Miracle {
	class Transform {
	private:
		Matrix4 m_transformationMatrix;

	public:
		Transform(const Vector3& translation);

		inline const Matrix4& getMatrix() const { return m_transformationMatrix; }

		void translate(const Vector3& translation);
	};
}
