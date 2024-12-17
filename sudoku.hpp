/**
 * @file sudoku.hpp
 * @brief Header file for the Sudoku class.
 *
 * This file contains the declarations of the Sudoku class, which provides
 * functionalities to generate, validate, and manipulate Sudoku grids of 
 * arbitrary sizes.
 */

#ifndef SUDOKU_HPP
#define SUDOKU_HPP

#include <vector>
#include <random>

/** @typedef grille_t
 *  @brief Type definition for the Sudoku grid.
 */
using grille_t = std::vector<std::vector<int>>;

/**
 * @class Sudoku
 * @brief Represents a Sudoku grid of arbitrary size.
 */
class Sudoku {
private:
    grille_t grille_;            ///< 2D grid representing the Sudoku.
    std::mt19937 rng_;           ///< Random number generator for grid generation.

    bool is_in_range(int a) const;
    bool is_row_valid(int row) const;
    bool is_col_valid(int col) const;
    bool is_subgrid_valid(int start_row, int start_col) const;

    bool can_place(int row, int col, int value) const;
    bool fill_grid(size_t row, size_t col, bool show = false);

public:
    Sudoku(int size = 9);

    const std::vector<int> & operator[](int line) const;
    int get(int line, int column) const;
    void set(int line, int column, int value);

    size_t size() const;

    bool is_grid_valid() const;
    void generate_random_grid(bool show = false);

    void set_seed(unsigned int seed);

    void display() const;
};

#endif // SUDOKU_HPP
