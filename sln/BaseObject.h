#pragma once

#include "Model.h"
#include "Object3d.h"
#include <DirectXMath.h>
#include <memory>

class BaseObject {

protected:
	std::unique_ptr<Object3d> obj;

	bool alive = true;


public:

	inline void SetAlive(bool Alive) { alive = Alive; }

	inline void SetPosition(const DirectX::XMFLOAT3& position) { obj->SetPosition(position); }
	inline const DirectX::XMFLOAT3& GetPosition() { return obj->GetPosition(); }

	inline void SetScale(const DirectX::XMFLOAT3& scale) { obj->SetScale(scale); }
	inline const DirectX::XMFLOAT3& GetScale() { return obj->GetScale(); }

	inline void SetRotation(const DirectX::XMFLOAT3& rotation) { obj->SetRotation(rotation); }
	inline const DirectX::XMFLOAT3& GetRotation() { return obj->GetRotation(); }

	//モデル初期化関数
	inline void SetModel(Model* model ) { obj->SetModel(model); }

	inline bool GetAlive() const { return alive; }

	/// <summary>
	/// 回転行列の取得
	/// </summary>
	/// <returns>回転行列</returns>
	inline const DirectX::XMMATRIX& GetMatRotation() const { return obj->GetMatRot(); }

	virtual void Initialize();

	virtual void Update();

	virtual void Draw();

};