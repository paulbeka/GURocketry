#include <stdlib.h>
#include <stdbool.h>
#include <assert.h>
#include <math.h>
#include "fcc_kalmanAlt.h"

// Function to perform an iteration of the Kalman Filter
kalmanState kalman(kalmanState Statek_1k_1, kalmanStruct CA, kalmanNoise Noise, kalmanMeas Zk){

    kalmanState Statekk;

    double Ht[3][3], Ft[3][3];

    int i = 0, j = 0;
    for (i = 0; i < 3; i++){  
        for (j = 0; j < 3; j++){
            Ft[i][j] = CA.F[j][i];
        }
    } 

    //------------------------------------ PREDICTION STEP ------------------------------------//
    // A Priori State Estimate
    // Xkk_1 = F*Xk_1k_1 + wk
    Statekk.Xkk_1[0] = CA.F[0][0]*Statek_1k_1.Xk_1k_1[0] + CA.F[0][1]*Statek_1k_1.Xk_1k_1[1] + CA.F[0][2]*Statek_1k_1.Xk_1k_1[2] + Noise.wk[0];
    Statekk.Xkk_1[1] = CA.F[1][1]*Statek_1k_1.Xk_1k_1[0] + CA.F[1][1]*Statek_1k_1.Xk_1k_1[1] + CA.F[1][2]*Statek_1k_1.Xk_1k_1[2] + Noise.wk[1];
    Statekk.Xkk_1[2] = CA.F[2][2]*Statek_1k_1.Xk_1k_1[0] + CA.F[2][1]*Statek_1k_1.Xk_1k_1[1] + CA.F[2][2]*Statek_1k_1.Xk_1k_1[2] + Noise.wk[2];
    
    // A Priori State Covariances
    // Pkk_1 = F*Pk_1k_1*F' + Q;
    double FP[3][3];
    // Need to multiply three matrices so first multiply (FP)F'
    FP[0][0] = CA.F[0][0]*Statek_1k_1.Pk_1k_1[0][0] + CA.F[0][1]*Statek_1k_1.Pk_1k_1[1][0] + CA.F[0][2]*Statek_1k_1.Pk_1k_1[2][0];
    FP[0][1] = CA.F[0][0]*Statek_1k_1.Pk_1k_1[0][1] + CA.F[0][1]*Statek_1k_1.Pk_1k_1[1][1] + CA.F[0][2]*Statek_1k_1.Pk_1k_1[2][1];
    FP[0][2] = CA.F[0][0]*Statek_1k_1.Pk_1k_1[0][2] + CA.F[0][1]*Statek_1k_1.Pk_1k_1[1][2] + CA.F[0][2]*Statek_1k_1.Pk_1k_1[2][2];

    FP[1][0] = CA.F[1][0]*Statek_1k_1.Pk_1k_1[0][0] + CA.F[1][1]*Statek_1k_1.Pk_1k_1[1][0] + CA.F[1][2]*Statek_1k_1.Pk_1k_1[2][0];
    FP[1][1] = CA.F[1][0]*Statek_1k_1.Pk_1k_1[0][1] + CA.F[1][1]*Statek_1k_1.Pk_1k_1[1][1] + CA.F[1][2]*Statek_1k_1.Pk_1k_1[2][1];
    FP[1][2] = CA.F[1][0]*Statek_1k_1.Pk_1k_1[0][2] + CA.F[1][1]*Statek_1k_1.Pk_1k_1[1][2] + CA.F[1][2]*Statek_1k_1.Pk_1k_1[2][2];

    FP[2][0] = CA.F[2][0]*Statek_1k_1.Pk_1k_1[0][0] + CA.F[2][1]*Statek_1k_1.Pk_1k_1[1][0] + CA.F[2][2]*Statek_1k_1.Pk_1k_1[2][0];
    FP[2][1] = CA.F[2][0]*Statek_1k_1.Pk_1k_1[0][1] + CA.F[2][1]*Statek_1k_1.Pk_1k_1[1][1] + CA.F[2][2]*Statek_1k_1.Pk_1k_1[2][1];
    FP[2][2] = CA.F[2][0]*Statek_1k_1.Pk_1k_1[0][2] + CA.F[2][1]*Statek_1k_1.Pk_1k_1[1][2] + CA.F[2][2]*Statek_1k_1.Pk_1k_1[2][2];

    // Multiply Result by the Transposed State Transition Matrix and add Process Noise Covariance
    Statekk.Pkk_1[0][0] = FP[0][0]*Ft[0][0] + FP[0][1]*Ft[1][0] + FP[0][2]*Ft[2][0] + CA.Q[0][0];
    Statekk.Pkk_1[0][1] = FP[0][0]*Ft[0][1] + FP[0][1]*Ft[1][1] + FP[0][2]*Ft[2][1] + CA.Q[0][1];
    Statekk.Pkk_1[0][2] = FP[0][0]*Ft[0][2] + FP[0][1]*Ft[1][2] + FP[0][2]*Ft[2][2] + CA.Q[0][2];

    Statekk.Pkk_1[1][0] = FP[1][0]*Ft[0][0] + FP[1][1]*Ft[1][0] + FP[1][2]*Ft[2][0] + CA.Q[1][0];
    Statekk.Pkk_1[1][1] = FP[1][0]*Ft[0][1] + FP[1][1]*Ft[1][1] + FP[1][2]*Ft[2][1] + CA.Q[1][1];
    Statekk.Pkk_1[1][2] = FP[1][0]*Ft[0][2] + FP[1][1]*Ft[1][2] + FP[1][2]*Ft[2][2] + CA.Q[1][2];

    Statekk.Pkk_1[2][0] = FP[2][0]*Ft[0][0] + FP[2][1]*Ft[1][0] + FP[2][2]*Ft[2][0] + CA.Q[2][0];
    Statekk.Pkk_1[2][1] = FP[2][0]*Ft[0][1] + FP[2][1]*Ft[1][1] + FP[2][2]*Ft[2][1] + CA.Q[2][1];
    Statekk.Pkk_1[2][2] = FP[2][0]*Ft[0][2] + FP[2][1]*Ft[1][2] + FP[2][2]*Ft[2][2] + CA.Q[2][2];


    //------------------------------------ CORRECTION STEP ------------------------------------//
    // Kalman Gain [3x2] Matrix (3 States and 2 Measurement States)
    // Since H is zeros and ones, H selects elements of Pkk_1
    double PHt[3][2];
    double S[2][2];
    double invS[2][2];
    double det;

    PHt[0][0] = Statekk.Pkk_1[0][0];
    PHt[0][1] = Statekk.Pkk_1[0][2];

    PHt[1][0] = Statekk.Pkk_1[1][0];
    PHt[1][1] = Statekk.Pkk_1[1][2];

    PHt[2][0] = Statekk.Pkk_1[2][0];
    PHt[2][1] = Statekk.Pkk_1[2][2];

    // Innovation S = H*Pkk_1*H' + R [2x2] Matrix
    S[0][0] = PHt[0][0] + CA.R[0][0];
    S[0][1] = PHt[0][1] + CA.R[0][1];
    S[1][0] = PHt[2][0] + CA.R[1][0];
    S[1][1] = PHt[2][1] + CA.R[1][1];

    // Inverse S
    det = S[0][0]*S[1][1] - S[0][1]*S[1][0];
    invS[0][0] = S[1][1]/det;
    invS[0][1] = -S[0][1]/det;
    invS[1][0] = -S[1][0]/det;
    invS[1][1] = S[0][0]/det;

    // Kalman Gain
    Statekk.K[0][0] = PHt[0][0]*invS[0][0] + PHt[0][1]*invS[1][0];
    Statekk.K[0][1] = PHt[0][0]*invS[0][1] + PHt[0][1]*invS[1][1];
    Statekk.K[1][0] = PHt[1][0]*invS[0][0] + PHt[1][1]*invS[1][0];

    Statekk.K[1][1] = PHt[1][0]*invS[0][1] + PHt[1][1]*invS[1][1];
    Statekk.K[2][0] = PHt[2][0]*invS[0][0] + PHt[2][1]*invS[1][0];
    Statekk.K[2][1] = PHt[2][0]*invS[0][1] + PHt[2][1]*invS[1][1];

    printf("\n\n[%1.7f,%1.7f\n%1.7f,%1.7f\n%1.10f,%1.7f]\n",Statekk.K[0][0],Statekk.K[0][1],Statekk.K[1][0],Statekk.K[1][1],Statekk.K[2][0],Statekk.K[2][1]);

    // A Posteriori State Estimate
    Statekk.Xkk[0] = Statekk.Xkk_1[0] + Statekk.K[0][0]*(Zk.s - Statekk.Xkk_1[0]) + Statekk.K[0][1]*(Zk.a - Statekk.Xkk_1[2]);
    Statekk.Xkk[1] = Statekk.Xkk_1[1] + Statekk.K[1][0]*(Zk.s - Statekk.Xkk_1[0]) + Statekk.K[1][1]*(Zk.a - Statekk.Xkk_1[2]);
    Statekk.Xkk[2] = Statekk.Xkk_1[2] + Statekk.K[2][0]*(Zk.s - Statekk.Xkk_1[0]) + Statekk.K[2][1]*(Zk.a - Statekk.Xkk_1[2]);

    // A Posteriori State Covariances
    Statekk.Pkk[0][0] = (1 - Statekk.K[0][0])*Statekk.Pkk_1[0][0] - Statekk.K[0][1]*Statekk.Pkk_1[2][0];
    Statekk.Pkk[0][1] = (1 - Statekk.K[0][0])*Statekk.Pkk_1[0][1] - Statekk.K[0][1]*Statekk.Pkk_1[2][1];
    Statekk.Pkk[0][2] = (1 - Statekk.K[0][0])*Statekk.Pkk_1[0][2] - Statekk.K[0][1]*Statekk.Pkk_1[2][2];

    Statekk.Pkk[1][0] = -Statekk.K[1][0]*Statekk.Pkk_1[0][0] + Statekk.Pkk_1[1][0] - Statekk.K[1][1]*Statekk.Pkk_1[2][0];
    Statekk.Pkk[1][1] = -Statekk.K[1][0]*Statekk.Pkk_1[0][1] + Statekk.Pkk_1[1][1] - Statekk.K[1][1]*Statekk.Pkk_1[2][1];
    Statekk.Pkk[1][2] = -Statekk.K[1][0]*Statekk.Pkk_1[0][2] + Statekk.Pkk_1[1][2] - Statekk.K[1][1]*Statekk.Pkk_1[2][2];

    Statekk.Pkk[2][0] = -Statekk.K[2][0]*Statekk.Pkk_1[0][0] + (1 - Statekk.K[2][1])*Statekk.Pkk_1[2][0];
    Statekk.Pkk[2][1] = -Statekk.K[2][0]*Statekk.Pkk_1[0][1] + (1 - Statekk.K[2][1])*Statekk.Pkk_1[2][1];
    Statekk.Pkk[2][2] = -Statekk.K[2][0]*Statekk.Pkk_1[0][2] + (1 - Statekk.K[2][1])*Statekk.Pkk_1[2][2];

    printf("\n\n[%1.7f,%1.7f,%1.10f\n%1.7f,%1.7f,%1.7f\n%1.10f,%1.7f,%1.7f]\n",Statekk.Pkk[0][0],Statekk.Pkk[0][1],Statekk.Pkk[0][2],Statekk.Pkk[1][0],Statekk.Pkk[1][1],Statekk.Pkk[1][2],Statekk.Pkk[2][0],Statekk.Pkk[2][1],Statekk.Pkk[2][2]);

    return Statekk;
}