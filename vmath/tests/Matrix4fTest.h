#pragma once
#include "testCommon.h"


class Matrix4fTest: public CppUnit::TestFixture
{
private:
	Matrix4f m1, m2, m3;
	Vector4f vx, vy, vz;
	Vector4f v1, v2, v3;
	Matrix4f mrx;
	Matrix4f mry;
	Matrix4f mrz;
	Vector4f v4;
	Vector3f v5;
	Matrix4f mrot;
	Matrix4f mtran;
	Vector3f v6;
	Vector4f v7;
	Matrix4f mtrans;
	Matrix4f minv;
	Matrix4f mi1, mi2;

public:

	void setUp();
	void tearDown();

	void testMultiplication();
	void testRotation();
	void testInversion();
	void testFromColumnOrRowMajorArray();
	void testOrthoProjection();
	void testFrustumProjection();
	void testCreateLookAt();
	static CppUnit::Test* suite();
};

