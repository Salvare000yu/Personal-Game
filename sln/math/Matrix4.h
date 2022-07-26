#pragma once
#include "Vector3.h"

//matrix型をMartin4型として扱う
using Matrix4 = MATRIX;

//単位行列
Matrix4 identity();

//拡大縮小行列
Matrix4 scale(const Vector3& s);

// 回転行列の設定
Matrix4 rotateX(float angle);
Matrix4 rotateY(float angle);
Matrix4 rotateZ(float angle);

Matrix4 transX(float a);
Matrix4 transY(float a);
Matrix4 transZ(float a);

// 平行移動行列の作成
Matrix4 translate(const Vector3& t);

// 座標変換（ベクトルと行列の掛け算をする）
Vector3 transform(const Vector3& v, const Matrix4& m);

// 代入演算子オーバーロード
Matrix4& operator *= (Matrix4& m1, const Matrix4& m2);

// 2項演算子オーバーロード
Matrix4 operator * (const Matrix4& m1, const Matrix4& m2);
Vector3 operator * (const Vector3& v, const Matrix4& m);

