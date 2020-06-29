#include "Vector2fTest.h"

void Vector2fTest::setUp()
{
	v1 = Vector2f(1, 0.5);
	v2 = Vector2f(-1, 0.75);
	v3 = Vector2f(0, 1.25);
	v4 = Vector2f(2, -0.25);
	v5 = Vector2f(3, 1.5);
	v6 = Vector2f(0.5, 0.25);
	v1n = Vector2f(-1, -0.5);
}

void Vector2fTest::tearDown()
{
}

void Vector2fTest::testAddition()
{
	CPPUNIT_ASSERT(v1 + v2 == v3);
}

void Vector2fTest::testSubtraction()
{
	CPPUNIT_ASSERT(v1 - v2 == v4);
}

void Vector2fTest::testMultiplication()
{
	CPPUNIT_ASSERT(v1 * 3 == v5);
}

void Vector2fTest::testDivision()
{
	CPPUNIT_ASSERT(v1 / 2 == v6);
}

void Vector2fTest::testNormalize()
{
	v5.normalize();
	CPPUNIT_ASSERT(std::abs(v5.length() - 1.0) < EPSILON);

}

void Vector2fTest::testNegation()
{
	CPPUNIT_ASSERT(-v1 == v1n);
}

CppUnit::Test* Vector2fTest::suite()
{
	CppUnit::TestSuite* suite = new CppUnit::TestSuite("Vector2f test");

	suite->addTest(new CppUnit::TestCaller<Vector2fTest>("addition",
			&Vector2fTest::testAddition));
	suite->addTest(new CppUnit::TestCaller<Vector2fTest>("subtraction",
			&Vector2fTest::testSubtraction));
	suite->addTest(new CppUnit::TestCaller<Vector2fTest>("multiplication",
			&Vector2fTest::testMultiplication));
	suite->addTest(new CppUnit::TestCaller<Vector2fTest>("division",
			&Vector2fTest::testDivision));
	suite->addTest(new CppUnit::TestCaller<Vector2fTest>("normalize",
			&Vector2fTest::testNormalize));
	suite->addTest(new CppUnit::TestCaller<Vector2fTest>("negate",
			&Vector2fTest::testNegation));

	return suite;
}
