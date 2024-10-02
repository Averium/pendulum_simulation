#pragma once

#include "matrix.hpp"


template <unsigned int STATE_DIM, unsigned int INPUT_DIM>
class LQRController {

public:

    LQRController() {}
    LQRController(const Matrix2D<double, INPUT_DIM, STATE_DIM>& K): K(K) {}

    Matrix2D<double, INPUT_DIM, 1U> control(
        const Matrix2D<double, STATE_DIM, 1U>& state,
        const Matrix2D<double, STATE_DIM, 1U>& reference
    ) const {
        return K * (reference - state);
    }

private:
    Matrix2D<double, INPUT_DIM, STATE_DIM> K;
};
