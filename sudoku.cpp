#include "sudoku.hpp"
#include <stdexcept>
#include <cmath>
#include <unordered_set>
#include <algorithm>
#include <random>
#include <numeric>
#include <iostream>
#include <iomanip>
#include <string>
#include <cstdlib> // Pour system()

void clear_console() {
    system("cls"); // Commande spécifique à Windows
}

/**
 * @brief Constructeur de la classe Sudoku.
 * @param size La taille de la grille (doit avoir une racine carrée entière).
 * @throws std::invalid_argument si la taille n'a pas une racine carrée entière.
 */
Sudoku::Sudoku(int size) : grille_(size, std::vector<int>(size, 0)) 
{
    double root = std::sqrt(size);
    if (root != static_cast<int>(root)) 
    {
        throw std::invalid_argument("Size must have an integer square root.");
    }
}

/**
 * @brief Vérifie si une valeur est dans l'intervalle valide de la grille.
 * @param a La valeur à vérifier.
 * @return True si la valeur est dans l'intervalle, False sinon.
 */
bool Sudoku::is_in_range(int a) const
{
    return a >= 0 && a < (int) size();
}

/**
 * @brief Opérateur d'accès en lecture à une ligne de la grille.
 * @param line L'indice de la ligne.
 * @return Une référence constante vers le vecteur représentant la ligne.
 * @throws std::out_of_range si l'indice est hors de la grille.
 */
const std::vector<int> & Sudoku::operator[](int line) const 
{
    if (!is_in_range(line)) 
    {
        throw std::out_of_range("Line index out of range.");
    }

    return grille_[line];
}

/**
 * @brief Obtient la valeur à une position donnée dans la grille.
 * @param line L'indice de la ligne.
 * @param column L'indice de la colonne.
 * @return La valeur à la position donnée.
 * @throws std::out_of_range si les indices sont hors de la grille.
 */
int Sudoku::get(int line, int column) const
{
    if (!is_in_range(column)) 
    {
        throw std::out_of_range("Column index out of range.");
    }

    return (*this)[line][column];
}

/**
 * @brief Définit une valeur à une position donnée dans la grille.
 * @param line L'indice de la ligne.
 * @param column L'indice de la colonne.
 * @param value La valeur à placer (doit être entre 1 et la taille de la grille).
 * @throws std::out_of_range si les indices sont hors de la grille.
 * @throws std::invalid_argument si la valeur est invalide.
 */
void Sudoku::set(int line, int column, int value)
{
    if (!is_in_range(line)) 
    {
        throw std::out_of_range("Line index out of range.");
    }

    if (!is_in_range(column)) 
    {
        throw std::out_of_range("Column index out of range.");
    }

    if (value < 1 || value > (int) size()) {
        throw std::invalid_argument("Value must be between 1 and the grid size.");
    }

    grille_[line][column] = value;
}

/**
 * @brief Obtient la taille de la grille.
 * @return La taille de la grille.
 */
size_t Sudoku::size() const
{
    return grille_.size();
}

/**
 * @brief Vérifie si une ligne est valide (pas de doublons).
 * @param row L'indice de la ligne à vérifier.
 * @return True si la ligne est valide, False sinon.
 */
bool Sudoku::is_row_valid(int row) const
{
    std::unordered_set<int> seen;
    for (int val : grille_[row]) {
        if (val != 0 && seen.find(val) != seen.end())
            return false;
        seen.insert(val);
    }
    return true;
}

/**
 * @brief Vérifie si une colonne est valide (pas de doublons).
 * @param col L'indice de la colonne à vérifier.
 * @return True si la colonne est valide, False sinon.
 */
bool Sudoku::is_col_valid(int col) const
{
    std::unordered_set<int> seen;
    for (size_t row = 0; row < size(); ++row) {
        int val = grille_[row][col];
        if (val != 0 && seen.find(val) != seen.end())
            return false;
        seen.insert(val);
    }
    return true;
}

/**
 * @brief Vérifie si une sous-grille est valide (pas de doublons).
 * @param start_row L'indice de départ de la ligne de la sous-grille.
 * @param start_col L'indice de départ de la colonne de la sous-grille.
 * @return True si la sous-grille est valide, False sinon.
 */
bool Sudoku::is_subgrid_valid(int start_row, int start_col) const
{
    std::unordered_set<int> seen;
    int root = std::sqrt(size());

    for (int i = 0; i < root; ++i) {
        for (int j = 0; j < root; ++j) {
            int val = grille_[start_row + i][start_col + j];
            if (val != 0 && seen.find(val) != seen.end())
                return false;
            seen.insert(val);
        }
    }
    return true;
}

