
#ifndef __MATRIX_H__
#define __MATRIX_H__

#include <iostream>
#include <vector>

class Matrix {
public:
	Matrix(int, int); // Empty matrix of size a x b
	Matrix(std::vector<std::vector<double>>&, int, int);// Matrix using vector 
	Matrix();
	~Matrix();
	Matrix(const Matrix&); // Copy matrix;
	Matrix& operator=(const Matrix&);

	inline double& operator()(int x, int y) { return p[x][y]; }

	Matrix& operator+=(const Matrix&);
	Matrix& operator-=(const Matrix&);
	Matrix& operator*=(const Matrix&);
	Matrix& operator*=(double);
	Matrix& operator/=(double);
	Matrix  operator^(int);

	static Matrix createIdentity(int);

	friend std::ostream& operator<<(std::ostream&, const Matrix&);
	friend std::istream& operator>>(std::istream&, Matrix&);

	// Functions for inverse and required;
	Matrix inverse();
	static Matrix augment(Matrix, Matrix);
	Matrix gaussianEliminate();
	Matrix rowReduceFromGaussian();
	Matrix transpose(); 
	void swapRows(int, int);

private:
	int rows_, cols_;
	double** p;

	void allocSpace();
	Matrix expHelper(const Matrix&, int);
};

Matrix operator+(const Matrix&, const Matrix&);
Matrix operator-(const Matrix&, const Matrix&);
Matrix operator*(const Matrix&, const Matrix&);
Matrix operator*(const Matrix&, double);
Matrix operator*(double, const Matrix&);
Matrix operator/(const Matrix&, double);

#endif