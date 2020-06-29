TESTS = testAll
TESTALL_OBJS = testMain.o vmath.o Vector2fTest.o Vector3fTest.o Matrix3fTest.o Matrix4fTest.o\
	QuatTest.o ConvTest.o MinMaxTest.o Aabb3Test.o
CPPFLAGS = -I../src -g2 -Wall
LDFLAGS = -ldl

# if this flags doesn't work, run pkg-config cppunit --cflags or pkg-config cppunit --libs 
# respectively and past outpt to CPPFLAGS or LDFLAGS respectively
CPPFLAGS += -I/opt/local/include 
LDFLAGS += -L/opt/local/lib -lcppunit 


all: $(TESTS)

testAll: $(TESTALL_OBJS)
	g++ $(LDFLAGS) -o testAll $(TESTALL_OBJS)

vmath.o: ../src/vmath.cpp ../src/vmath.h
	g++ $(CPPFLAGS) -c $< -o $@

%.o: %.cpp
	g++ $(CPPFLAGS) -c $< -o $@

clean:
	rm -f $(TESTS) $(TESTALL_OBJS)
