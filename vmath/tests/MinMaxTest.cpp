#include "MinMaxTest.h"

void MinMaxTest::testMinVector2f()
{
	Vector2f fmin(1,2);
	Vector2f fmax = fmin * 2;

	CPPUNIT_ASSERT(std::min(fmin, fmax) == fmin);
	CPPUNIT_ASSERT(std::min(fmax, fmin) == fmin);
	CPPUNIT_ASSERT(std::min(-fmax, -fmin) == -fmax);
}

void MinMaxTest::testMinVector2d()
{
	Vector2d dmin(1,2);
	Vector2d dmax = dmin * 2;

	CPPUNIT_ASSERT(std::min(dmin, dmax) == dmin);
	CPPUNIT_ASSERT(std::min(dmax, dmin) == dmin);
	CPPUNIT_ASSERT(std::min(-dmax, -dmin) == -dmax);
}

void MinMaxTest::testMinVector3f()
{
	Vector3f fmin(1,2,4);
	Vector3f fmax = fmin * 2;

	CPPUNIT_ASSERT(std::min(fmin, fmax) == fmin);
	CPPUNIT_ASSERT(std::min(fmax, fmin) == fmin);
	CPPUNIT_ASSERT(std::min(-fmax, -fmin) == -fmax);
}

void MinMaxTest::testMinVector3d()
{
	Vector3d dmin(1,2,3);
	Vector3d dmax = dmin * 2;

	CPPUNIT_ASSERT(std::min(dmin, dmax) == dmin);
	CPPUNIT_ASSERT(std::min(dmax, dmin) == dmin);
	CPPUNIT_ASSERT(std::min(-dmax, -dmin) == -dmax);
}

void MinMaxTest::testMinVector4f()
{
	Vector4f fmin(1,2,4,5);
	Vector4f fmax = fmin * 2;

	CPPUNIT_ASSERT(std::min(fmin, fmax) == fmin);
	CPPUNIT_ASSERT(std::min(fmax, fmin) == fmin);
	CPPUNIT_ASSERT(std::min(-fmax, -fmin) == -fmax);
}

void MinMaxTest::testMinVector4d()
{
	Vector4d dmin(1,2,3,7);
	Vector4d dmax = dmin * 2;

	CPPUNIT_ASSERT(std::min(dmin, dmax) == dmin);
	CPPUNIT_ASSERT(std::min(dmax, dmin) == dmin);
	CPPUNIT_ASSERT(std::min(-dmax, -dmin) == -dmax);
}

void MinMaxTest::testMaxVector2f()
{
	Vector2f fmin(1,2);
	Vector2f fmax = fmin * 2;

	CPPUNIT_ASSERT(std::max(fmin, fmax) == fmax);
	CPPUNIT_ASSERT(std::max(fmax, fmin) == fmax);
	CPPUNIT_ASSERT(std::max(-fmax, -fmin) == -fmin);
}

void MinMaxTest::testMaxVector2d()
{
	Vector2d dmin(1,2);
	Vector2d dmax = dmin * 2;

	CPPUNIT_ASSERT(std::max(dmin, dmax) == dmax);
	CPPUNIT_ASSERT(std::max(dmax, dmin) == dmax);
	CPPUNIT_ASSERT(std::max(-dmax, -dmin) == -dmin);
}

void MinMaxTest::testMaxVector3f()
{
	Vector3f fmin(1,2,4);
	Vector3f fmax = fmin * 2;

	CPPUNIT_ASSERT(std::max(fmin, fmax) == fmax);
	CPPUNIT_ASSERT(std::max(fmax, fmin) == fmax);
	CPPUNIT_ASSERT(std::max(-fmax, -fmin) == -fmin);
}

void MinMaxTest::testMaxVector3d()
{
	Vector3d dmin(1,2,3);
	Vector3d dmax = dmin * 2;

	CPPUNIT_ASSERT(std::max(dmin, dmax) == dmax);
	CPPUNIT_ASSERT(std::max(dmax, dmin) == dmax);
	CPPUNIT_ASSERT(std::max(-dmax, -dmin) == -dmin);
}

void MinMaxTest::testMaxVector4f()
{
	Vector4f fmin(1,2,4,5);
	Vector4f fmax = fmin * 2;

	CPPUNIT_ASSERT(std::max(fmin, fmax) == fmax);
	CPPUNIT_ASSERT(std::max(fmax, fmin) == fmax);
	CPPUNIT_ASSERT(std::max(-fmax, -fmin) == -fmin);
}

void MinMaxTest::testMaxVector4d()
{
	Vector4d dmin(1,2,3,7);
	Vector4d dmax = dmin * 2;

	CPPUNIT_ASSERT(std::max(dmin, dmax) == dmax);
	CPPUNIT_ASSERT(std::max(dmax, dmin) == dmax);
	CPPUNIT_ASSERT(std::max(-dmax, -dmin) == -dmin);
}


CppUnit::Test* MinMaxTest::suite()
{
	CppUnit::TestSuite* suite = new CppUnit::TestSuite("Min-Max test");
	suite->addTest(new CppUnit::TestCaller<MinMaxTest>("Vector2f Min", &MinMaxTest::testMinVector2f));
	suite->addTest(new CppUnit::TestCaller<MinMaxTest>("Vector2d Min", &MinMaxTest::testMinVector2d));
	suite->addTest(new CppUnit::TestCaller<MinMaxTest>("Vector3f Min", &MinMaxTest::testMinVector3f));
	suite->addTest(new CppUnit::TestCaller<MinMaxTest>("Vector3d Min", &MinMaxTest::testMinVector3d));
	suite->addTest(new CppUnit::TestCaller<MinMaxTest>("Vector4f Min", &MinMaxTest::testMinVector4f));
	suite->addTest(new CppUnit::TestCaller<MinMaxTest>("Vector4d Min", &MinMaxTest::testMinVector4d));

	suite->addTest(new CppUnit::TestCaller<MinMaxTest>("Vector2f Max", &MinMaxTest::testMaxVector2f));
	suite->addTest(new CppUnit::TestCaller<MinMaxTest>("Vector2d Max", &MinMaxTest::testMaxVector2d));
	suite->addTest(new CppUnit::TestCaller<MinMaxTest>("Vector3f Max", &MinMaxTest::testMaxVector3f));
	suite->addTest(new CppUnit::TestCaller<MinMaxTest>("Vector3d Max", &MinMaxTest::testMaxVector3d));
	suite->addTest(new CppUnit::TestCaller<MinMaxTest>("Vector4f Max", &MinMaxTest::testMaxVector4f));
	suite->addTest(new CppUnit::TestCaller<MinMaxTest>("Vector4d Max", &MinMaxTest::testMaxVector4d));

	return suite;
}

