#include <matrix.h>
#include <catch.hpp>
#include <sstream>
#include <cstring>
#include <fstream>

SCENARIO("matrix init", "[init]") {
	Matrix m;
	REQUIRE(m.rows() == 0);
	REQUIRE(m.columns() == 0);
}

SCENARIO("matrix init params", "[init params]") {
	int r = 3, c = 4;
	Matrix m(r, c);
	REQUIRE(m.rows() == r);
	REQUIRE(m.columns() == c);
}

SCENARIO("matrix fill", "[fill]") {
	Matrix m;
	//m.fill("/home/travis/build/malexkiy/matrix/build/tests/a.mtx");
	
	std::ofstream f1("test.f");
	f1<<1;
	f1.close();
	//std::ifstream f("/home/travis/build/malexkiy/matrix/build/tests/a.mtx", std::ios_base::binary);
	/*int r, c, *data;

	f >> r >> c;
	f.read((char*)data, r * c * sizeof(data[0]));
	f.close();

	REQUIRE(m.rows() == r);
	REQUIRE(m.columns() == c);

	for (int i = 0; i < m.rows(); i++)
	{
		REQUIRE(memcmp(m.getRow(i + 1), data + i*c, m.columns()*sizeof(int)) == 0);
	}*/
}

SCENARIO("matrix op==", "[op==]") {
	std::stringstream ssa, ssb;
	Matrix a, b;

	ssa << "3 3 1 2 3 4 5 6 7 8 9";
	ssb << "3 3 1 2 3 4 5 6 7 8 9";

	ssa >> a;
	ssb >> b;
	REQUIRE(a.rows() == b.rows());
	REQUIRE(a.columns() == b.columns());
	
	for (int i = 0; i < a.rows(); i++)
	{
		REQUIRE(memcmp(a.getRow(i + 1), b.getRow(i + 1), a.columns()*sizeof(int)) == 0);
	}
}

SCENARIO("matrix op+", "[op+]") {
	std::stringstream ssa, ssb, ssc;
	Matrix a, b, c;

	ssa << "3 3 1 2 3 4 5 6 7 8 9";
	ssb << "3 3 10 11 12 13 14 15 16 17 18";
	ssc << "3 3 11 13 15 17 19 21 23 25 27";

	ssa >> a;
	ssb >> b;
	ssc >> c;
	REQUIRE(a + b == c);
}

SCENARIO("matrix op*", "[op*]") {
	std::stringstream ssa, ssb, ssc;
	Matrix a, b, c;

	ssa << "3 3 1 2 3 4 5 6 7 8 9";
	ssb << "3 3 10 11 12 13 14 15 16 17 18";
	ssc << "3 3 84 90 96 201 216 231 318 342 366";

	ssa >> a;
	ssb >> b;
	ssc >> c;
	REQUIRE(a.columns() == b.rows());
	REQUIRE(c.rows() == a.rows());
	REQUIRE(c.columns() == b.columns());
	REQUIRE(a * b == c);
}

SCENARIO("matrix op=", "[op=]") {
	std::stringstream ssa;
	Matrix a, b;

	ssa << "3 3 1 2 3 4 5 6 7 8 9";

	ssa >> a;
	b = a;
	REQUIRE(a.rows() == b.rows());
	REQUIRE(a.columns() == b.columns());
	REQUIRE(a == b);
}
