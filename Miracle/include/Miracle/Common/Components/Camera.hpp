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
		Camera(
			float zoomFactor,
			float nearClipPlaneDistance,
			float farClipPlaneDistance
		) :
			m_projectionType(CameraProjectionType::orthographic),
			m_zoomFactor(zoomFactor),
			m_nearClipPlaneDistance(nearClipPlaneDistance),
			m_farClipPlaneDistance(farClipPlaneDistance)
		{}

		template <Angle TAngle>
		Camera(
			TAngle fieldOfView,
			float nearClipPlaneDistance,
			float farClipPlaneDistance
		) :
			m_projectionType(CameraProjectionType::perspective),
			m_zoomFactor(fieldOfViewToZoomFactor(fieldOfView)),
			m_nearClipPlaneDistance(nearClipPlaneDistance),
			m_farClipPlaneDistance(farClipPlaneDistance)
		{}

		inline CameraProjectionType getProjectionType() const { return m_projectionType; }

		inline void setProjectionType(CameraProjectionType projectionType) {
			m_projectionType = projectionType;
		}

		inline float getZoomFactor() const { return m_zoomFactor; }

		inline void setZoomFactor(float zoomFactor) { m_zoomFactor = zoomFactor; }

		template <Angle TAngle>
		inline TAngle getFieldOfView() const { return zoomFactorToFieldOfView<TAngle>(m_zoomFactor); }

		template <Angle TAngle>
		inline void setFieldOfView(TAngle fieldOfView) { m_zoomFactor = fieldOfViewToZoomFactor(fieldOfView); }

		inline float getNearClipPlaneDistance() const { return m_nearClipPlaneDistance; }

		inline void setNearClipPlaneDistance(float nearClipPlaneDistance) { m_nearClipPlaneDistance = nearClipPlaneDistance; }

		inline float getFarClipPlaneDistance() const { return m_farClipPlaneDistance; }

		inline void setFarClipPlaneDistance(float farClipPlaneDistance) { m_farClipPlaneDistance = farClipPlaneDistance; }

	private:
		template <Angle TAngle>
		inline TAngle zoomFactorToFieldOfView(float zoomFactor) const {
			return static_cast<TAngle>(Radians{ 2.0f * std::atan(1.0f / m_zoomFactor) });
		}

		template <Angle TAngle>
		inline float fieldOfViewToZoomFactor(TAngle fieldOfView) const {
			return 1.0f / std::tan(static_cast<Radians>(fieldOfView).value / 2.0f);
		}
	};
}
