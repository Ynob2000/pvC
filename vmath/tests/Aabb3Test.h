#pragma once
#include "testCommon.h"

class Aabb3Test: public CppUnit::TestFixture
{
private:
public:
	void testCreation();
	void testValidity();
	void testExtension();
	void testTransformTranslation();
	void testTransformRotation();
	void testTransformScale();
	void testIntersection();
	static CppUnit::Test* suite();
};
