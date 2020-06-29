#pragma once
#include "testCommon.h"

class MinMaxTest : public CppUnit::TestCase
{
public:
	void testMinVector2f();
	void testMinVector2d();
	void testMinVector3f();
	void testMinVector3d();
	void testMinVector4f();
	void testMinVector4d();
	void testMaxVector2f();
	void testMaxVector2d();
	void testMaxVector3f();
	void testMaxVector3d();
	void testMaxVector4f();
	void testMaxVector4d();
	static CppUnit::Test* suite();
};
