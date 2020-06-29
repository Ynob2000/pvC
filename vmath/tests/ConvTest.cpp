#include "ConvTest.h"

void ConvTest::setUp()
{
	c2 = Vector2<char> (1, 2);
	c3 = Vector3<char> (1, 2, 3);
	c4 = Vector4<char> (0, 1, 2, 3);
	cm3.identity();
	cm4.identity();
	cq = Quaternion<char> (1, Vector3<char> (1, 2, 3));
}

void ConvTest::tearDown()
{
}

void ConvTest::testVector2()
{
	i2 = c2;
	f2 = i2;
	d2 = f2;
	CPPUNIT_ASSERT(c2 == d2);
}

void ConvTest::testVector3()
{
	i3 = c3;
	f3 = i3;
	d3 = f3;
	CPPUNIT_ASSERT(c3 == d3);
}

void ConvTest::testVector4()
{
	i4 = c4;
	f4 = i4;
	d4 = f4;
	CPPUNIT_ASSERT(c4 == d4);
}

void ConvTest::testMatrix3()
{
	im3 = cm3;
	fm3 = im3;
	dm3 = fm3;
	CPPUNIT_ASSERT(cm3 == dm3);
}

void ConvTest::testMatrix4()
{
	im4 = cm4;
	fm4 = im4;
	dm4 = fm4;
	CPPUNIT_ASSERT(cm4 == dm4);
}

void ConvTest::testQuaternion()
{
	iq = cq;
	fq = iq;
	dq = fq;
	CPPUNIT_ASSERT(cq == dq);
}

CppUnit::Test* ConvTest::suite()
{
	CppUnit::TestSuite* suite = new CppUnit::TestSuite("Conversion test");
	suite->addTest(new CppUnit::TestCaller<ConvTest>("Vector2 conversion",
			&ConvTest::testVector2));
	suite->addTest(new CppUnit::TestCaller<ConvTest>("Vector3 conversion",
			&ConvTest::testVector3));
	suite->addTest(new CppUnit::TestCaller<ConvTest>("Vector4 conversion",
			&ConvTest::testVector4));
	suite->addTest(new CppUnit::TestCaller<ConvTest>("Matrix3 conversion",
			&ConvTest::testMatrix3));
	suite->addTest(new CppUnit::TestCaller<ConvTest>("Matrix4 conversion",
			&ConvTest::testMatrix4));
	suite->addTest(new CppUnit::TestCaller<ConvTest>(
			"Quaternion conversion", &ConvTest::testQuaternion));

	return suite;
}
