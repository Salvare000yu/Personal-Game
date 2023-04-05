#pragma once
#include "Camera.h"
#include "BaseObject.h"

class CameraTracking :
	public Camera
{
public:
	using Camera::Camera;

	// �R���X�g���N�^
	CameraTracking();

	inline void SetTrackingTarget(BaseObject* trackingTarget) { this->trackingTarget = trackingTarget; };

	void StartUpdate() override;

private:
	// �g���b�L���O����W�I
	BaseObject* trackingTarget;

	XMFLOAT3 trackingTargetToCameraTarget = { 0.f,20.f,0.f };

	// �J�����̎��_���璍���_�܂ł̋���
	float eyeToCameraTargetLength = 60;
};
