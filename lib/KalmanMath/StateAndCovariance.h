#ifndef STATEANDCOVARIANCE_H__
#define STATEANDCOVARIANCE_H__

#include "Matrix.h"

struct StateAndCovariance {
	Matrix state;
	Matrix covariance;
};

#endif