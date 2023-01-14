#include <Miracle/Application/Models/Scene.hpp>

namespace Miracle::Application {
	Scene::Scene(
		IEcs& ecs,
		const SceneInitProps& initProps
	) :
		m_container(ecs.createContainer()),
		m_backgroundColor(initProps.backgroundColor)
	{
		m_container->setEntityPosition(initProps.entityPosition);
	}

	void Scene::setBackgroundColor(const Color3f& color) {
		m_backgroundColor = color;
	}

	void Scene::setEntityPosition(const Vector2f& position) {
		m_container->setEntityPosition(position);
	}
}
