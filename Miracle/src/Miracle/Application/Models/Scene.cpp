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

		if (initProps.entityBehaviourFactory.has_value()) {
			m_container->addEntityBehaviour(initProps.entityBehaviourFactory.value()());
		}
	}

	void Scene::setBackgroundColor(const Color3f& color) {
		m_backgroundColor = color;
	}

	void Scene::setEntityPosition(const Vector2f& position) {
		m_container->setEntityPosition(position);
	}

	void Scene::start() {
		m_container->forEachEntityBehaviour(
			[](Behaviour& behaviour) {
				behaviour.start();
			}
		);
	}

	void Scene::update() {
		m_container->forEachEntityBehaviour(
			[](Behaviour& behaviour) {
				behaviour.update();
			}
		);
	}
}
