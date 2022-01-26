#include <stdexcept>
#include "Matrix.h"
#include <vector>

#define EPS 1e-10

using std::ostream; using std::istream; using std::endl;
using std::domain_error; using std::cout; using std::vector;

/* Constructors */
Matrix::Matrix(int rows, int cols) : rows_(rows), cols_(cols) {
	allocSpace();
	for (int i = 0; i < rows_; i++) {
		for (int j = 0; j < cols_; j++) {
			p[i][j] = 0;
		}
	}

}

Matrix::Matrix(std::vector<std::vector<double>>& matrixList, int rows, int cols) : rows_(rows), cols_(cols) {
	allocSpace();
	for (int i = 0; i < rows_; i++) {
		for (int j = 0; j < cols_; j++) {
			p[i][j] = matrixList[i][j];
		}
	}
}

Matrix::Matrix() : rows_(1), cols_(1) {
	allocSpace();
	p[0][0] = 0;
}

Matrix::~Matrix() {
	for (int i = 0; i < rows_; i++) {
		delete[] p[i];
	}
	delete[] p;
}

Matrix::Matrix(const Matrix& copyMatrix) : rows_(copyMatrix.rows_), cols_(copyMatrix.cols_) {
	allocSpace();
	for (int i = 0; i < rows_; i++) {
		for (int j = 0; j < cols_; j++) {
			p[i][j] = copyMatrix.p[i][j];
		}
	}
}


/* Public Member Functions */
Matrix& Matrix::operator=(const Matrix& copyMatrix) {
	if (this == &copyMatrix) {
		return *this;
	}

	if (rows_ != copyMatrix.rows_ || cols_ != copyMatrix.cols_) {
		for (int i = 0; i < rows_; i++) {
			delete[] p[i];
		}
		delete[] p;

		rows_ = copyMatrix.rows_;
		cols_ = copyMatrix.cols_;
		allocSpace();
	}

	for (int i = 0; i < rows_; i++) {
		for (int j = 0; j < cols_; j++) {
			p[i][j] = copyMatrix.p[i][j];
		}
	}
	return *this;
}

Matrix& Matrix::operator+=(const Matrix& addingMatrix) {
	if (rows_ != addingMatrix.rows_ || cols_ != addingMatrix.cols_) {
		throw std::invalid_argument("Matrix must be the same size");
	}
	for (int i = 0; i < rows_; i++) {
		for (int j = 0; j < cols_; j++) {
			p[i][j] += addingMatrix.p[i][j];
		}
	}
	return *this;
}

Matrix& Matrix::operator-=(const Matrix& minusMatrix) {
	if (rows_ != minusMatrix.rows_ || cols_ != minusMatrix.cols_) {
		throw std::invalid_argument("Matrix must be the same size");
	}
	for (int i = 0; i < rows_; i++) {
		for (int j = 0; j < cols_; j++) {
			p[i][j] -= minusMatrix.p[i][j];
		}
	}
	return *this;
}

Matrix& Matrix::operator*=(const Matrix& multiplyMatrix) {
	bool bothMatch = rows_ == multiplyMatrix.rows_ && cols_ == multiplyMatrix.cols_; 
	if (rows_ != multiplyMatrix.cols_ && !bothMatch) {
		throw std::invalid_argument("Rows from first matrix must match column from the second or dimensions match");
	}
	Matrix temp(rows_, multiplyMatrix.cols_);
	for (int i = 0; i < temp.rows_; i++) {
		for (int j = 0; j < temp.cols_; j++) {
			for (int k = 0; k < cols_; k++) {
				temp.p[i][j] += (p[i][k] * multiplyMatrix.p[k][j]);
			}
		}
	}
	return (*this = temp);
}

Matrix& Matrix::operator*=(double num) {
	for (int i = 0; i < rows_; i++) {
		for (int j = 0; j < cols_; j++) {
			p[i][j] *= num;
		}
	}
	return *this;
}

Matrix& Matrix::operator/=(double num) {
	for (int i = 0; i < rows_; i++) {
		for (int j = 0; j < cols_; j++) {
			p[i][j] /= num;
		}
	}
	return *this;
}

Matrix Matrix::operator^(int num) {
	if (rows_ != cols_) {
		throw std::invalid_argument("Matrix must be square for a power to be raised");
	}
	Matrix temp(*this);
	return expHelper(temp, num);
}

void Matrix::swapRows(int r1, int r2) {
	double* temp = p[r1];
	p[r1] = p[r2];
	p[r2] = temp;
}

/* Static Functions */
Matrix Matrix::createIdentity(int size) {
	Matrix temp(size, size);
	for (int i = 0; i < temp.rows_; ++i) {
		for (int j = 0; j < temp.cols_; ++j) {
			if (i == j) {
				temp.p[i][j] = 1;
			}
			else {
				temp.p[i][j] = 0;
			}
		}
	}
	return temp;
}

Matrix Matrix::augment(Matrix A, Matrix B) {
	Matrix AB(A.rows_, A.cols_ + B.cols_);
	for (int i = 0; i < AB.rows_; ++i) {
		for (int j = 0; j < AB.cols_; ++j) {
			if (j < A.cols_)
				AB(i, j) = A(i, j);
			else
				AB(i, j) = B(i, j - B.cols_);
		}
	}
	return AB;
}

