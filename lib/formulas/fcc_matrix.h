/* GU ROCKETRY - SALTIRE-02
 *
 *  fcc_matrix.h
 *
 *  Date: 13/04/2021
 *  Author: Joe Gibbs
 */

#ifndef MATRIX_H
#define MATRIX_H

#include <math.h>
#include <vector>


class matrix{

    public:
        matrix();

        // Constructors
        matrix(int Rows, int Cols);

        matrix(const matrix& base);

        matrix(int Rows, int Cols , float * coef);

        static matrix eye(int size);

        static matrix zeros(int rows, int cols);

        static matrix ones(int rows, int cols);

        static matrix diag(int size, float *coefs);

        // Operators
        float& operator() (int row, int col);

        float operator() (int row, int col) const;

        float& operator() (int index);

        float operator() (int index) const;

        matrix& operator = (const matrix& rightM);

        friend bool operator == (const matrix& lefM, const matrix& rightM);

        friend bool operator != (const matrix& lefM, const matrix& rightM);

        friend matrix& operator += (matrix& leftM, const matrix& rightM);

        friend matrix& operator -= (matrix& leftM, const matrix& rightM);

        friend matrix& operator *=(matrix& leftM, const matrix& rightM);

        friend matrix& operator *=(matrix& leftM, float number);

        friend matrix& operator /=(matrix& leftM, float number);

        const matrix operator -();

        friend const matrix operator +=(matrix& leftM, float number);

        friend const matrix operator -=(matrix& leftM, float number);

        friend const matrix operator +(const matrix& leftM, const matrix& rightM);

        friend const matrix operator +(const matrix& leftM, float number);

        friend const matrix operator +(float number, const matrix& leftM);

        friend const matrix operator -(const matrix& leftM, const matrix& rightM);

        friend const matrix operator -(const matrix& leftM, float number);

        friend const matrix operator -(float number, const matrix& leftM);

        friend const matrix operator *(const matrix& leftM, const matrix& rightM);

        friend const matrix operator *(const matrix& leftM, float number);

        friend const matrix operator /(const matrix& leftM, float number);

        friend const matrix operator *(float number, const matrix& leftM);

        friend matrix& operator <<(matrix& leftM, float number);

        bool isZero() const;

        bool isVector() const;

        bool isSquare() const;

        static const matrix ToPackedVector(const matrix& Mat);

        static void AddRow(matrix& Mat, int index);

        static void AddRow(matrix& Receip, const matrix& Row, int index);

        static void AddCol(matrix& Mat, int index);

        static void AddCol(matrix& Receip, const matrix& Col, int index);

        static void DeleteRow(matrix& Mat, int Row);

        static void DeleteCol(matrix& Mat, int Col);

        static const matrix ExportRow(const matrix& Mat, int row);

        static const matrix ExportCol(const matrix& Mat, int col);

        void Resize(int Rows, int Cols);

        void Clear();

        void add(int Row, int Col, float number);

        float sum() const;

        float getNumber(int Row, int Col) const;

        void getCoef(float *coef) const;

        int  getCols() const;

        int  getRows() const;

        int size();

        void print() const;

        // Linear Algebra Operations
        matrix Transpose() const;

        matrix Inv() const;

        matrix TaylorInv(int order) const;

        static float dot(const matrix& leftM, const matrix& rightM);

        float det() const;

        float trace();

        float norm() const;

        static matrix cross(const matrix& leftM, const matrix& rightM);

        //static matrix quatmul(const matrix& leftM, const matrix& rightM);

        //static matrix quatInv(const matrix& leftM);

        //static matrix quatConj(const matrix& leftM);

        // Kinematic Transformations
        static matrix RotX(float radians);

        static matrix RotY(float radians);

        static matrix RotZ( float radians );

        static matrix Rot321(float roll, float pitch, float yaw);

        static matrix Rot321(matrix euler);

        static matrix Transl( float x, float y, float z );

        static matrix quat2rot(matrix quat);

        static matrix quat2euler(matrix quat);

        static matrix euler2quat(matrix euler);

        static matrix euler2rot123(matrix euler);

        static matrix euler2rot(matrix euler);

        static matrix rot2euler(matrix rot);

        static matrix rot2quat(matrix rot);

    private:
        /** 2-D Vector Array */
        std::vector < std::vector<float> > _matrix;

        /** Number of Rows in Matrix */
        int _nRows;

        /** Number of Columns in Matrix */
        int _nCols;

        /** Last Element Row position in Matrix */
        int _pRow;

        /** Last Element Col position in Matrix */
        int _pCol;

}; // Matrix

static float NullCoef;
extern float NullCoef;

   
#endif