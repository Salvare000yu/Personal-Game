﻿#include "Field.h"
#include <yaml/Yaml.hpp>

using namespace DirectX;//DirectX::省略

Field* Field::GetInstance()
{
	static Field instance;
	return &instance;
}

void Field::Initialize()
{
	//ymlデータ
	{
		Yaml::Node root;
		try
		{
			Yaml::Parse(root, "Resources/charDataFile/field.yml");
		}
		catch (...)
		{
			throw;
		}
		swingDist = root["swingDist"].As<float>();
		swingSp = root["swingSp"].As<float>();
		groundPosDef = root["groundPosDef"].As<float>();
		shiftSpeed = root["shiftSpeed"].As<float>();
	}
	//------objからモデルデータ読み込み---
	mod_groundBottom.reset(Model::LoadFromOBJ("ground_bottom"));
	obj_groundBottom.reset(Object3d::Create());
	obj_ground.emplace("ground_gre", Object3d::Create());
	obj_ground.emplace("ground_mag", Object3d::Create());
	mod_tunnel.reset(Model::LoadFromOBJ("tunnel"));
	//------3dオブジェクト生成------//
	obj_tunnel.reset(Object3d::Create());
	//------3dオブジェクトに3dモデルを紐づける------//
	obj_groundBottom->SetModel(mod_groundBottom.get());
	obj_tunnel->SetModel(mod_tunnel.get());
	//------object3dスケール------//
	constexpr float groundScale = 5000;
	obj_groundBottom->SetScale({ groundScale, groundScale, groundScale });
	obj_tunnel->SetScale({ groundScale, groundScale, groundScale * 2.f });

	//地面
	for (auto& i : obj_ground) {
		auto& model = mod_ground.emplace(i.first, Model::LoadFromOBJ(i.first)).first;
		constexpr float tilingNum = 16.f;
		model->second->SetTiling({ tilingNum, tilingNum });
		i.second->SetModel(mod_ground.at(i.first).get());
		i.second->SetScale(obj_groundBottom->GetScale());//地面下と合わせる
	}
	obj_ground.at("ground_mag")->SetPosition({ 0,-299,0 });
	//------object3d位置------//
	obj_groundBottom->SetPosition({ 0,-220,0 });
	obj_tunnel->SetPosition({ 0,40,0 });
}

void Field::GroundMove()
{
	groundMoveTime = ++groundMoveFrame / 60.f;

	float num = std::sinf((float)groundMoveTime * swingSp) * swingDist;
	XMFLOAT2 tmp{};

	//地面の数だけ
	for (auto& i : obj_ground) {
		XMFLOAT3 pos = i.second->GetPosition();
		pos.y = groundPosDef + num;//初期位置＋揺らす値
		i.second->SetPosition(pos);
		num = -num;//二枚目は逆に揺らす

		//UVシフト
		tmp = i.second->GetModel()->GetUvShift();
		tmp.y += shiftSpeed;
		i.second->GetModel()->SetUvShift(tmp);

		i.second->Update();
	}
}

void Field::TunnelMove()
{
	//UVシフト
	XMFLOAT2 tmp = mod_tunnel->GetUvShift();
	tmp.y += shiftSpeed / mod_ground.at("ground_gre")->GetTiling().y;
	mod_tunnel->SetUvShift(tmp);
}

void Field::Update()
{
	GroundMove();//地面揺らす
	TunnelMove();//トンネル動き

	obj_groundBottom->Update();
	obj_tunnel->Update();
}

void Field::Draw()
{
	//3dオブジェ描画
	for (auto& i : obj_ground) {
		i.second->Draw();
	}
	obj_groundBottom->Draw();
	obj_tunnel->Draw();
}