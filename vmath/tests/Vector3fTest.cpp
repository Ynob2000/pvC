#include "Vector3fTest.h"

void Vector3fTest::setUp()
{
	v1 = Vector3f(1, 0.5, 1);
	v2 = Vector3f(-1, 0.75, -1);
	v3 = Vector3f(0, 1.25, 0);
	v4 = Vector3f(2, -0.25, 2);
	v5 = Vector3f(3, 1.5, 3);
	v6 = Vector3f(0.5, 0.25, 0.5);
	v1n = Vector3f(-1, -0.5, -1);
}

void Vector3fTest::tearDown()
{
}

void Vector3fTest::testAddition()
{
	CPPUNIT_ASSERT(v1 + v2 == v3);
}

void Vector3fTest::testSubstraction()
{
	CPPUNIT_ASSERT(v1 - v2 == v4);
}

void Vector3fTest::testMultiplication()
{
	CPPUNIT_ASSERT(v1 * 3 == v5);
}

void Vector3fTest::testDivision()
{
	CPPUNIT_ASSERT(v1 / 2 == v6);
}

void Vector3fTest::testNormalize()
{
	v5.normalize();
	CPPUNIT_ASSERT(std::abs(v5.length() - 1.0) < EPSILON);

}

void Vector3fTest::testNegation()
{
	CPPUNIT_ASSERT(-v1 == v1n);
}

CppUnit::Test* Vector3fTest::suite()
{
	CppUnit::TestSuite* suite = new CppUnit::TestSuite("Vector2f test");

	suite->addTest(new CppUnit::TestCaller<Vector3fTest>("addition",
			&Vector3fTest::testAddition));
	suite->addTest(new CppUnit::TestCaller<Vector3fTest>("substraction",
			&Vector3fTest::testSubstraction));
	suite->addTest(new CppUnit::TestCaller<Vector3fTest>("multiplication",
			&Vector3fTest::testMultiplication));
	suite->addTest(new CppUnit::TestCaller<Vector3fTest>("division",
			&Vector3fTest::testDivision));
	suite->addTest(new CppUnit::TestCaller<Vector3fTest>("normalize",
			&Vector3fTest::testNormalize));
	suite->addTest(new CppUnit::TestCaller<Vector3fTest>("negate",
			&Vector3fTest::testNegation));

	return suite;
}
