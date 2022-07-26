#pragma once
#include "Vector3.h"

//matrix�^��Martin4�^�Ƃ��Ĉ���
using Matrix4 = MATRIX;

//�P�ʍs��
Matrix4 identity();

//�g��k���s��
Matrix4 scale(const Vector3& s);

// ��]�s��̐ݒ�
Matrix4 rotateX(float angle);
Matrix4 rotateY(float angle);
Matrix4 rotateZ(float angle);

Matrix4 transX(float a);
Matrix4 transY(float a);
Matrix4 transZ(float a);

// ���s�ړ��s��̍쐬
Matrix4 translate(const Vector3& t);

// ���W�ϊ��i�x�N�g���ƍs��̊|���Z������j
Vector3 transform(const Vector3& v, const Matrix4& m);

// ������Z�q�I�[�o�[���[�h
Matrix4& operator *= (Matrix4& m1, const Matrix4& m2);

// 2�����Z�q�I�[�o�[���[�h
Matrix4 operator * (const Matrix4& m1, const Matrix4& m2);
Vector3 operator * (const Vector3& v, const Matrix4& m);

