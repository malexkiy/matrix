#include "Matrix.h"
#include <iostream>
#include <fstream>
#include <cstdlib>
using std::cout;
using std::endl;
using std::ios_base;
using std::ifstream;


Matrix::Matrix()
{
	_rows = 0;
	_cols = 0;
	_data = nullptr;
	_row = nullptr;
	_col = nullptr;
}


Matrix::Matrix(size_t rows, size_t cols)
{
	_rows = rows;
	_cols = cols;
	_data = new int[_rows*_cols]();
	_row = nullptr;
	_col = nullptr;
}


Matrix::Matrix(size_t rows, size_t cols, int *data)
{
	_rows = rows;
	_cols = cols;
	_data = data;
	_row = nullptr;
	_col = nullptr;
}


Matrix::Matrix(const Matrix& m)
{
	_rows = m._rows;
	_cols = m._cols;
	_data = new int[_rows*_cols];
	memcpy(_data, m._data, _rows * _cols * sizeof(_data[0]));
	/*for (size_t i = 0; i < _rows*_cols; i++)
		_data[i] = m._data[i];*/
	_row = nullptr;
	_col = nullptr;
}


Matrix::~Matrix()
{
	delete[] _data;
	delete[] _row;
	delete[] _col;
}


void Matrix::fill(const char* fileName)
{
	ifstream file(fileName, ios_base::binary);

	if (!file.is_open())
		throw "Failed to open file\n";

	size_t r, c, size;
	file.read((char*)&r, sizeof(r));
	file.read((char*)&c, sizeof(c));
	size = 2 * sizeof(size_t) + r * c * sizeof(_data[0]);

	file.seekg(0, ios_base::end);
	if (size != file.tellg())
		throw "Invalid file!\n";

	file.seekg(2 * sizeof(size_t));
	
	delete[] _data;
	_rows = r;
	_cols = c;
	_data = new int[_rows*_cols]();

	file.read((char*)_data, _rows * _cols * sizeof(_data[0]));

	file.close();
}


void Matrix::print(std::ostream& os) const
{
	if (_data == nullptr || _rows == 0 || _cols == 0)
		throw "Matrix has not been initialized!\n";

	os << endl;
	os << _rows << " " << _cols << endl;
	for (size_t r = 0; r < _rows; r++)
	{
		for (size_t c = 0; c < _cols; c++)
		{
			os << _data[c + r*_cols] << "\t";
		}
		os << endl;
	}
}


std::ostream& operator<<(std::ostream& os, const Matrix& m)
{
	m.print(os);
	return os;
}


std::istream& operator>>(std::istream& is, Matrix& m)
{
	size_t r, c;
	is >> r >> c;
	if (r&c)
	{
		delete[] m._data;
		m._rows = r;
		m._cols = c;
		m._data = new int[m._rows*m._cols]();
		for (size_t i = 0; i < m._rows * m._cols; i++)
		{
			is >> m._data[i];
		}
	}

	return is;
}


size_t Matrix::rows() const
{
	return _rows;
}


size_t Matrix::columns() const
{
	return _cols;
}


int* Matrix::getRow(size_t row) const
{
	if (row < 1 || row > _rows)
		throw "Row is out of range!\n";

	delete[] _row;
	_row = new int[_cols];
	memcpy(_row, _data + (row - 1)*_cols, _cols*sizeof(_data[0]));

	return _row;
}


int* Matrix::getColumn(size_t col) const
{
	if (col < 1 || col > _cols)
		throw "Column is out of range!\n";

	delete[] _col;
	_col = new int[_cols];
	for (size_t r = 0; r < _rows; r++)
	{
		_col[r] = _data[r*_cols + col];
	}

	return _col;
}


Matrix Matrix::operator+(const Matrix& m) const
{
	if (_data == nullptr || _rows == 0 || _cols == 0)
		throw "Matrix has not been initialized!\n";
	if (m._data == nullptr || m._rows == 0 || m._cols == 0)
		throw "Matrix has not been initialized!\n";
	if ((_rows != m._rows) || (_cols != m._cols))
		throw "Sizes are not equal!\n";

	int *data = new int[_rows*_cols];

	for (size_t i = 0; i < _rows * _cols; i++)
	{
		data[i] = _data[i] + m._data[i];
	}

	return Matrix(_rows, _cols, data);
}


Matrix Matrix::operator*(const Matrix& m) const
{
	if (_data == nullptr || _rows == 0 || _cols == 0)
		throw "Matrix has not been initialized!\n";
	if (m._data == nullptr || m._rows == 0 || m._cols == 0)
		throw "Matrix has not been initialized!\n";
	if (_cols != m._rows)
		throw "Matrices can not be multiplied!\n";

	int *data = new int[_rows*m._cols];
	int sum;
	int *m1_c, *m1_base, *m2_r, *m2_base, *data_out;
	
	m1_base = _data;
	data_out = data;
	for (size_t r = 0; r < _rows; r++)
	{
		m2_base = m._data;
		for (size_t c = 0; c < m._cols; c++)
		{
			sum = 0;
			m1_c = m1_base;
			m2_r = m2_base;
			for (size_t i = 0; i < _cols; i++)
			{
				sum += *m1_c * *m2_r;
				m1_c++;
				m2_r += m._cols;
			}
			*data_out++ = sum;
			m2_base++;
		}
		m1_base += _cols;
	}

	return Matrix(_rows, m._cols, data);
}


Matrix& Matrix::operator=(const Matrix& m)
{
	if (this != &m)
	{
		delete[] _data;
		_rows = m._rows;
		_cols = m._cols;
		_data = new int[_rows*_cols];
		memcpy(_data, m._data, _rows * _cols * sizeof(_data[0]));
		/*for (size_t i = 0; i < _rows*_cols; i++)
			_data[i] = m._data[i];*/
	}

	return *this;
}


bool Matrix::operator==(const Matrix& m) const
{
	for (size_t i = 0; i < _rows * _cols; i++)
	{
		if (_data[i] != m._data[i])
			return false;
	}

	return true;
}


bool Matrix::operator!=(const Matrix& m) const
{

	for (size_t i = 0; i < _rows * _cols; i++)
	{
		if (_data[i] != m._data[i])
			return true;
	}

	return false;
}


int Matrix::operator()(const size_t row, const size_t col) const
{
	if (row < 1 || row > _rows)
		throw "Row is out of range!\n";
	if (col < 1 || col > _cols)
		throw "Col is out of range!\n";

	return _data[col - 1 + (row - 1)*_cols];
}
