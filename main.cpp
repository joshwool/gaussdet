#include <iostream>
#include <memory>

float sign = 1;

struct LeastZeros {
    float **row;
    std::unique_ptr<LeastZeros> next;
};

template<size_t size>
void detRec (float *(&rows)[size], int matrix_shape) {
    if (matrix_shape == 1) {
        return;
    }

    size_t least_zeros = size;
    size_t first_cell = size-matrix_shape;

    LeastZeros head {nullptr};
    LeastZeros *tail = &head;

    for (size_t i = first_cell; i < size; i++) {
        size_t zeros = 0;
        for (size_t j = first_cell; j < size; j++) {
            if (*(rows[i] + j) != 0) {
                break;
            }
            zeros++;
        }
        if (zeros < least_zeros) {
            least_zeros = zeros;
            head.row = &rows[i];
            head.next = nullptr;
            tail = &head;
        }
        else if (zeros == least_zeros) {
            tail->next = std::unique_ptr<LeastZeros>(new LeastZeros{&rows[i], nullptr});
            tail = tail->next.get();
        }

    }

    if (*head.row != rows[first_cell]) {
        float *temp = rows[first_cell];
        rows[first_cell] = *head.row;
        *head.row = temp;
        head.row = &rows[first_cell];
        sign *= -1;
    }

    LeastZeros *current = &head;

    while (current->next) {
        current = (current->next).get();

        float multi = (-*(*(current->row) + first_cell)) / *(*(head.row) + first_cell);

        for (size_t j = first_cell; j < size; j++) {
            *(*(current->row) + j) += (multi * *(*(head.row) + j));
            if (-0.00001 < *(*(current->row) + j) && *(*(current->row) + j) < 0.00001) {
                *(*(current->row) + j) = 0;
            }
        }
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

    float matrix[5][5] = {
        {0, 6, -2, -1, 5},
        {0, 0, 0, -9, -7},
        {0, 15, 35, 0, 0},
        {0, -1, -11, -2, 1},
        {-2, -2, 3, 0, -2}
    };

    std::cout << determinant(matrix) << std::endl;

    return 0;
}