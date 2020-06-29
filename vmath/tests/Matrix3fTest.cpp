#include "Matrix3fTest.h"

void Matrix3fTest::setUp()
{
	static const float maData[] =
	{
		1,3,2,
		2,1,3,
		3,2,1
	};

	static const float mbData[] =
	{
		-1, -2, -3,
		 2,  3, -1,
		-3,  1,  2
	};

	static const float mabData[] =
	{
		-14, -11, -11,
		  5,   7,  12,
		  5,  -4,  -1
	};

	static const float mabInvData[] =
	{
		-41, -33,   55,
		-65, -69, -113,
		 55, 111,   43
	};

	static const float mabInvFactor = 1 / 684.0f;

	ma = maData;
	mb = mbData;
	mab = mabData;
	mabInv = mabInvData;
	mabInv = mabInv * mabInvFactor;

}

void Matrix3fTest::tearDown()
{ }

void Matrix3fTest::testMultiplication()
{
	Matrix3f rab = ma * mb;

	/*
	std::cout << "ma=\n" << ma << std::endl;
	std::cout << "mb=\n" << mb << std::endl;
	std::cout << "ma * mb=\n" << rab << std::endl;
	*/

	CPPUNIT_ASSERT(rab == mab);

}

void Matrix3fTest::testDeterminant()
{
	Matrix3f rab = ma * mb;
	CPPUNIT_ASSERT(rab.det() == -684);
}

void Matrix3fTest::testInversion()
{
	Matrix3f rabInv = mab.inverse();

	/*
	Matrix3f rab = ma * mb;
	std::cout << "ma * mb=\n" << rab << std::endl;
	std::cout << "inv(ma*mb) computed=\n" << rabInv << std::endl;
	std::cout << "inv(ma*mb) expected=\n" << mabInv << std::endl;
	*/

	CPPUNIT_ASSERT(rabInv == mabInv);
}

void Matrix3fTest::testFromColumnOrRowMajorArray()
{
	const static float mdata[] =
	{
			1,2,3,
			4,5,6,
			7,8,9
	};

	// vmath uses column major order
	// c must contain m (as described by mdata)
	Matrix3f c(mdata);
	c = c.transpose();

	Matrix3f m(Matrix3f::fromRowMajorArray(mdata));
	CPPUNIT_ASSERT(m == c);

	m = Matrix3f::fromColumnMajorArray(mdata);
	m = m.transpose();
	CPPUNIT_ASSERT(m == c);
}

CppUnit::Test* Matrix3fTest::suite()
{
	CppUnit::TestSuite* suite = new CppUnit::TestSuite("Matrix3f test");
	suite->addTest(new CppUnit::TestCaller<Matrix3fTest>("multiplication",
			&Matrix3fTest::testMultiplication));
	suite->addTest(new CppUnit::TestCaller<Matrix3fTest>("determinant",
			&Matrix3fTest::testDeterminant));
	suite->addTest(new CppUnit::TestCaller<Matrix3fTest>("inversion",
			&Matrix3fTest::testInversion));

	//testFromColumnOrRowMajorArray
	suite->addTest(new CppUnit::TestCaller<Matrix3fTest>("from column or row major array",
			&Matrix3fTest::testFromColumnOrRowMajorArray));

	return suite;
}
