#pragma once

#include <array>

#include <Miracle/Application/IKeyboard.hpp>
#include <Miracle/Application/IMultimediaFramework.hpp>
#include <Miracle/Application/EventDispatcher.hpp>
#include <Miracle/Application/EventSubscriber.hpp>
#include <Miracle/Application/Models/Events/KeyInputEvent.hpp>
#include <Miracle/Infrastructure/View/Glfw/Window.hpp>

namespace Miracle::Infrastructure::Input::Glfw {
	struct KeyState {
	private:
		Application::KeyInputAction action;
		bool updated;

	public:
		inline bool isUpdated() const { return updated; }

		inline Application::KeyInputAction getAction() const { return action; }

		inline Application::KeyInputAction getActionAndSetStateAsDated() {
			updated = false;
			return action;
		}

		inline void setAction(Application::KeyInputAction action) {
			this->action = action;
			updated = true;
		}
	};

	class Keyboard :
		public Application::EventSubscriber<Application::EventTypes::keyInput>,
		public Application::IKeyboard
	{
	private:
		Application::IMultimediaFramework& m_multimediaFramework;
		View::Glfw::Window& m_window;

		std::array<KeyState, static_cast<size_t>(KeyboardKey::keyLast) + 1> m_keyStates = {};

	public:
		Keyboard(
			Application::EventDispatcher& eventDispatcher,
			Application::IMultimediaFramework& multimediaFramework,
			View::Glfw::Window& window
		);

		~Keyboard();

		virtual void onEvent(const Application::Event& event) override;

		virtual bool isKeyPressed(KeyboardKey key) override;

		virtual bool isKeyPressedOrRepeated(KeyboardKey key) override;

		virtual bool isKeyHeld(KeyboardKey key) const override;
	};
}
