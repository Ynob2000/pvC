#include "Aabb3Test.h"

void Aabb3Test::testCreation()
{
	Vector3f zero(0,0,0);
	Vector3f p(4,2,1);

	Aabb3f unitCube(-0.5, -0.5, -0.5, 0.5, 0.5, 0.5);

	CPPUNIT_ASSERT_EQUAL(false, Aabb3f().valid());
	CPPUNIT_ASSERT_EQUAL(Vector3f(0,0,0), Aabb3f(Vector3f(10,3,10)).size());

	Aabb3f box(unitCube);
	CPPUNIT_ASSERT_EQUAL(unitCube, box);

	CPPUNIT_ASSERT_EQUAL(Aabb3f(0,0,0,1,1,1), Aabb3f(1,1,1,0,0,0));
}

void Aabb3Test::testValidity()
{
	CPPUNIT_ASSERT_EQUAL(true, Aabb3f(0,0,0).valid());
	CPPUNIT_ASSERT_EQUAL(true, Aabb3f(0,0,0, 1,1,1).valid());
	CPPUNIT_ASSERT_EQUAL(true, Aabb3f(0,0,0, -1,-1,-1).valid());

	Aabb3f b;
	CPPUNIT_ASSERT_EQUAL(false, b.valid());
	b.extend(Vector3f(1,2,3));
	CPPUNIT_ASSERT_EQUAL(true, b.valid());
	b.invalidate();
	CPPUNIT_ASSERT_EQUAL(false, b.valid());
	b.extend(Aabb3f(0,0,0,1,1,1));
	CPPUNIT_ASSERT_EQUAL(true, b.valid());
	b.invalidate();
	CPPUNIT_ASSERT_EQUAL(false, b.valid());
	b.extend(Aabb3f(0,0,0,0,0,0));
	CPPUNIT_ASSERT_EQUAL(true, b.valid());
}

void Aabb3Test::testExtension()
{
	Aabb3f box1(0,0,0,1,1,1);
	Aabb3f box2(0,0,0,-1,-1,-1);
	Aabb3f boxBig(-1,-1,-1,1,1,1);

	Aabb3f b;
	b.extend(box1);
	b.extend(box2);

	CPPUNIT_ASSERT_EQUAL(boxBig, b);

	b.invalidate();
	b << box1 << box2;
	CPPUNIT_ASSERT_EQUAL(boxBig, b);

	b.invalidate();
	b << Vector3f(-1,-1,-1) << Vector3f(1,1,1);
	CPPUNIT_ASSERT_EQUAL(boxBig, b);

	b.invalidate();
	b << Vector3f(-1,1,-1) << Vector3f(1,-1,1);
	CPPUNIT_ASSERT_EQUAL(boxBig, b);
}


void Aabb3Test::testTransformTranslation()
{
	Aabb3f b1(-1,-1,-1,1,1,1);
	Matrix4f t = Matrix4f::createTranslation(10,20,30);
	Aabb3f bx(9,19,29,11,21,31);
	CPPUNIT_ASSERT_EQUAL(bx, b1 * t);
}

void Aabb3Test::testTransformRotation()
{
	/*
	 * Original:
	 *           A +Y
	 *           |
	 *           *--*
	 *           |  |
	 *  -X <-----0--*-----> +X
	 *
	 * Transformed:
	 *           A +Y
	 *           |
	 *        *--*
	 *        |  |
	 * -X <---*--0--------> +X
	 */

	Matrix4f rot = Matrix4f::createRotationAroundAxis(0,0,-90);
	Aabb3f b1(0,0,0,1,1,1);
	Aabb3f b2(b1.transformed(rot));

	Aabb3f bx(0,0,0,-1,1,1);
	CPPUNIT_ASSERT_EQUAL(bx, b2);

	b2 = b1 * rot;
	CPPUNIT_ASSERT_EQUAL(bx, b2);



}

void Aabb3Test::testTransformScale()
{
	Matrix4f scale = Matrix4f::createScale(5,5,5);

	Aabb3f b1(-1,-1,-1,1,1,1);
	Aabb3f bx(-5,-5,-5,5,5,5);

	CPPUNIT_ASSERT_EQUAL(bx, b1 * scale);
}

void Aabb3Test::testIntersection()
{
	Vector3f zero(0,0,0);
	Aabb3f b1(0,0,0,1,1,1), b2(-.5,-.5,-.5,.5,.5,.5);
	Aabb3f b1x(-1.,-1.,-1.,-.1,-.1,-.1), b2x(2,2,2,3,3,3);
	Aabb3f bi(0.,0.,0.,.5,.5,.5);

	// Box vs Box
	CPPUNIT_ASSERT_EQUAL(true, b1.intersects(b2));
	CPPUNIT_ASSERT_EQUAL(true, b2.intersects(b1));

	// Box vs Point
	CPPUNIT_ASSERT_EQUAL(true, b1.intersects(zero));
	CPPUNIT_ASSERT_EQUAL(true, b2.intersects(zero));

	// Self intersection
	CPPUNIT_ASSERT_EQUAL(true, b1.intersects(b1));
	CPPUNIT_ASSERT_EQUAL(true, b2.intersects(b2));

	// Result of intersection
	CPPUNIT_ASSERT_EQUAL(bi, b1.intersection(b2));


	// Miss Box vs Box tests
	CPPUNIT_ASSERT_EQUAL(false, b1.intersects(b1x));
	CPPUNIT_ASSERT_EQUAL(false, b1x.intersects(b1));
	CPPUNIT_ASSERT_EQUAL(false, b2.intersects(b2x));
	CPPUNIT_ASSERT_EQUAL(false, b2x.intersects(b2));

	CPPUNIT_ASSERT_EQUAL(false, b1.intersection(b1x).valid());
	CPPUNIT_ASSERT_EQUAL(false, b1x.intersection(b1).valid());
	CPPUNIT_ASSERT_EQUAL(false, b2.intersection(b2x).valid());
	CPPUNIT_ASSERT_EQUAL(false, b2x.intersection(b2).valid());

	// Invalid box intersection tests
	Aabb3f binv;
	CPPUNIT_ASSERT_EQUAL(false, binv.intersects(binv));
	CPPUNIT_ASSERT_EQUAL(false, binv.intersection(binv).valid());
}


CppUnit::Test* Aabb3Test::suite()
{
	CppUnit::TestSuite* suite = new CppUnit::TestSuite("Aabb3 test");
	suite->addTest(new CppUnit::TestCaller<Aabb3Test>("Aabb3 creation",  &Aabb3Test::testCreation));
	suite->addTest(new CppUnit::TestCaller<Aabb3Test>("Aabb3 validity",  &Aabb3Test::testValidity));
	suite->addTest(new CppUnit::TestCaller<Aabb3Test>("Aabb3 extension", &Aabb3Test::testExtension));
	suite->addTest(new CppUnit::TestCaller<Aabb3Test>("Aabb3 transform translation", &Aabb3Test::testTransformTranslation));
	suite->addTest(new CppUnit::TestCaller<Aabb3Test>("Aabb3 transform rotation", &Aabb3Test::testTransformRotation));
	suite->addTest(new CppUnit::TestCaller<Aabb3Test>("Aabb3 transform scale", &Aabb3Test::testTransformScale));
	suite->addTest(new CppUnit::TestCaller<Aabb3Test>("Aabb3 intersection", &Aabb3Test::testIntersection));


	return suite;
}
