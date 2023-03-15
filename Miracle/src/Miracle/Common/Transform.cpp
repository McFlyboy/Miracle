#include <Miracle/Common/Transform.hpp>

namespace Miracle {
	Transform::Transform(const Vector3& translation) :
		m_transformationMatrix(Matrix4::createTranslation(translation))
	{}

	void Transform::translate(const Vector3& translation) {
		m_transformationMatrix *= Matrix4::createTranslation(translation);
	}
}
