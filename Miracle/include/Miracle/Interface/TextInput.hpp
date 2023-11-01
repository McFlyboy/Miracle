#pragma once

#include <string>
#include <functional>
#include <utility>

#include <Miracle/App.hpp>
#include <Miracle/Application/TextInputService.hpp>

namespace Miracle {
	using TextInputReceivedCallback = Application::TextInputReceivedCallback;

	class TextInput {
	public:
		TextInput() = delete;

		static void setTextInputReceiver(
			std::u8string& textInputReceiver,
			TextInputReceivedCallback&& textInputReceivedCallback = []() {}
		) {
			if (App::s_currentApp == nullptr) [[unlikely]] throw NoAppRunningError();

			App::s_currentApp->m_dependencies->getTextInputService().setTextInputReceiver(
				textInputReceiver,
				std::move(textInputReceivedCallback)
			);
		}

		static void unsetTextInputReceiver() {
			if (App::s_currentApp == nullptr) [[unlikely]] throw NoAppRunningError();

			App::s_currentApp->m_dependencies->getTextInputService().unsetTextInputReceiver();
		}
	};
}
