/**
 *
 */

#include "matrix.h"

/**
 * @brief   Constructor for Matrix class
 * @details Initializes the matrix to be an identity matrix
 */

Matrix::Matrix() {
  // initialize the matrix to be an identity matrix
  for (int i = 0; i < 4; i++) {
    vector<double> row;
    for (int j = 0; j < 4; j++) {
      if (i == j) {
        row.push_back(1);
      } else {
        row.push_back(0);
      }
    }
    matrix.push_back(row);
  }
}

/**
 * @brief   Get the matrix
 * @return  The matrix
 */

vector<vector<double>> Matrix::getMatrix() { return this->matrix; }

/**
 * @brief   change the matrix's value
 * @param   i the row index
 * @param   j the column index
 * @param   value the value to be set
 */

void Matrix::setMatrixValue(int i, int j, double value) {
  this->matrix[i][j] = value;
}

/**
 * @brief   Multiply two matrices
 * @param   other_matrix the matrix to be multiplied with
 * @return  the product of the two matrices
 */

Matrix Matrix::multiply(Matrix &other_matrix) {
  // get the matrix
  vector<vector<double>> other_matrix_values = other_matrix.getMatrix();

  // multiply the matrices
  Matrix product;
  for (int i = 0; i < 4; i++) {
    for (int j = 0; j < 4; j++) {
      double value = 0;
      for (int k = 0; k < 4; k++) {
        value += this->matrix[i][k] * other_matrix_values[k][j];
      }
      product.setMatrixValue(i, j, value);
    }
  }

  return product;
}

/**
 * @brief   print the matrix
 */
void Matrix::printMatrix() {
  // print the matrix
  for (int i = 0; i < 4; i++) {
    cout << "[ ";
    for (int j = 0; j < 4; j++) {
      cout << this->matrix[i][j] << " ";
    }
    cout << "]" << endl;
  }
}