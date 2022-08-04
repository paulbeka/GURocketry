#pragma once

#ifndef KALMANMATH_H__
#define KALMANMATH_H__

#include "StateAndCovariance.h"
#include "Matrix.h"
#include <ctime>


class KalmanMath  {
public:
	unsigned long time_called;
	void initializeKalman();
	Matrix calculateF();
	StateAndCovariance& correction(StateAndCovariance&, Matrix&, Matrix&);
	StateAndCovariance& prediction(StateAndCovariance&);
	StateAndCovariance& kalmanIteration(StateAndCovariance&, Matrix&, Matrix&);

private:
	Matrix R; 
	Matrix Q;
	Matrix identity_3 = Matrix::createIdentity(3);
};

#endif