Matrix Matrix::gaussianEliminate() {
	Matrix Ab(*this);
	int rows = Ab.rows_;
	int cols = Ab.cols_;
	int Acols = cols - 1;

	int i = 0; // row tracker
	int j = 0; // column tracker

	// iterate through the rows
	while (i < rows) {
		// find a pivot for the row
		bool pivot_found = false;
		while (j < Acols && !pivot_found) {
			if (Ab(i, j) != 0) { // pivot not equal to 0
				pivot_found = true;
			}
			else { // check for a possible swap
				int max_row = i;
				double max_val = 0;
				for (int k = i + 1; k < rows; ++k) {
					double cur_abs = Ab(k, j) >= 0 ? Ab(k, j) : -1 * Ab(k, j);
					if (cur_abs > max_val) {
						max_row = k;
						max_val = cur_abs;
					}
				}
				if (max_row != i) {
					Ab.swapRows(max_row, i);
					pivot_found = true;
				}
				else {
					j++;
				}
			}
		}

		// perform elimination as normal if pivot was found
		if (pivot_found) {
			for (int t = i + 1; t < rows; ++t) {
				for (int s = j + 1; s < cols; ++s) {
					Ab(t, s) = Ab(t, s) - Ab(i, s) * (Ab(t, j) / Ab(i, j));
					if (Ab(t, s) < EPS && Ab(t, s) > -1 * EPS)
						Ab(t, s) = 0;
				}
				Ab(t, j) = 0;
			}
		}

		i++;
		j++;
	}
	return Ab;
}

Matrix Matrix::rowReduceFromGaussian() {
	Matrix R(*this);
	int rows = R.rows_;
	int cols = R.cols_;

	int i = rows - 1; // row tracker
	int j = cols - 2; // column tracker

	// iterate through every row
	while (i >= 0) {
		// find the pivot column
		int k = j - 1;
		while (k >= 0) {
			if (R(i, k) != 0)
				j = k;
			k--;
		}

		// zero out elements above pivots if pivot not 0
		if (R(i, j) != 0) {

			for (int t = i - 1; t >= 0; --t) {
				for (int s = 0; s < cols; ++s) {
					if (s != j) {
						R(t, s) = R(t, s) - R(i, s) * (R(t, j) / R(i, j));
						if (R(t, s) < EPS && R(t, s) > -1 * EPS)
							R(t, s) = 0;
					}
				}
				R(t, j) = 0;
			}

			// divide row by pivot
			for (int k = j + 1; k < cols; ++k) {
				R(i, k) = R(i, k) / R(i, j);
				if (R(i, k) < EPS && R(i, k) > -1 * EPS)
					R(i, k) = 0;
			}
			R(i, j) = 1;
		}
		i--;
		j--;
	}
	return R;
}

Matrix Matrix::transpose() {
	Matrix ret(cols_, rows_);

	for (int i = 0; i < rows_; i++) {
		for (int j = 0; j < cols_; i++) {
			ret.p[i][j] = p[j][i];
		}
	}
	return ret;
}

Matrix Matrix::inverse() {
	Matrix I = Matrix::createIdentity(rows_);
	Matrix AI = Matrix::augment(*this, I);
	Matrix U = AI.gaussianEliminate();
	Matrix IAInverse = U.rowReduceFromGaussian();
	Matrix AInverse(rows_, cols_);
	for (int i = 0; i < AInverse.rows_; i++) {
		for (int j = 0; j < AInverse.cols_; j++) {
			AInverse(i, j) = IAInverse(i, j + cols_);
		}
	}
	return AInverse;
}


/* Private functions */
void Matrix::allocSpace() {
	p = new double* [rows_];
	for (int i = 0; i < rows_; ++i) {
		p[i] = new double[cols_];
	}
}

// Recursive matrix power 

Matrix Matrix::expHelper(const Matrix& m, int num) {
	if (num == 0) {
		return createIdentity(m.rows_);
	}
	else if (num == 1) {
		return m;
	}
	else if (num % 2 == 0) {  // num is even
		return expHelper(m * m, num / 2);
	}
	else {   // num is odd
		return m * expHelper(m * m, (num - 1) / 2);
	}
}

// Non-Member functions
Matrix operator+(const Matrix& m1, const Matrix& m2) {
	Matrix temp(m1);
	return (temp += m2);
}

Matrix operator-(const Matrix& m1, const Matrix& m2) {
	Matrix temp(m1);
	return (temp -= m2);
}

Matrix operator*(const Matrix& m1, const Matrix& m2) {
	Matrix temp(m1);
	return (temp *= m2);
}

Matrix operator*(const Matrix& m, double num) {
	Matrix temp(m);
	return (temp *= num);
}

Matrix operator*(double num, const Matrix& m) {
	return (m * num);
}

Matrix operator/(const Matrix& m, double num) {
	Matrix temp(m);
	return (temp /= num);
}

// Output function
ostream& operator<<(ostream& os, const Matrix& m) {
	for (int i = 0; i < m.rows_; ++i) {
		os << m.p[i][0];
		for (int j = 1; j < m.cols_; ++j) {
			os << " " << m.p[i][j];
		}
		os << endl;
	}
	return os;
}

// Input function
istream& operator>>(istream& is, Matrix& m) {
	for (int i = 0; i < m.rows_; ++i) {
		for (int j = 0; j < m.cols_; ++j) {
			is >> m.p[i][j];
		}
	}
	return is;
}
