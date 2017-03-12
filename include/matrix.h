#pragma once

#include <istream>
#include <ostream>

class Matrix
{
private:
	size_t _rows;
	size_t _cols;
	int *_data;
	mutable int *_row;
	mutable int *_col;
	Matrix(size_t, size_t, int*);

public:
	Matrix();
	Matrix(const Matrix&);
	Matrix(size_t, size_t);
	~Matrix();

	void fill(const char* fileName);
	void print(std::ostream&) const;

	size_t rows() const;
	size_t columns() const;
	int* getRow(size_t) const;
	int* getColumn(size_t) const;

	friend std::ostream& operator<<(std::ostream&, const Matrix&);
	friend std::istream& operator>>(std::istream&, Matrix&);

	Matrix operator+(const Matrix&) const;
	Matrix operator*(const Matrix&) const;
	Matrix& operator=(const Matrix&);
	bool operator==(const Matrix&) const;
	bool operator!=(const Matrix&) const;
	int operator()(const size_t, const size_t) const;
};

