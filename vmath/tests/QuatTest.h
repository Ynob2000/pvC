#pragma once
#include "testCommon.h"

class QuatTest: public CppUnit::TestFixture
{
private:
	Vector3f v1;
	Quatf q1;
	Matrix3f rot;
	Vector3f w1;
	Vector3f r1;
	Vector4f v2;
	Quatf q2, q3, q4;
	Matrix4f rot2;
	Vector4f w2;
	Vector4f r2;
	Quatf q5;
	Quatf q5n;
	Quatf q5c;

public:

	void setUp();
	void tearDown();

	void testRotAxis();
	void testTransform();
	void testRotEuler();
	void testNegation();
	void testConjugate();
	void testFromMatrix();

	static CppUnit::Test* suite();
};
