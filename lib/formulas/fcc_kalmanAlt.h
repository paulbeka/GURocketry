/* GU ROCKETRY - SALTIRE-02
 *
 *  fcc_kalmanAlt.c
 *
 *  Date: 15/03/2021
 *  Author: Joe Gibbs
 */

// DEFINE MACRO VARIABLES
#define NALT            3           // Number of States of Altitude Kalman Filter
#define MALT            2           // Number of Measured States of Altitude Kalman Filter


// Define Structs for use in Altitude Kalman Filter

typedef struct kf_matrix{

    double M[3][3];

} kf_matrix;


typedef struct kalmanStuct{

    double F[3][3];             // State Transition Matrix
    double H[2][3];             // Measurement Matrix
    double Q[3][3];            // Process Noise Covariance Matrix
    double R[2][2];            // Measurement Noise Covariance Matrix
    
} kalmanStruct;

typedef struct kalmanState{

    double Xk_1k_1[3];               // A Posteriori State Estimate at time k-1
    double Xkk_1[3];               // A Priori State Estimate at time k
    double Xkk[3];               // A Posteriori State Estimate at time k
    double Pk_1k_1[3][3];           // A Posteriori State Covariance at time k-1
    double Pkk_1[3][3];           // A Priori State Covariance at time k
    double Pkk[3][3];           // A Posteriori State Covariance at time k
    double K[3][2];           // Kalman Gain

} kalmanState;

typedef struct kalmanNoise{

    double wk[3];

} kalmanNoise;

typedef struct kalmanMeas{

    double s;
    double a;

} kalmanMeas;

typedef struct eventState{

    double Xk[3];
    double Xk_1[3];
    double Xk_2[3];
    double Xk_3[3];
    
} eventState;

kalmanState kalman(kalmanState Statek_1k_1, kalmanStruct CA, kalmanNoise Noise, kalmanMeas Zk);