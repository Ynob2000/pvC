////////////////////////////////////////////////////////////////////////////////
//
// VMath library testing suite
//
////////////////////////////////////////////////////////////////////////////////
#include "testCommon.h"
#include <cppunit/ui/text/TestRunner.h>
#include "Vector2fTest.h"
#include "Vector3fTest.h"
#include "Matrix3fTest.h"
#include "Matrix4fTest.h"
#include "QuatTest.h"
#include "ConvTest.h"
#include "MinMaxTest.h"
#include "Aabb3Test.h"


int main(int argc, char** argv)
{

	CppUnit::TextUi::TestRunner runner;
	runner.addTest(Vector2fTest::suite());
	runner.addTest(Vector3fTest::suite());
	runner.addTest(Matrix3fTest::suite());
	runner.addTest(Matrix4fTest::suite());
	runner.addTest(QuatTest::suite());
	runner.addTest(ConvTest::suite());
	runner.addTest(MinMaxTest::suite());
	runner.addTest(Aabb3Test::suite());
	runner.run();

	return 0;
}

