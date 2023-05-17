#pragma once
#include "Camera.h"
#include "BaseObject.h"

class CameraTracking :
	public Camera
{
public:
	using Camera::Camera;

	// コンストラクタ
	CameraTracking();

	inline void SetTrackingTarget(BaseObject* trackingTarget) { this->trackingTarget = trackingTarget; };

	void StartUpdate() override;

private:
	// トラッキングする標的
	BaseObject* trackingTarget;

	XMFLOAT3 trackingTargetToCameraTarget = { 0.f,20.f,0.f };

	// カメラの視点から注視点までの距離
	float eyeToCameraTargetLength = 60;
};
