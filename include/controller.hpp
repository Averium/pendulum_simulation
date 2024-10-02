#pragma once

#include "matrix.hpp"


template<int STATE_DIM, int INPUT_DIM>
class LQRController {

public:

    LQRController() {}

    LQRController(
        const Matrix2D<double, STATE_DIM, STATE_DIM>& A,
        const Matrix2D<double, STATE_DIM, INPUT_DIM>& B,
        const Matrix2D<double, STATE_DIM, STATE_DIM>& Q,
        const Matrix2D<double, INPUT_DIM, INPUT_DIM>& R
    )
        : A(A), B(B), Q(Q), R(R)
    {
        AT = A.transpose();
        BT = B.transpose();

        Matrix2D<double, STATE_DIM, STATE_DIM> P = solve_DARE(A, B, Q, R);
        K = (R + BT * P * B).inverse() * (BT * P * A);
        for (int i = 0; i<6; i++) { debug("K element: ", K(i)); }
    }

    Matrix2D<double, INPUT_DIM, 1> control(const Matrix2D<double, STATE_DIM, 1>& state) {
        return -K * state;
    }

    Matrix2D<double, STATE_DIM, STATE_DIM> solve_DARE(
        const Matrix2D<double, STATE_DIM, STATE_DIM>& A,
        const Matrix2D<double, STATE_DIM, INPUT_DIM>& B,
        const Matrix2D<double, STATE_DIM, STATE_DIM>& Q,
        const Matrix2D<double, INPUT_DIM, INPUT_DIM>& R
    )
    {
        Matrix2D<double, STATE_DIM, STATE_DIM> P = Q;
        Matrix2D<double, STATE_DIM, STATE_DIM> prevP;

        const double tolerance = 1e-10;

        do {
            prevP = P;
            P = Q + AT * P * A - AT * P * B * (R + BT * P * B).inverse() * BT * P * A;
        } while ((P - prevP).norm() > tolerance);

        return P;
    }

private:
    Matrix2D<double, STATE_DIM, STATE_DIM> A;
    Matrix2D<double, STATE_DIM, INPUT_DIM> B;
    Matrix2D<double, STATE_DIM, STATE_DIM> Q;
    Matrix2D<double, INPUT_DIM, INPUT_DIM> R;
    Matrix2D<double, INPUT_DIM, STATE_DIM> K;
    Matrix2D<double, STATE_DIM, STATE_DIM> AT;
    Matrix2D<double, INPUT_DIM, STATE_DIM> BT;
};
