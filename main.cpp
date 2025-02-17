#include <iostream>
#include <memory>

float sign = 1;

template<size_t size>
void detRec (float *(&rows)[size], int matrix_shape) {
    if (matrix_shape == 1) {
        return;
    }

    size_t first_cell = size-matrix_shape;

    float **top = nullptr;

    for (size_t i = first_cell; i < size; i++) {
        if (*(rows[i] + first_cell) != 0) {
            if (!top) {
                top = &rows[i];
            }
            else {
                float multi = (-*(rows[i] + first_cell)) / *(*top + first_cell);

                for (size_t j = first_cell; j < size; j++) {
                    *(rows[i] + j) += (multi * *(*top + j));
                    if (-0.00001 < *(rows[i] + j) && *(rows[i] + j) < 0.00001) {
                        *(rows[i] + j) = 0;
                    }
                }
            }
        }
    }

    if (top != nullptr && *top != rows[first_cell]) {
        float *temp = rows[first_cell];
        rows[first_cell] = *top;
        *top = temp;
        sign *= -1;
    }

    for (size_t i = 0; i < size; i++) {
        for (size_t j = 0; j < size; j++) {
            std::cout << *(rows[i] + j) << " ";
        }
        std::cout << std::endl;
    }

    std::cout << std::endl;

    detRec(rows, --matrix_shape);
}


template<size_t size>
float determinant(float matrix[size][size]) {
    float *rows[size];

    for (size_t i = 0; i < size; i++) {
        rows[i] = &(matrix[i][0]);
    }

    size_t i = size - 1;
    while (i--) {
        bool all_zeros = true;
        for (int j = 0; j < size; j++) {
            if (*(rows[i] + j) != 0) {
                all_zeros = false;
                break;
            }
        }
        if (all_zeros) {
            return 0;
        }
    }

    detRec(rows, size);

    float product = 1;

    float *diagonal = nullptr;
    for (size_t ij = 0; ij < size; ij++) {
        diagonal = rows[ij] +ij;
        product *= *diagonal;

    }

    product *= sign;

    return product;
}


int main() {

    float matrix[4][4] = {
        {-1, 2, 1, 2},
        {2, 1, 0, 1},
        {1, 3, 1, 3},
        {-2, 4, 2, 4}
    };

    std::cout << determinant(matrix) << std::endl;

    return 0;
}