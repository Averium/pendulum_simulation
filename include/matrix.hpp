#pragma once

#include <array>
#include <iostream>
#include <string>
#include <cassert>

#include "const.hpp"


template <typename TYPE, unsigned int ROWS, unsigned int COLS>
class Matrix2D {

public:
    Matrix2D() { data.fill(static_cast<TYPE>(0)); }
    Matrix2D(const std::array<TYPE, ROWS * COLS>& init) { data = init; }

    static Matrix2D<TYPE, ROWS, COLS> identity(void) {
        Matrix2D<TYPE, ROWS, COLS> result;
        for (unsigned int row = 0U; row < ROWS; ++row) {
            for (unsigned int col = 0U; col < COLS; ++col) {
                result(row, col) = (row == col) ? static_cast<TYPE>(1) : static_cast<TYPE>(0);
            }
        }
        return result;
    }

    std::array<TYPE, ROWS * COLS> get_array(void) const {
        return data;
    }

    TYPE& operator() (const unsigned int index) {
        assert(index < ROWS * COLS);
        return data[index];
    }

    TYPE& operator() (const unsigned int row, const unsigned int col) {
        assert(row < ROWS && col < COLS);
        return data[row * COLS + col];
    }

    const TYPE& operator() (const unsigned int index) const {
        assert(index < ROWS * COLS);
        return data[index];
    }

    const TYPE& operator() (const unsigned int row, const unsigned int col) const {
        assert(row < ROWS && col < COLS);
        return data[row * COLS + col];
    }

    template <typename TYPE_U>
    Matrix2D<TYPE, ROWS, COLS> operator + (const Matrix2D<TYPE_U, ROWS, COLS>& other) const {
        Matrix2D<TYPE, ROWS, COLS> result;
        for (unsigned int index = 0U; index < COLS * ROWS; ++index) {
            result(index) = data[index] + other(index);
        }
        return result;
    }

    template <typename TYPE_U>
    void operator += (const Matrix2D<TYPE_U, ROWS, COLS>& other) {
        Matrix2D<TYPE, ROWS, COLS> result;
        for (unsigned int index = 0U; index < COLS * ROWS; ++index) {
            data[index] = data[index] + other(index);
        }
    }

    template <typename TYPE_U>
    Matrix2D<TYPE, ROWS, COLS> operator - (const Matrix2D<TYPE_U, ROWS, COLS>& other) const {
        Matrix2D<TYPE, ROWS, COLS> result;
        for (unsigned int index = 0U; index < COLS * ROWS; ++index) {
            result(index) = data[index] - other(index);
        }
        return result;
    }

    template <typename TYPE_U, unsigned int COLS_U>
    Matrix2D<TYPE, ROWS, COLS_U> operator * (const Matrix2D<TYPE_U, COLS, COLS_U>& other) const {
        Matrix2D<TYPE, ROWS, COLS_U> result;

        for (unsigned int row = 0U; row < ROWS; ++row) {
            for (unsigned int col_u_index = 0U; col_u_index < COLS_U; ++col_u_index) {
                TYPE element = static_cast<TYPE>(0);
                for (unsigned int col = 0U; col < COLS; ++col) {
                    element += (*this)(row, col) * other(col, col_u_index);
                }
                result(row, col_u_index) = element;
            }
        }

        return result;
    }

    template <typename TYPE_U>
    Matrix2D<TYPE, ROWS, COLS> operator * (const TYPE_U number) const {
        Matrix2D<TYPE, ROWS, COLS> result;
        for (unsigned int element = 0U; element < ROWS * COLS; ++element) {
            result(element) = static_cast<TYPE>(data[element] * number);
        }
        return result;
    }

    template <typename TYPE_U>
    Matrix2D<TYPE, ROWS, COLS> operator / (const TYPE_U number) const {
        Matrix2D<TYPE, ROWS, COLS> result;
        for (unsigned int element = 0U; element < ROWS * COLS; ++element) {
            result(element) = static_cast<TYPE>(data[element] / number);
        }
        return result;
    }

    template <typename TYPE_U>
    Matrix2D<TYPE, ROWS, COLS> operator + (const TYPE_U number) const {
        Matrix2D<TYPE, ROWS, COLS> result;
        for (unsigned int element = 0U; element < ROWS * COLS; ++element) {
            data[element] = static_cast<TYPE>(data[element] + number);
        }
        return result;
    }

