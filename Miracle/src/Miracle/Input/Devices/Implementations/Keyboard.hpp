#pragma once

#include <cstdint>
#include <array>

#include <Miracle/components/Miracle/Input/Devices/IKeyboard.hpp>

#include <Miracle/View/Implementations/Window.hpp>

using namespace Miracle::View::Implementations;

namespace Miracle::Input::Devices::Implementations {
	class Keyboard : public IKeyboard {
	public:
		Keyboard(const Window& window);

		virtual bool keyPressed(const Key& key) override;
		virtual bool keyPressedOrRepeated(const Key& key) override;
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
