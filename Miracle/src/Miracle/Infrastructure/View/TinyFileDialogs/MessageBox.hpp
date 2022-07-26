#pragma once

#include <cstdint>
#include <string>
#include <utility>

#include <tinyfiledialogs/tinyfiledialogs.h>

namespace Miracle::Infrastructure::View::TinyFileDialogs {
	class MessageBox {
	public:
		enum class Icon : uint8_t {
			info,
			warning,
			error,
			question
		};

	private:
		const std::string m_title;
		const std::string m_message;
		Icon m_icon;

	public:
		MessageBox(
			const std::string& title,
			const std::string& message,
			Icon icon
		) :
			m_title(title),
			m_message(message),
			m_icon(icon)
		{}

		MessageBox(
			std::string&& title,
			std::string&& message,
			Icon icon
		) :
			m_title(std::move(title)),
			m_message(std::move(message)),
			m_icon(icon)
		{}

		inline void show() const {
			switch (m_icon) {
			case Icon::info:
				tinyfd_messageBox(m_title.c_str(), m_message.c_str(), "ok", "info", 1);
				break;

			case Icon::warning:
				tinyfd_messageBox(m_title.c_str(), m_message.c_str(), "ok", "warning", 1);
				break;

			case Icon::error:
				tinyfd_messageBox(m_title.c_str(), m_message.c_str(), "ok", "error", 1);
				break;

			case Icon::question:
				tinyfd_messageBox(m_title.c_str(), m_message.c_str(), "ok", "question", 1);
				break;
			}
		}
	};
}
