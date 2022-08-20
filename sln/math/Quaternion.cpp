//#include "Quaternion.h"
//#include "Matrix4.h"
//#include <cmath>
//
////成分を指定してクォータニオンを作成
//Quaternion quaternion(float x, float y, float z, float w)
//{
//	Quaternion result{ x,y,z,w };
//	return result;
//}
//
////任意軸まわりの回転の指定からクォータニオンを作成
//Quaternion quaternion(const Vector3& v, float angle)
//{
//	float _sin = sin(angle / 2.0f);
//	return quaternion(_sin * v.x, _sin * v.y, _sin * v.z, cos(angle / 2.0f));
//}
//
////dot関数
//float dot(const Quaternion& q1, const Quaternion& q2)
//{
//	return q1.x * q2.x + q1.y * q2.y + q1.z * q2.z + q1.w * q2.w;
//}
//
////length関数の定義
//float length(const Quaternion& q)
//{
//	return(float)sqrt(dot(q, q));
//}
//
////normalize関数の定義
//Quaternion normalize(const Quaternion& q)
//{
//	Quaternion result = q;
//	float len = length(result);
//	if (len != 0) {
//		result /= len;
//	}
//	return result;
//}
//
////共役四次元数を求める　conjugate
//Quaternion conjugate(const Quaternion& q)
//{
//	//考えて
//	//p11
//	Quaternion result= q;
//	result.x = -q.x;
//	result.y = -q.y;
//	result.z = -q.z;
//	result.w = q.w;
//
//	return result;
//}
//
//Quaternion operator +(const Quaternion& q)
//{
//	return q;
//}
//
//Quaternion operator-(const Quaternion& q)
//{
//	Quaternion result = { -q.x,-q.y,-q.z,-q.w };
//	return result;
//}
//
//Quaternion& operator +=(Quaternion& q1, const Quaternion& q2)
//{
//	q1.x += q2.x;
//	q1.y += q2.y;
//	q1.z += q2.z;
//	q1.w += q2.w;
//	return q1;
//}
//
//Quaternion& operator -=(Quaternion& q1, const Quaternion& q2)
//{
//	q1.x -= q2.x;
//	q1.y -= q2.y;
//	q1.z -= q2.z;
//	q1.w -= q2.w;
//	//考えて
//	return q1;
//}
//
//Quaternion& operator *=(Quaternion& q1, const Quaternion& q2)
//{
//	Quaternion result{
//		 q1.x*q2.w+q1.y*q2.z-q1.z*q2.y+q1.w*q2.x,//x成分
//		-q1.x*q2.z+q1.y*q2.w+q1.z*q2.x+q1.w*q2.y,//y
//		 q1.x*q2.y-q1.y*q2.x+q1.z*q2.w+q1.w*q2.z,//z
//		-q1.x*q2.x-q1.y*q2.y-q1.z*q2.z+q1.w*q2.w//w
//	};//考えて　スライドp10参考に 資料はwxyz順
//	q1 = result;
//	return q1;
//}
////代入演算子*=のオーバーロード
////クォータニオン*=スカラー
//Quaternion& operator *=(Quaternion& q1, float s)
//{
//	q1.x *= s;
//	q1.y *= s;
//	q1.z *= s;
//	q1.w *= s;
//	return q1;
//	//考えて
//}
//
////クォータニオン/=スカラー
//Quaternion& operator /=(Quaternion& q, float s)
//{
//	return q *= 1.0f / s;
//}
//
////クォータニオン＋クォータニオン
//Quaternion operator +(const Quaternion & q1, const Quaternion & q2)
//{
//	Quaternion result = q1;
//	return result += q2;
//}
//
////クォータニオン-クォータニオン
//Quaternion operator -(const Quaternion& q1, const Quaternion& q2)
//{
//	Quaternion result = q1;
//	return result -= q2;
//	//考えて
//}
//
////クォータニオン＊クォータニオン
//Quaternion operator *(const Quaternion& q1, const Quaternion& q2)
//{
//	Quaternion result = q1;
//	return result *= q2;
//}
//
////クォータニオン＊スカラー
//Quaternion operator *(const Quaternion& q, float s)
//{
//	Quaternion result = q;
//	return result *= s;
//}
//
////スカラー＊クォータニオン
//Quaternion operator *(float s, const Quaternion& q)
//{
//	Quaternion result = q;
//	return result *= s;
//}
//
////クォータニオン/スカラー
//Quaternion operator/(const Quaternion& q, float s)
//{
//	Quaternion result = q;
//	return result /= s;
//	//考えて
//}
//
////q1→q2で補間
//Quaternion slerp(const Quaternion& q1, const Quaternion& q2, float t)
//{
//	float cos = dot(q1, q2);
//	Quaternion t2 = q2;
//	if (cos < 0.0f) {
//		cos = -cos;
//		t2 = -q2;
//	}
//	float k0 = 1.0f - t;
//	float k1 = t;
//	if ((1.0f - cos) > 0.001f) {
//		float theta = (float)acos(cos);
//		k0 = (float)(sin(theta * k0) / sin(theta));
//		k1 = (float)(sin(theta * k1) / sin(theta));
//	}
//	return q1 * k0 + t2 * k1;
//}
//
////クォータニオンを線形補完する関数
//Quaternion lerp(const Quaternion& q1,
//	const Quaternion& q2, float t)
//{
//	float cos = dot(q1, q2);
//	Quaternion t2 = q2;
//	if (cos < 0.0f) {
//		cos = -cos;
//		t2 = -q2;
//	}
//	float k0 = 1.0f - t;
//	float k1 = t;
//	return q1 * k0 + t2 * k1;
//}
//
////回転軸の算出
//Vector3 getAxis(const Quaternion& q)
//{
//	Vector3 result;
//
//	float x = q.x;
//	float y = q.y;
//	float z = q.z;
//	float _len = length(q);
//
//	result.x = x / _len;
//	result.y = y / _len;
//	result.z = z / _len;
//
//	return result;
//}
//
////回転からクォータニオンを求める関数
//Quaternion quaternion(const Matrix4& m)
//{
//	Quaternion result;
//	float tr = m.m[0][0] + m.m[1][1]
//		+m.m[2][2]+ m.m[3][3];
//	if (tr >= 1.0f) {
//		float fourD = 2.0f * sqrt(tr);
//		result.x = (m.m[1][2] - m.m[2][1]) / fourD;
//		result.y = (m.m[2][0] - m.m[0][2]) / fourD;
//		result.z = (m.m[0][1] - m.m[1][0]) / fourD;
//		result.w = fourD / 4.0f;
//		return result;
//	}
//	int i = 0;
//	if (m.m[0][0] <= m.m[1][1]) {
//		i = 1;
//	}
//	if (m.m[2][2] > m.m[i][i]) {
//		i = 2;
//	}
//	int j = (i + 1) % 3;
//	int k = (j + 1) % 3;
//	tr = m.m[i][i] - m.m[j][j]
//		- m.m[k][k] + 1.0f;
//	float fourD = 2.0f * sqrt(tr);
//	float qa[4];
//	qa[i] = fourD / 4.0f;
//	qa[j] = (m.m[j][i] + m.m[i][j]) / fourD;
//	qa[k] = (m.m[k][i] + m.m[i][k]) / fourD;
//	qa[3] = (m.m[j][k] - m.m[k][j]) / fourD;
//	result.x = qa[0];
//	result.y = qa[1];
//	result.z = qa[2];
//	result.w = qa[3];
//	return result;
//}
//
////クォータニオンから回転行列に変換する関数
//Matrix4 rotate(const Quaternion& q)
//{
//	float xx = q.x * q.x * 2.0f;
//	float yy = q.y * q.y * 2.0f;
//	float zz = q.z * q.z * 2.0f;
//	float xy = q.x * q.y * 2.0f;
//	float xz = q.x * q.z * 2.0f;
//	float yz = q.y * q.z * 2.0f;
//	float wx = q.w * q.x * 2.0f;
//	float wy = q.w * q.y * 2.0f;
//	float wz = q.w * q.z * 2.0f;
//	XMMATRIX result = {
//		1.0f - yy - zz,xy + wz,xz - wy,0.0f,
//		xy - wz,1.0f - xx - zz,yz + wx,0.0f,
//		xz + wy,yz - wx,1.0f - xx - yy,0.0f,
//		0.0f,0.0f,0.0f,1.0f
//	};
//	return result;
//}
////ベクトルvをクォータニオンqで回転し、結果をクォータニオンで返す
//Quaternion quaternion(const Vector3& v, const Quaternion& q)
//{
//	Quaternion qq = conjugate(q);//qの共役四次元数を作成
//	Quaternion qv = quaternion(v.x, v.y, v.z, 0);//ベクトルvをクォータニオン化する
//
//	return q * qv * qq;//ベクトルを回転したクォータニオンを返す
//}