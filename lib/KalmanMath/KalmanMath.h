#pragma once

#ifndef KALMANMATH_H__
#define KALMANMATH_H__

#include "StateAndCovariance.h"
#include "Matrix.h"

class KalmanMath  {
public:
	clock_t time_called;
	Matrix calculateF();
	StateAndCovariance& correction(StateAndCovariance&, Matrix&, Matrix&, Matrix&);
	StateAndCovariance& prediction(StateAndCovariance&);
	void kalmanLoop(Matrix&, StateAndCovariance&, Matrix&, Matrix&);

private:
	Matrix identity_3 = Matrix::createIdentity(3);
};

#endif