#pragma once
#include "testCommon.h"

class Vector2fTest: public CppUnit::TestCase
{
private:
	Vector2f v1, v2, v3, v4, v5, v6, v1n;

public:
	//Vector2fTest(std::string name) : CppUnit::TestCase(name){}

	void setUp();
	void tearDown();

	void testAddition();
	void testSubtraction();
	void testMultiplication();
	void testDivision();
	void testNormalize();
	void testNegation();

	static CppUnit::Test* suite();
};
