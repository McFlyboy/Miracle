#pragma once

#include <array>
#include <functional>
#include <utility>

#include <Miracle/Application/IKeyboard.hpp>
#include <Miracle/Application/IMultimediaFramework.hpp>
#include <Miracle/Application/EventDispatcher.hpp>
#include <Miracle/Application/EventSubscriber.hpp>
#include <Miracle/Application/Events/KeyInputEvent.hpp>
#include <Miracle/Infrastructure/View/Glfw/Window.hpp>

namespace Miracle::Infrastructure::Input::Glfw {
	class Keyboard :
		public Application::EventSubscriber<Application::KeyInputEvent>,
		public Application::IKeyboard
	{
	private:
		Application::IMultimediaFramework& m_multimediaFramework;
		View::Glfw::Window& m_window;

		std::array<Application::KeyState, static_cast<size_t>(KeyboardKey::keyLast) + 1> m_keyStates = {};
		std::function<void(KeyboardKey)> m_keyPressedCallback = [](KeyboardKey) {};

	public:
		Keyboard(
			Application::EventDispatcher& eventDispatcher,
			Application::IMultimediaFramework& multimediaFramework,
			View::Glfw::Window& window
		);

		~Keyboard();

		virtual bool isKeyPressed(KeyboardKey key) const override;

		virtual bool isKeyPressedOrRepeated(KeyboardKey key) const override;

		virtual bool isKeyHeld(KeyboardKey key) const override;

		virtual void setAllKeyStatesAsDated() override;

		virtual void setKeyPressedCallback(std::function<void(KeyboardKey)>&& keyPressedCallback) override {
			m_keyPressedCallback = std::move(keyPressedCallback);
		}

		virtual void unsetKeyPressedCallback() override {
			m_keyPressedCallback = [](KeyboardKey) {};
		}

	private:
		void handleKeyInputEvent(const Application::KeyInputEvent& event);
	};
}