/**
 * @brief Vérifie si la grille entière est valide.
 * @return True si la grille est valide, False sinon.
 */
bool Sudoku::is_grid_valid() const
{
    int root = std::sqrt(size());

    for (size_t i = 0; i < size(); ++i) {
        if (!is_row_valid(i) || !is_col_valid(i))
            return false;
    }

    for (size_t i = 0; i < size(); i += root) {
        for (size_t j = 0; j < size(); j += root) {
            if (!is_subgrid_valid(i, j))
                return false;
        }
    }

    return true;
}

/**
 * @brief Définit la seed pour le générateur aléatoire.
 * @param seed La seed à utiliser.
 */
void Sudoku::set_seed(unsigned int seed)
{
    rng_.seed(seed);
}

/**
 * @brief Vérifie si une valeur peut être placée à une position donnée.
 * @param row L'indice de la ligne.
 * @param col L'indice de la colonne.
 * @param value La valeur à placer.
 * @return True si la valeur peut être placée, False sinon.
 */
bool Sudoku::can_place(int row, int col, int value) const
{
    // Check row
    for (size_t c = 0; c < size(); ++c) {
        if (grille_[row][c] == value) {
            return false;
        }
    }

    // Check column
    for (size_t r = 0; r < size(); ++r) {
        if (grille_[r][col] == value) {
            return false;
        }
    }

    // Check sub-grid
    int root = std::sqrt(size());
    int start_row = (row / root) * root;
    int start_col = (col / root) * root;
    for (int r = 0; r < root; ++r) {
        for (int c = 0; c < root; ++c) {
            if (grille_[start_row + r][start_col + c] == value) {
                return false;
            }
        }
    }

    return true; // Safe to place
}

/**
 * @brief Remplit la grille de manière récursive en respectant les règles du Sudoku.
 * @param row L'indice de la ligne actuelle.
 * @param col L'indice de la colonne actuelle.
 * @return True si la grille est remplie avec succès, False sinon.
 */
bool Sudoku::fill_grid(size_t row, size_t col, bool show)
{
    if (row == size()) {
        return true; // All rows are filled
    }

    // Calculate the next cell coordinates
    size_t next_row = (col == size() - 1) ? row + 1 : row;
    size_t next_col = (col == size() - 1) ? 0 : col + 1;

    // Shuffle numbers for randomness
    std::vector<int> numbers(size());
    std::iota(numbers.begin(), numbers.end(), 1); // Fill with 1 to N
    std::shuffle(numbers.begin(), numbers.end(), rng_);

    // Try placing each number
    for (int value : numbers) {
        if (can_place(row, col, value)) {
            grille_[row][col] = value; // Place the number
            
            if(show) {
                clear_console();
                display();
            }

            if (fill_grid(next_row, next_col, show)) {
                return true; // Continue filling
            }
            grille_[row][col] = 0; // Backtrack
        }
    }

    return false; // No valid placement found
}

/**
 * @brief Génère une grille de Sudoku valide aléatoire.
 * @throws std::runtime_error si la génération échoue.
 */
void Sudoku::generate_random_grid(bool show)
{
    // Clear the grid
    for (auto& row : grille_) {
        std::fill(row.begin(), row.end(), 0);
    }

    // Fill the grid using backtracking
    if (!fill_grid(0, 0, show)) {
        throw std::runtime_error("Failed to generate a valid Sudoku grid.");
    }
}

/**
 * @brief Affiche la grille de Sudoku dans la console.
 */
void Sudoku::display() const
{
    int root = std::sqrt(size()); // Taille d'un bloc
    int max_value = size();       // Valeur maximale dans la grille
    int max_width = std::to_string(max_value).size(); // Longueur du plus grand nombre

    // Ligne de séparation pour les blocs horizontaux
    std::string horizontal_line((size() * (max_width + 1)) + (root - 1), '-');

    for (size_t i = 0; i < size(); ++i) {
        if (i % root == 0 && i != 0) {
            std::cout << horizontal_line << std::endl; // Ligne de séparation
        }

        for (size_t j = 0; j < size(); ++j) {
            if (j % root == 0 && j != 0) {
                std::cout << "| "; // Séparation verticale entre blocs
            }
            // Afficher chaque valeur avec un espacement dynamique
            grille_[i][j] == 0 ? std::cout << std::setw(max_width) << "." << " " : std::cout << std::setw(max_width) << grille_[i][j] << " ";
        }
        std::cout << std::endl;
    }
}