    template <typename TYPE_U>
    Matrix2D<TYPE, ROWS, COLS> operator - (const TYPE_U number) const {
        Matrix2D<TYPE, ROWS, COLS> result;
        for (unsigned int element = 0U; element < ROWS * COLS; ++element) {
            data[element] = static_cast<TYPE>(data[element] - number);
        }
        return result;
    }

    template <unsigned int COLS_U>
    Matrix2D<TYPE, ROWS, COLS + COLS_U> concath(const Matrix2D<TYPE, ROWS, COLS_U>& other) const {
        Matrix2D<TYPE, ROWS, COLS + COLS_U> result;
        for (unsigned int row = 0U; row < ROWS; ++row) {
            for (unsigned int col = 0U; col < COLS + COLS_U; ++col) {
                if (col < COLS) { result(row, col) = (*this)(row, col);      }
                else {            result(row, col) = other(row, col - COLS); }
            }
        }
        return result;
    }

    template <unsigned int ROWS_U>
    Matrix2D<TYPE, ROWS + ROWS_U, COLS> concatv(const Matrix2D<TYPE, ROWS_U, COLS>& other) const {
        Matrix2D<TYPE, ROWS + ROWS_U, COLS> result;
        for (unsigned int row = 0U; row < ROWS + ROWS_U; ++row) {
            for (unsigned int col = 0U; col < COLS; ++col) {
                if (row < ROWS) { result(row, col) = (*this)(row, col);      }
                else {            result(row, col) = other(row - ROWS, col); }
            }
        }
        return result;
    }

    Matrix2D<TYPE, ROWS, COLS * 2> concath(void) const { return concath(*this); }

    Matrix2D<TYPE, ROWS * 2, COLS> concatv(void) const { return concatv(*this); }

    Matrix2D<TYPE, COLS, ROWS> transpose(void) const {
        Matrix2D<TYPE, COLS, ROWS> transposed;

        for (unsigned int row = 0U; row < ROWS; ++row) {
            for (unsigned int col = 0U; col < COLS; ++col) {
                transposed(col, row) = (*this)(row, col);
            }
        }
        return transposed;
    }

    Matrix2D<TYPE, ROWS, COLS> inverse(void) const {
        assert(ROWS == COLS);

        const unsigned int N = ROWS;

        // Augmented matrix [A | I]
        Matrix2D<TYPE, N, 2 * N> augmented = this->concath(this->identity());

        // Apply Gaussian elimination
        for (unsigned int row = 0U; row < N; ++row) {
            
            // If diagonal element is zero, find a row below the current row to swap with
            if (augmented(row, row) == static_cast<TYPE>(0)) {
                bool swapped = false;
                for (unsigned int swap_row = row + 1; swap_row < N; ++swap_row) {
                    if (augmented(swap_row, row) != static_cast<TYPE>(0)) {
                        // Swap the current row with the row we found
                        for (unsigned int col = 0; col < 2 * N; ++col) {
                            std::swap(augmented(row, col), augmented(swap_row, col));
                        }
                        swapped = true;
                        break;
                    }
                }
            }

            // Make the diagonal contain all ones by dividing with the diagonal element of current row
            TYPE diag = augmented(row, row);
            for (unsigned int col = 0U; col < 2 * N; ++col) {
                augmented(row, col) /= diag;
            }

            // Make all other elements in this column zero by subtracting a factored other rows
            for (unsigned int element = 0U; element < N; ++element) {
                if (element != row) {
                    TYPE factor = augmented(element, row);

                    for (unsigned int col = 0U; col < 2 * N; ++col) {
                        augmented(element, col) -= factor * augmented(row, col);
                    }
                }
            }
        }

        // Extract the inverse from the augmented matrix from the right half of argumented matrix
        Matrix2D<TYPE, N, N> result;
        for (unsigned int row = 0U; row < N; ++row) {
            for (unsigned int col = 0U; col < N; ++col) {
                result(row, col) = augmented(row, col + N);
            }
        }

        return result;
    }

    void print(void) const {
        for (unsigned int row = 0U; row < ROWS; ++row) {
            for (unsigned int col = 0U; col < COLS; ++col) {
                std::cout << (*this)(row, col) << "\t";
            }
            std::cout << "\n";
        }
        std::cout << "\n";
    }

private:
    std::array<TYPE, ROWS * COLS> data;
};