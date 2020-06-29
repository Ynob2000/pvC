#pragma once
#include "testCommon.h"

class Vector3fTest: public CppUnit::TestCase
{
private:
	Vector3f v1, v2, v3, v4, v5, v6, v1n;

public:
	//Vector2fTest(std::string name) : CppUnit::TestCase(name){}

	void setUp();
	void tearDown();

	void testAddition();
	void testSubstraction();
	void testMultiplication();
	void testDivision();
	void testNormalize();
	void testNegation();
	static CppUnit::Test* suite();
};
