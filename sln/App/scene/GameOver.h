#pragma once
/// <summary>
/// �^�C�g�����
/// </summary>
#include "BaseScene.h"
#include "Sprite.h"
#include "Object3d.h"
#include <memory>

class GameOver :public BaseScene
{
public:

	enum class SelectPattern {
		def,
		goTitle,
		retry,
	};
	SelectPattern selectPattern_ = SelectPattern::def;

	void Initialize() override;

	void Finalize() override;

	void Retry();

	void GoTitle();

	void Update() override;

	void Draw() override;

	void DrawUI() override;

private:

	std::unique_ptr < Sprite> gameover = nullptr;
	std::unique_ptr < Sprite> sp_gotitle = nullptr;
	std::unique_ptr < Sprite> sp_retry = nullptr;
	std::unique_ptr < Sprite> sp_Now = nullptr;

	float NowSelect = 0;//0:��������i�f�t�H�j
	void  (GameOver::* pFunc)();

	float WaitKeyEase = 0;//�C�[�Y�̃L�[�ҋ@����

	bool CursorMoveNowFlag = false;//�ړ����͉�������� false:�����ĂȂ�

	const float EaseValDef = 25;
	float EaseVal = EaseValDef;//�C�[�Y�l
	float DecEaseVal = 0.5;//�C�[�Y�l���炷�l

	bool SSOp0_1Flag = false;//1�Ɉړ��J�n�t���O
	bool SSStart1_0Flag = false;//1����0��

	//Model* model_1 = nullptr;
	//Model* model_2 = nullptr;

	//Object3d* object3d_1 = nullptr;
	//Object3d* object3d_2 = nullptr;
	//Object3d* object3d_3 = nullptr;
};
