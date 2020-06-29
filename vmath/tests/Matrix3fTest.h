#pragma once
#include "testCommon.h"

class Matrix3fTest : public CppUnit::TestFixture
{
private:
	Matrix3f ma,mb,mab, mabInv;
public:
	void setUp();
	void tearDown();

	void testMultiplication();
	void testDeterminant();
	void testInversion();
	void testFromColumnOrRowMajorArray();
	static CppUnit::Test* suite();
};
