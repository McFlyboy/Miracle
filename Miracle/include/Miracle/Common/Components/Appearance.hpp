#pragma once

#include <cstddef>

#include <Miracle/Common/Math/ColorRgb.hpp>

namespace Miracle {
	class Appearance {
	private:
		bool m_visible;
		size_t m_meshIndex;
		ColorRgb m_color;

	public:
		Appearance(
			bool visible,
			size_t meshIndex,
			ColorRgb color
		) :
			m_visible(visible),
			m_meshIndex(meshIndex),
			m_color(color)
		{}

		constexpr bool isVisible() const { return m_visible; }

		constexpr void setVisible(bool visible) { m_visible = visible; }

		constexpr size_t getMeshIndex() const { return m_meshIndex; }

		constexpr void setMeshIndex(size_t meshIndex) { m_meshIndex = meshIndex; }

		constexpr const ColorRgb& getColor() const { return m_color; }

		constexpr void setColor(const ColorRgb& color) { m_color = color; }
	};
}
