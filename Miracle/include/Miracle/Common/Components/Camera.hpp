#pragma once

#include <Miracle/Common/Models/CameraProjectionType.hpp>

namespace Miracle {
	class Camera {
	private:
		CameraProjectionType m_projectionType;
		float m_zoomLevel;
		float m_fieldOfView;

	public:
		Camera(
			CameraProjectionType projectionType,
			float zoomLevel,
			float fieldOfView
		) :
			m_projectionType(projectionType),
			m_zoomLevel(zoomLevel),
			m_fieldOfView(fieldOfView)
		{}

		inline CameraProjectionType getProjectionType() const { return m_projectionType; }

		inline void setProjectionType(CameraProjectionType projectionType) {
			m_projectionType = projectionType;
		}

		inline float getZoomLevel() const { return m_zoomLevel; }

		inline void setZoomLevel(float zoomLevel) { m_zoomLevel = zoomLevel; }

		inline float getFieldOfView() const { return m_fieldOfView; }

		inline void setFieldOfView(float fieldOfView) { m_fieldOfView = fieldOfView; }
	};
}
