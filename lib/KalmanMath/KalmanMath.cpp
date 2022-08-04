#include "Matrix.h"
#include "KalmanMath.h"
#include <ctime>
#include <cmath>
#include <Arduino.h>

using std::istream; using std::ostream; using std::clock; using std::vector;
using std::pow; 

Matrix KalmanMath::calculateF() {
	clock_t time_difference = clock() - KalmanMath::time_called;
	double timeInSeconds = time_difference / (double)CLOCKS_PER_SEC;
	double halfDtSquared = (1.0 / 2.0) * (pow(timeInSeconds, 2.0));
	vector<vector<double>> matrixList = { {1, timeInSeconds, halfDtSquared}, {0, 1, timeInSeconds}, {0,0,1} };
	Matrix F = Matrix(matrixList, 3, 3);
	delay(3000);
	KalmanMath::time_called = clock();
	return F;

}

StateAndCovariance& KalmanMath::correction(StateAndCovariance& SC, Matrix& sensorData, Matrix& H) {
	vector<vector<double>> RList = { { pow(0.3,2), 0 }, { 0, pow(0.0147,2) } };
	Matrix R = Matrix(RList, 2, 2);
	Matrix post = Matrix(3, 3);
	Matrix HTranspose = H.transpose();
	Matrix invS = ((H * SC.covariance) * HTranspose + R).inverse();
	Matrix kalmanGain = SC.covariance * HTranspose * (invS);
	SC.state = SC.state + kalmanGain * (sensorData - H * SC.state);
	SC.covariance = (identity_3 - kalmanGain * H) * SC.covariance;
	return SC;
}

StateAndCovariance& KalmanMath::prediction(StateAndCovariance& SC) {
	Matrix Q = Matrix(3,3);
	Matrix F = KalmanMath::calculateF();
	Matrix FTranspose = F.transpose();
	SC.state = F * SC.state;
	SC.covariance = (F * SC.covariance) * FTranspose + Q;
	return SC;
}

StateAndCovariance& KalmanMath::kalmanIteration(StateAndCovariance& SC, Matrix& sensorData, Matrix& H) {
	if (H(0, 0) == 1 && H(1, 2) == 1) {
		KalmanMath::correction(SC, sensorData, H);
	}
	else if (H(0, 0) == 1 || H(1, 2) == 1) {
		KalmanMath::correction(SC, sensorData, H);
	}
	else {
		KalmanMath::prediction(SC);
	}
	KalmanMath::time_called = clock();

	return SC;
}

// int main() {
	
// 	KalmanMath km;
// 	StateAndCovariance SC;
// 	vector<vector<double>> HList = { {1,0,0}, {0,0,1} };
// 	Matrix sensorData = Matrix(2,1);
// 	Matrix H = Matrix(HList, 2, 3);
// 	Matrix R = Matrix(2, 2);
// 	std::cout << "H Matrix: " << '\n' << H << '\n';
// 	std::cout << "R Matrix: " << '\n' << R << '\n';
// 	std::cout << "sensorData Matrix: " << '\n' << sensorData << '\n';

// SC.state = Matrix(3, 1);
// SC.covariance = Matrix(3, 3);
// km.time_called = clock();
// km.kalmanIteration(SC, sensorData, H);
// std::cout << SC.state << '\n';
// std::cout << SC.covariance << '\n';

// 	return 0;
// }

