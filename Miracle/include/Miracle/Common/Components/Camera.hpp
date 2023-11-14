#pragma once

#include <cmath>

#include <Miracle/Common/Models/CameraProjectionType.hpp>
#include <Miracle/Common/Math/Angle.hpp>

namespace Miracle {
	class Camera {
	private:
		CameraProjectionType m_projectionType;
		float m_zoomFactor;
		float m_nearClipPlaneDistance;
		float m_farClipPlaneDistance;

	public:
		constexpr Camera(
			float zoomFactor,
			float nearClipPlaneDistance,
			float farClipPlaneDistance
		) :
			m_projectionType(CameraProjectionType::orthographic),
			m_zoomFactor(zoomFactor),
			m_nearClipPlaneDistance(nearClipPlaneDistance),
			m_farClipPlaneDistance(farClipPlaneDistance)
		{}

		constexpr Camera(
			Angle auto fieldOfView,
			float nearClipPlaneDistance,
			float farClipPlaneDistance
		) :
			m_projectionType(CameraProjectionType::perspective),
			m_zoomFactor(fieldOfViewToZoomFactor(fieldOfView)),
			m_nearClipPlaneDistance(nearClipPlaneDistance),
			m_farClipPlaneDistance(farClipPlaneDistance)
		{}

		constexpr CameraProjectionType getProjectionType() const { return m_projectionType; }

		constexpr void setProjectionType(CameraProjectionType projectionType) {
			m_projectionType = projectionType;
		}

		constexpr float getZoomFactor() const { return m_zoomFactor; }

		constexpr void setZoomFactor(float zoomFactor) { m_zoomFactor = zoomFactor; }

		template <Angle TAngle>
		TAngle getFieldOfView() const { return zoomFactorToFieldOfView<TAngle>(m_zoomFactor); }

		void setFieldOfView(Angle auto fieldOfView) { m_zoomFactor = fieldOfViewToZoomFactor(fieldOfView); }

		constexpr float getNearClipPlaneDistance() const { return m_nearClipPlaneDistance; }

		constexpr void setNearClipPlaneDistance(float nearClipPlaneDistance) { m_nearClipPlaneDistance = nearClipPlaneDistance; }

		constexpr float getFarClipPlaneDistance() const { return m_farClipPlaneDistance; }

		constexpr void setFarClipPlaneDistance(float farClipPlaneDistance) { m_farClipPlaneDistance = farClipPlaneDistance; }

	private:
		template <Angle TAngle>
		TAngle zoomFactorToFieldOfView(float zoomFactor) const {
			return static_cast<TAngle>(Radians{ 2.0f * std::atan(1.0f / m_zoomFactor) });
		}

		float fieldOfViewToZoomFactor(Angle auto fieldOfView) const {
			return 1.0f / std::tan(static_cast<Radians>(fieldOfView).value / 2.0f);
		}
	};
}
