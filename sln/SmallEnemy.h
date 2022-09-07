#pragma once
#include "BaseScene.h"
#include "Object3d.h"
#include "Camera.h"
#include "BaseObject.h"

#include <memory>

class SmallEnemy:public BaseObject
{
private:
	// Microsoft::WRL::πΘͺ
	template <class T> using ComPtr = Microsoft::WRL::ComPtr<T>;
	// DirectX::πΘͺ
	using XMFLOAT2 = DirectX::XMFLOAT2;
	using XMFLOAT3 = DirectX::XMFLOAT3;
	using XMFLOAT4 = DirectX::XMFLOAT4;
	using XMMATRIX = DirectX::XMMATRIX;

public:

	//ϊ»
	void Initialize()override;

	//XV
	void Update()override;

	//`ζ
	void Draw()override;

	std::unique_ptr<Camera> camera; //J

	//Α¦ιάΕ
	static const int32_t BulVanishTime = 60*6;//υ½

	//ΑΕtO
	bool isVanish_=FALSE;

	//ΑΕΜι
	int32_t vanishTimer_ = BulVanishTime;

	bool IsVanish()const { return isVanish_; }

private:
	////-----------------model
	//std::unique_ptr < Model> mod_classplayer = nullptr;//©@

	////-----------------obj
	//std::unique_ptr < Object3d> obj_classplayer = nullptr;//©@

	// Microsoft::WRL::πΘͺ
	template <class T> using ComPtr = Microsoft::WRL::ComPtr<T>;
	// DirectX::πΘͺ
	using XMFLOAT2 = DirectX::XMFLOAT2;
	using XMFLOAT3 = DirectX::XMFLOAT3;
	using XMFLOAT4 = DirectX::XMFLOAT4;
	using XMMATRIX = DirectX::XMMATRIX;

	std::unique_ptr < Model> mod_smallenemy = nullptr;

	//SEneXΜόκιΟ
	float SEneRandX = 0;
};