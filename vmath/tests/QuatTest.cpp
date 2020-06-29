#include "QuatTest.h"

void QuatTest::setUp()
{
	v1 = Vector3f(1, 0, 0);
	r1 = Vector3f(0, 0, 1);
	v2 = Vector4f(15, 0, 0, 1);
	r2 = Vector4f(0, -15, 0, 1);

	q5 = Quatf(0.5, Vector3f(1, 2, 3));
	q5n = Quatf(-0.5, Vector3f(-1, -2, -3));
	q5c = Quatf(0.5, Vector3f(-1, -2, -3));
}

void QuatTest::tearDown()
{
}

void QuatTest::testRotAxis()
{
	Vector3f v1(1, 0, 0);
	q1 = Quatf::fromAxisRot(Vector3f(0, 1, 0), -90);
	rot = q1.rotMatrix();
	w1 = rot * v1;

	/*std::cout << "V1 = " << v1 << std::endl;
	 std::cout << "W1 = " << w1 << std::endl;
	 std::cout << "R1 = " << r1 << std::endl;
	 std::cout << "Rot = " << std::endl << rot;*/
	CPPUNIT_ASSERT(r1 == w1);
}

void QuatTest::testTransform()
{
	q2 = Quatf::fromAxisRot(Vector3f(0, 1, 0), 180);
	q3 = Quatf::fromAxisRot(Vector3f(0, 0, -1), 90);
	//q4 = q2 + q3;
	//q4.normalize();
	Matrix4f m1, m2;
	m1 = q2.transform();
	m2 = q3.transform();
	rot2 = m1 * m2;//q3.transform();//q2.transform();// * q3.transform();//q4.transform();
	w2 = Vector4f(1, 0, 0, 1);
	r2 = Vector4f(0, -1, 0, 1);
	Vector4f v2 = rot2 * w2;
	q4 = q2 * q3;
	Vector4f v3 = q4.transform() * w2;

	/*std::cout << "W2 = " << w2 << std::endl;
	 std::cout << "R2 = " << r2 << std::endl;
	 std::cout << "Rot2 = " << std::endl << rot2;
	 std::cout << "V2 = " << v2 << std::endl;
	 std::cout << "V3 = " << v3 << std::endl;*/
	CPPUNIT_ASSERT(r2 == v2);
	CPPUNIT_ASSERT(r2 == v3);
}

void QuatTest::testRotEuler()
{
	q2 = Quatf::fromEulerAngles(0, 0, -90);
	rot2 = q2.transform();
	w2 = rot2 * v2;

	/*std::cout << "V2 = " << v2 << std::endl;
	 std::cout << "W2 = " << w2 << std::endl;
	 std::cout << "R2 = " << r2 << std::endl;
	 std::cout << "Rot2 = " << std::endl << rot2;*/
	CPPUNIT_ASSERT(r2 == w2);
}

void QuatTest::testNegation()
{
	CPPUNIT_ASSERT(-q5 == q5n);
}

void QuatTest::testConjugate()
{
	CPPUNIT_ASSERT(~q5 == q5c);
}

void QuatTest::testFromMatrix()
{
	// around X
	for (float angle = 0.0; angle < 360.0; angle += 1.0)
	{
		const Vector3f axis(1.0f, 0.0f, 0.0f);
		Quatf q1 = Quatf::fromAxisRot(axis, angle);
		Matrix4f m1 = q1.transform();
		Quatf q2 = Quatf::fromMatrix(m1);

		if (!(q1 == q2 || q1 == -q2))
		{
			std::cout << "axis: " << axis << std::endl;
			printf("angle=%f -- cosa/2 %f sina/2 %f\n", angle, cos(angle / 2), sin(angle / 2));
			std::cout << m1;
			std::cout << q1 << std::endl << q2 << std::endl;
		}

		CPPUNIT_ASSERT(q1==q2 || q1==-q2);
	}
	// around Y
	for (float angle = 0.0; angle < 360.0; angle += 1.0)
	{
		const Vector3f axis(0.0f, 1.0f, 0.0f);
		Quatf q1 = Quatf::fromAxisRot(axis, angle);
		Matrix4f m1 = q1.transform();
		Quatf q2 = Quatf::fromMatrix(m1);
		if (!(q1 == q2 || q1 == -q2))
		{
			std::cout << "axis: " << axis << std::endl;
			printf("angle=%f -- cosa/2 %f sina/2 %f\n", angle, cos(angle / 2), sin(angle / 2));
			std::cout << m1;
			std::cout << q1 << std::endl << q2 << std::endl;
		}
		CPPUNIT_ASSERT(q1==q2 || q1==-q2);
	}
	// around Z
	for (float angle = 0.0; angle < 360.0; angle += 1.0)
	{
		const Vector3f axis(0.0f, 0.0f, 1.0f);
		Quatf q1 = Quatf::fromAxisRot(axis, angle);
		Matrix4f m1 = q1.transform();
		Quatf q2 = Quatf::fromMatrix(m1);
		if (!(q1 == q2 || q1 == -q2))
		{
			std::cout << "axis: " << axis << std::endl;
			printf("angle=%f -- cosa/2 %f sina/2 %f\n", angle, cos(angle / 2), sin(angle / 2));
			std::cout << m1;
			std::cout << q1 << std::endl << q2 << std::endl;
		}
		CPPUNIT_ASSERT(q1==q2 || q1==-q2);
	}

}

CppUnit::Test* QuatTest::suite()
{
	CppUnit::TestSuite* suite = new CppUnit::TestSuite("Quaternion test");
	suite->addTest(new CppUnit::TestCaller<QuatTest>(
			"Rotation around axis", &QuatTest::testRotAxis));
	suite->addTest(new CppUnit::TestCaller<QuatTest>(
			"Roation from euler angles", &QuatTest::testRotEuler));
	suite->addTest(new CppUnit::TestCaller<QuatTest>(
			"Rotation around axis using transform()",
			&QuatTest::testTransform));
	suite->addTest(new CppUnit::TestCaller<QuatTest>(
			"Negation of quaternion", &QuatTest::testNegation));
	suite->addTest(new CppUnit::TestCaller<QuatTest>(
			"Conjugation of quaternion", &QuatTest::testConjugate));
	suite->addTest(new CppUnit::TestCaller<QuatTest>(
			"Matrix decomposition", &QuatTest::testFromMatrix));

	return suite;
}
