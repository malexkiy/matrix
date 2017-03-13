#include <matrix.h>
#include <catch.hpp>
#include <sstream>
#include <cstring>
#include <fstream>
#include <iostream>

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
	
	size_t ls[] = { 3, 3 };
	size_t bs[] = { 0x0300000000000000, 0x0300000000000000 };
	int ldata[] = { 1, 2, 3, 4, 5, 6, 7, 8, 9 };
	int bdata[] = { 0x01000000, 0x02000000, 0x03000000, 0x04000000, 0x05000000, 0x06000000, 0x07000000, 0x08000000, 0x09000000 };
	const int T = 0xFFEEFFEE;
	int t = 0;
	std::ofstream otf("t.t", std::ios_base::binary);
	otf.write((const char*)&T, sizeof(T));
	otf.close();
	std::ifstream itf("t.t", std::ios_base::binary);
	itf.read((char*)&t, sizeof(t));
	itf.close();

	std::ofstream of("a.mtx", std::ios_base::binary);
	if (t == T)
	{
		of.write((const char*)(ls), 2 * sizeof(size_t));
		of.write((const char*)(ldata), 9 * sizeof(int));
		std::cerr << "little" << std::endl;
	}
	else
	{
		of.write((const char*)(bs), 2 * sizeof(size_t));
		of.write((const char*)(bdata), 9 * sizeof(int));
		std::cerr << "big" << std::endl;
	}
	of.close();
	
	m.fill("a.mtx");
	
	std::ifstream f("a.mtx", std::ios_base::binary);
	int *data;
	size_t r, c;

	f.read((char*)&r, sizeof(r));
	f.read((char*)&c, sizeof(c));
	
	data = new int[r*c]();
	
	f.read((char*)data, r * c * sizeof(data[0]));
	f.close();

	REQUIRE(m.rows() == r);
	REQUIRE(m.columns() == c);

	for (int i = 0; i < m.rows(); i++)
	{
		REQUIRE(memcmp(m.getRow(i + 1), data + i*c, m.columns()*sizeof(int)) == 0);
	}
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
