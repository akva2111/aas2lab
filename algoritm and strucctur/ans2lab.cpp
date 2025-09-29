#include <string>      
#include <vector>      
#include <iostream>    
#include <unordered_map> 
#include <sstream>
using BadCharTable = std::unordered_map<char, int>;
BadCharTable buildBadCharTable(const std::string& pattern) {
    BadCharTable table;
    for (int i = 0; i < pattern.length(); ++i) {
        table[pattern[i]] = i;
    }
    return table;
}
int findIndex(const std::string& text, const std::string& pattern) {
    if (pattern.empty()) {
        return 0; // Empty pattern
    }
    if (text.length() < pattern.length()) {
        return -1; // Pattern cannot be found
    }

    BadCharTable badCharTable = buildBadCharTable(pattern);
    int n = text.length();
    int m = pattern.length();
    int shift = 0;
    while (shift <= (n - m)) {
        int j = m - 1; // Start comparison from the end
        while (j >= 0 && pattern[j] == text[shift + j]) {
            j--;
        }
        if (j < 0) {
            return shift; 
        }
        else {
            char mismatchedChar = text[shift + j];
            int badCharShift = 0;
            if (badCharTable.count(mismatchedChar)) {
                badCharShift = j - badCharTable[mismatchedChar];
                if (badCharShift < 1) badCharShift = 1;
            }
            else {
                badCharShift = j + 1;
            }
            shift += badCharShift; 
        }
    }
    return -1; // Pattern not found.
}