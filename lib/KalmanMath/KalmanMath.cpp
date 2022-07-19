#include "Matrix.h"
#include "KalmanMath.h"
#include <ctime>
#include <cmath>
#include <Windows.h>

using std::istream; using std::ostream; using std::clock; using std::vector;
using std::pow; 

Matrix KalmanMath::calculateF() {

	clock_t time_difference = clock() - KalmanMath::time_called;
	double timeInSeconds = time_difference / (double)CLOCKS_PER_SEC;
	double halfDtSquared = (1.0 / 2.0) * (pow(timeInSeconds, 2.0));
	vector<vector<double>> matrixList = { {1, timeInSeconds, halfDtSquared}, {0, 1, timeInSeconds}, {0,0,1} };
	Matrix F = Matrix(matrixList, 3, 3);
	Sleep(3000);
	KalmanMath::time_called = clock();
	return F;

}

StateAndCovariance& KalmanMath::correction(StateAndCovariance& SC, Matrix& sensorData, Matrix& H, Matrix& R) {
	Matrix post = Matrix(3, 3);
	Matrix HTranspose = H.transpose();
	Matrix invS = ((H * SC.covariance) * HTranspose + R).inverse();
	Matrix kalmanGain = SC.covariance * HTranspose * (invS);
	SC.state = SC.state + kalmanGain * (sensorData - H * SC.state);
	post = post - (kalmanGain* H);
	Matrix postTranspose = post.transpose();
	Matrix kalmanTranspose = kalmanGain.transpose();
	SC.covariance = post * SC.covariance * postTranspose + ((kalmanGain * R) * kalmanTranspose);
	return SC;
}

StateAndCovariance& KalmanMath::prediction(StateAndCovariance& SC) {
	Matrix F = KalmanMath::calculateF();
	Matrix FTranspose = F.transpose();
	SC.state = F * SC.state;
	SC.covariance = (F * SC.covariance) * FTranspose;
	return SC;
}

void KalmanMath::kalmanIteration(Matrix& H, StateAndCovariance& SC, Matrix& sensorData, Matrix& R) {
	if (H(0, 0) == 1 && H(1, 2) == 1) {
		KalmanMath::correction(SC, sensorData, H, R);
	}
	else if (H(0, 0) == 1 || H(1, 2) == 1) {
		KalmanMath::correction(SC, sensorData, H, R);
	}
	else {
		KalmanMath::prediction(SC);
	}
	KalmanMath::time_called = clock();
}

int main() {
	
	KalmanMath km;
	StateAndCovariance SC;
	vector<vector<double>> HList = { {1,0,0}, {0,0,1} };
	Matrix sensorData = Matrix(2,1);
	Matrix H = Matrix(HList, 2, 3);
	Matrix R = Matrix(2, 2);
	std::cout << "H Matrix: " << '\n' << H << '\n';
	std::cout << "R Matrix: " << '\n' << R << '\n';
	std::cout << "sensorData Matrix: " << '\n' << sensorData << '\n';

	SC.state = Matrix(3, 1);
	SC.covariance = Matrix(3, 3);
	km.time_called = clock();
	km.kalmanIteration(H, SC, sensorData, R);
	// std::cout << SC.state << '\n';
	// std::cout << SC.covariance << '\n';

	return 0;
}

