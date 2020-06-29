#pragma once
#include "testCommon.h"

class ConvTest: public CppUnit::TestFixture
{
private:
	Vector2<char> c2;
	Vector2<int> i2;
	Vector2<float> f2;
	Vector2<double> d2;

	Vector3<char> c3;
	Vector3<int> i3;
	Vector3<float> f3;
	Vector3<double> d3;

	Vector4<char> c4;
	Vector4<int> i4;
	Vector4<float> f4;
	Vector4<double> d4;

	Matrix3<char> cm3;
	Matrix3<int> im3;
	Matrix3<float> fm3;
	Matrix3<double> dm3;

	Matrix4<char> cm4;
	Matrix4<int> im4;
	Matrix4<float> fm4;
	Matrix4<double> dm4;

	Quaternion<char> cq;
	Quaternion<int> iq;
	Quaternion<float> fq;
	Quaternion<double> dq;

public:
	void setUp();
	void tearDown();

	void testVector2();
	void testVector3();
	void testVector4();
	void testMatrix3();
	void testMatrix4();
	void testQuaternion();

	static CppUnit::Test* suite();
};
