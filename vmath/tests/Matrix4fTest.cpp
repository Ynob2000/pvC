#include "Matrix4fTest.h"


void Matrix4fTest::setUp()
{
	vx = Vector4f(1, 0, 0, 0);
	vy = Vector4f(0, 1, 0, 0);
	vz = Vector4f(0, 0, 1, 0);
	v4 = Vector4f(-1, 0.56, 9.3, 1);
	v5 = Vector3f(v4.x, v4.y, v4.z);

}

void Matrix4fTest::tearDown()
{
}

void Matrix4fTest::testMultiplication()
{
	m2.at(1, 3) = 5;
	m2.at(3, 1) = 7;
	m2.at(2, 2) = 0;
	m2.at(0, 2) = 11;
	m2.at(2, 0) = 13;
	m2.at(3, 3) = 99;
	m2.at(3, 0) = -1;
	m2.at(0, 3) = 1;

	m3 = m1 * m2 * m1;
	/*std::cout << "M1:" << std::endl << m1;
	 std::cout << "M2:" << std::endl << m2;
	 std::cout << "M3 = M1 * M1 * M2:" << std::endl << m3;
	 std::cout << "M1 * M1:" << std::endl << m1 * m1;*/
	CPPUNIT_ASSERT(m3 == m2);
}

void Matrix4fTest::testRotation()
{
	mrx = Matrix4f::createRotationAroundAxis(-90, 0, 0);
	mry = Matrix4f::createRotationAroundAxis(0, -90, 0);
	mrz = Matrix4f::createRotationAroundAxis(0, 0, -90);

	v1 = mrx * vy;
	v2 = mry * vz;
	v3 = mrz * vx;

	//std::cout << v1 << v2 << v3 << std::endl;
	CPPUNIT_ASSERT(v1 == vz);
	CPPUNIT_ASSERT(v2 == vx);
	CPPUNIT_ASSERT(v3 == vy);
}

void Matrix4fTest::testInversion()
{
	float data1[] =
	{
		1,2,3,4,
		2,3,4,1,
		3,4,1,2,
		4,1,2,3
	};

	float data2[] =
	{
		-9,  1,  1, 11,
		 1,  1, 11, -9,
		 1, 11, -9,  1,
		11, -9,  1,  1
	};

	mi1 = Matrix4f(data1);
	mi2 = mi1.inverse();
	Matrix4f imr(data2);
	imr = imr * 1/40.0f;

	/*
	std::cout << "mi1 = \n" << mi1 << std::endl;
	std::cout << "mi2 = \n" << mi2 << std::endl;
	std::cout << "imr = \n" << imr << std::endl;
	*/
	CPPUNIT_ASSERT(mi2 == imr);

}

void Matrix4fTest::testFromColumnOrRowMajorArray()
{
	const static float mdata[] =
	{
			 1, 2, 3, 4,
			 5, 6, 7, 8,
			 9,10,11,12,
			13,14,15,16
	};

	// vmath uses column major order
	// c must contain m (as described by mdata)
	Matrix4f c(mdata);
	c = c.transpose();

	Matrix4f m(Matrix4f::fromRowMajorArray(mdata));
	CPPUNIT_ASSERT(m == c);

	m = Matrix4f::fromColumnMajorArray(mdata);
	m = m.transpose();
	CPPUNIT_ASSERT(m == c);
}

void Matrix4fTest::testOrthoProjection()
{
	Matrix4f proj(Matrix4f::createOrtho(-100, 100, -100, 100, -1, 1));
	Vector4f center(0,0,0,1);
	Vector4f minPoint(-100, -100, 1, 1);
	Vector4f maxPoint(100, 100, -1, 1);

	Vector4f projCenter = proj * center;
	Vector4f projMinPoint = proj * minPoint;
	Vector4f projMaxPoint = proj * maxPoint;

	/*
	std::cerr << projCenter << std::endl;
	std::cerr << projMinPoint << std::endl;
	std::cerr << projMaxPoint << std::endl;
	*/

	CPPUNIT_ASSERT(projCenter.x == 0 && projCenter.y == 0 && projCenter.z == 0);
	CPPUNIT_ASSERT(projMinPoint.x == -1 && projMinPoint.y == -1 && projMinPoint.z == -1);
	CPPUNIT_ASSERT(projMaxPoint.x == 1 && projMaxPoint.y == 1 && projMaxPoint.z == 1);
}

void Matrix4fTest::testFrustumProjection()
{
	Matrix4f proj(Matrix4f::createFrustum(-1,1,-1,1,0.1,100.0));
	Vector4f minPoint(0,0,-0.1, 1);
	Vector4f maxPoint(0,0,-100, 1);

	minPoint = proj * minPoint;
	minPoint /= minPoint.w;
	maxPoint = proj * maxPoint;
	maxPoint /= maxPoint.w;

	const float zEpsilon = 0.1f;

	/*
	std::cerr << minPoint << std::endl;
	std::cerr << maxPoint << std::endl;
	std::cerr << zEpsilon << std::endl;
	*/

	CPPUNIT_ASSERT(minPoint.x == 0 && minPoint.y == 0 && fabs(minPoint.z + 1) < zEpsilon);
	CPPUNIT_ASSERT(maxPoint.x == 0 && maxPoint.y == 0 && fabs(maxPoint.z - 1) < zEpsilon);
}

void Matrix4fTest::testCreateLookAt()
{
	Vector4f src(10, 5, 10, 1);
	Vector3f eye(5, 5, 10);
	Vector3f center(src.x, src.y, src.z);
	Vector3f up(0,1,0);
	Vector4f dest(0, 0, -5, 1);
	Matrix4f view(Matrix4f::createLookAt(eye, center, up));

	Vector4f result = view * src;
	/*
	std::cerr << "result:" << result << std::endl;
	std::cerr << "dest:" << dest << std::endl;
	*/
	CPPUNIT_ASSERT(result == dest);
}

CppUnit::Test* Matrix4fTest::suite()
{
	CppUnit::TestSuite* suite = new CppUnit::TestSuite("Matrix4f test");
	suite->addTest(new CppUnit::TestCaller<Matrix4fTest>("multiplication",
			&Matrix4fTest::testMultiplication));
	suite->addTest(new CppUnit::TestCaller<Matrix4fTest>("rotation",
			&Matrix4fTest::testRotation));
	suite->addTest(new CppUnit::TestCaller<Matrix4fTest>("inversion",
			&Matrix4fTest::testInversion));
	//testFromColumnOrRowMajorArray
	suite->addTest(new CppUnit::TestCaller<Matrix4fTest>("from column or row major array",
			&Matrix4fTest::testFromColumnOrRowMajorArray));
	suite->addTest(new CppUnit::TestCaller<Matrix4fTest>("ortho projection",
			&Matrix4fTest::testOrthoProjection));
	suite->addTest(new CppUnit::TestCaller<Matrix4fTest>("frustum projection",
			&Matrix4fTest::testFrustumProjection));
	suite->addTest(new CppUnit::TestCaller<Matrix4fTest>("create look at",
			&Matrix4fTest::testCreateLookAt));

	return suite;
}

