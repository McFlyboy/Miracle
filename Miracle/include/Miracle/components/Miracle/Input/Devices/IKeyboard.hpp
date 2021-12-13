#pragma once

namespace Miracle::Input::Devices {
	class MIRACLE_API IKeyboard {
	protected:
		IKeyboard() = default;

	public:
		IKeyboard(const IKeyboard&) = delete;
		virtual ~IKeyboard() = default;
	};
}
