#pragma once
#include "Object3d.h"
#include "BaseObject.h"

#include <memory>

class PlayerBullet :public BaseObject
{
private:
	// Microsoft::WRL::‚ğÈ—ª
	template <class T> using ComPtr = Microsoft::WRL::ComPtr<T>;
	// DirectX::‚ğÈ—ª
	using XMFLOAT2 = DirectX::XMFLOAT2;
	using XMFLOAT3 = DirectX::XMFLOAT3;
	using XMFLOAT4 = DirectX::XMFLOAT4;
	using XMMATRIX = DirectX::XMMATRIX;

	XMFLOAT3 velocity{ 0,0,6 };

public:
	//‰Šú‰»
	void Initialize()override;

	//XV
	void Update()override;

	//•`‰æ
	void Draw()override;

	std::unique_ptr<Camera> camera; //ƒJƒƒ‰

	//Á‚¦‚é‚Ü‚Å
	static const int32_t BulVanishTime = 60 * 5;//õ–½

	//Á–Å‚Ìé
	int32_t vanishTimer_ = BulVanishTime;

	void SetVelocity(const XMFLOAT3& velocity) { this->velocity = velocity; }

	const XMFLOAT3& GetVelocity() { return velocity; }
};
