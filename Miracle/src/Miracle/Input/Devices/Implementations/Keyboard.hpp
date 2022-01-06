#pragma once

#include <cstdint>
#include <array>

#include <Miracle/components/Miracle/Input/Devices/IKeyboard.hpp>

#include <Miracle/View/Implementations/Window.hpp>

using namespace Miracle::View::Implementations;

namespace Miracle::Input::Devices::Implementations {
	class Keyboard : public IKeyboard {
	private:
		const Window& m_window;

	public:
		Keyboard(const Window& window);
		~Keyboard() override;

		virtual bool keyPressed(const Key& key) override;
		virtual bool keyPressedContinuously(const Key& key) override;
		virtual bool keyIsDown(const Key& key) override;

		enum class KeyStates : int8_t {
			/* Action states */
			Released          = 0x00,
			Pressed           = 0x01,
			Repeated          = 0x02,

			/* Checked states */
			Checked           = 0x00,
			Unchecked         = 0x10,

			/* Combined states */
			ReleasedChecked   = Released | Checked,
			PressedChecked    = Pressed  | Checked,
			RepeatedChecked   = Repeated | Checked,
			ReleasedUnchecked = Released | Unchecked,
			PressedUnchecked  = Pressed  | Unchecked,
			RepeatedUnchecked = Repeated | Unchecked
		};
	};
}
