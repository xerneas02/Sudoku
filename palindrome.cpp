#include <iostream>
#include <string>
#include <vector>


/**
 * @brief Finds the longest palindromic substring in a given string using Manacher's algorithm.
 * 
 * Manacher's algorithm is an efficient algorithm to find the longest palindromic substring in linear time.
 * The algorithm transforms the input string to avoid even/odd length complications, then uses a center expansion
 * technique to find the longest palindrome centered at each position.
 * 
 * @param s The input string.
 * @return The longest palindromic substring.
 */
std::string longestPalindrome(const std::string& s);

int main() {
    std::vector<std::string> testCases = {
        "babad",
        "cbbd",
        "a",
        "ac",
        "racecar",
        "noon",
        "forgeeksskeegfor"
        "aibohphobia",
        "abcdefghijklmnopqrstuvwxyzzyxwvutsrqponmlkjihgfedcba",
        "abacdfgdcaba",
        "abacdfgdcabba",
        "abacdedcaba",
        "abacdedcabbacdedcaba",
        "abacdedcabbacdedcabbacdedcaba",
        "abacdedcabbacdedcabbacdedcabbacdedcaba",
        "abacdedcabbacdedcabbacdedcabbacdedcabbacdedcaba",
        "abacdedcabbacdedcabbacdedcabbacdedcabbacdedcabbacdedcaba"
    };

    for (const auto& testCase : testCases) {
        std::cout << "Input: " << testCase << std::endl;
        std::cout << "Longest Palindromic Substring: " << longestPalindrome(testCase) << std::endl;
        std::cout << "-----------------------------" << std::endl;
    }

    return 0;
}

std::string preprocessString(const std::string& s) {
    if (s.empty()) return "^$";
    std::string result = "^";
    for (char c : s) {
        result += "#" + std::string(1, c);
    }
    result += "#$";
    return result;
}

std::string removeSpecialCharacters(const std::string& s) {
    std::string result;
    for (char c : s) {
        if (std::isalnum(c)) {
            result += c;
        }
    }
    return result;
}

std::string longestPalindrome(const std::string& s) {
    std::string processedString = preprocessString(s);
    int n = processedString.size();
    std::vector<int> p(n, 0);

    for (int center = 0; center < n; center++)
    {
        for (int right = center+1, left = center-1; right < n && left >= 0 && processedString[right] == processedString[left]; right++, left--)
        {
            p[center]++;
        }
        
    }
    
    int maxLen = 0;
    int centerIndex = 0;
    for (int i = 1; i < n - 1; i++) {
        if (p[i] > maxLen) {
            maxLen = p[i];
            centerIndex = i;
        }
    }

    int start = (centerIndex - maxLen) / 2;
    std::string longestPalindromicSubstring = s.substr(start, maxLen);
    return removeSpecialCharacters(longestPalindromicSubstring);
}