#include <iostream>
#include <string>
#include <vector>
#include <algorithm>

/**
 * @brief Prétraite la chaîne d'entrée en insérant des séparateurs afin de simplifier le traitement
 *        des palindromes de longueur paire et impaire.
 * 
 * @param s La chaîne d'entrée.
 * @return La chaîne transformée avec des marqueurs de début, fin et des séparateurs entre chaque caractère.
 */
std::string preprocessString(const std::string& s) {
    if (s.empty()) 
        return "^$";
    std::string T = "^";
    for (char c : s) {
        T += "#" + std::string(1, c);
    }
    T += "#$";
    return T;
}

/**
 * @brief Calcule pour chaque position de la chaîne prétraitée le rayon du palindrome centré en cette position.
 * 
 * Utilise la technique du "miroir" pour éviter des comparaisons redondantes.
 * 
 * @param T La chaîne prétraitée.
 * @return Un vecteur d'entiers où P[i] correspond au rayon du palindrome centré en la position i.
 */
std::vector<int> computePalindromeRadii(const std::string & T) {
    int n = T.size();
    std::vector<int> P(n, 0);
    int center = 0, right = 0;  // 'center' et 'right' définissent la fenêtre actuelle la plus à droite

    for (int i = 1; i < n - 1; i++) {
        int mirror = 2 * center - i;  // position miroir de i par rapport au centre actuel

        if (i < right)
            P[i] = std::min(right - i, P[mirror]);

        // Extension du palindrome centré en i
        while (T[i + 1 + P[i]] == T[i - 1 - P[i]])
            P[i]++;

        // Mise à jour de la fenêtre si le palindrome centré en i s'étend au-delà de 'right'
        if (i + P[i] > right) {
            center = i;
            right = i + P[i];
        }
    }
    return P;
}

/**
 * @brief Recherche dans le vecteur P la position centrale qui donne le plus long palindrome.
 * 
 * @param P Le vecteur contenant les rayons des palindromes pour chaque position de la chaîne prétraitée.
 * @return Une paire {centerIndex, maxLen} où centerIndex est l'indice du centre du plus long palindrome
 *         et maxLen la longueur de ce palindrome (rayon).
 */
std::pair<int, int> findLongestPalindromeIndex(const std::vector<int>& P) {
    int maxLen = 0;
    int centerIndex = 0;
    for (int i = 1; i < static_cast<int>(P.size()) - 1; i++) {
        if (P[i] > maxLen) {
            maxLen = P[i];
            centerIndex = i;
        }
    }
    return {centerIndex, maxLen};
}

/**
 * @brief Trouve et retourne le plus long sous-palindrome contenu dans la chaîne d'entrée.
 * 
 * @param s La chaîne d'entrée.
 * @return Le plus long sous-palindrome.
 */
std::string longestPalindrome(const std::string& s) {
    if (s.empty()) 
        return "";
    
    // Étape 1 : Prétraiter la chaîne d'entrée
    std::string T = preprocessString(s);
    
    // Étape 2 : Calculer pour chaque position de T le rayon du palindrome centré en celle-ci
    std::vector<int> P = computePalindromeRadii(T);
    
    // Étape 3 : Trouver le centre ayant le plus grand rayon
    auto [centerIndex, maxLen] = findLongestPalindromeIndex(P);
    
    // Étape 4 : Déduire la position de départ dans la chaîne d'origine
    int start = (centerIndex - maxLen) / 2;
    return s.substr(start, maxLen);
}

int main() {
    std::vector<std::string> testCases = {
        "babad",
        "cbbd",
        "a",
        "ac",
        "racecar",
        "noon",
        "forgeeksskeegfor",
        "aibohphobia",
        "abcdefghijklmnopqrstuvwxyzzyxwvutsrqponmlkjihgfedcba",
        "abacdfgdcaba",
        "abacdfgdcabba",
        "abacdedcaba",
        "abacdedcabbacdedcaba",
        "abacdedcabbacdedcabbacdedcaba",
        "abacdedcabbacdedcabbacdedcabbacdedcaba",
        "abacdedcabbacdedcabbacdedcabbacdedcabbacdedcaba"
    };

    for (const auto& testCase : testCases) {
        std::cout << "Input: " << testCase << std::endl;
        std::cout << "Longest Palindromic Substring: " << longestPalindrome(testCase) << std::endl;
        std::cout << "-----------------------------" << std::endl;
    }

    return 0;
}
