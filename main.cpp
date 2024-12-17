#include <iostream>
#include "sudoku.hpp"

int main() {
    Sudoku sudoku(25);

    sudoku.set_seed(42);
    sudoku.generate_random_grid();

    std::cout << "Sudoku grid:\n";
    sudoku.display();

    std::cout << sudoku.is_grid_valid() << std::endl;

    return 0;
}
