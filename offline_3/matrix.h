/**
 * @file matrix.h
 * @brief   Matrix class
 *          This class is used to represent a matrix
 *          and perform operations on it.
 *          It is used to represent the transformation
 */

#ifndef MATRIX_H
#define MATRIX_H

#include <vector>

using namespace std;

class Matrix {
  /**
   * @brief   The matrix
   */
  vector<vector<double>> matrix;

public:
  /**
   * @brief   Constructor for Matrix class
   *          Initializes the matrix to be an identity matrix
   */
  Matrix();

  /**
   * @brief   Get the matrix
   * @return  The matrix
   */
  vector<vector<double>> getMatrix();

  /**
   * @brief   change the matrix's value
   * @param   i the row index
   * @param   j the column index
   * @param   value the value to be set
   */
  void setMatrixValue(int i, int j, double value);

  /**
   * @brief   Multiply two matrices
   * @param   other_matrix the matrix to be multiplied with
   * @return  the product of the two matrices
   */
  Matrix multiply(Matrix &other_matrix);

  /**
   * @brief   print the matrix
   */
  void printMatrix();
};

#endif // MATRIX_H