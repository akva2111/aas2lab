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
std::vector<int> findAll(const std::string& text, const std::string& pattern) {
    std::vector<int> occurrences; // Stores indices of all found occurrences.

    if (pattern.empty()) {
        // An empty pattern is considered to be found at the beginning of any string.
        if (!text.empty()) occurrences.push_back(0);
        else occurrences.push_back(0); // Also at the beginning if text is empty.
        return occurrences;
    }
    if (text.length() < pattern.length()) {
        return occurrences; // Pattern cannot be found if text is shorter.
    }

    BadCharTable badCharTable = buildBadCharTable(pattern);
    int n = text.length();
    int m = pattern.length();
    int shift = 0; // Current alignment of the pattern's start in the text.

    while (shift <= (n - m)) {
        int j = m - 1; // Start comparison from the end of the pattern.

        while (j >= 0 && pattern[j] == text[shift + j]) {
            j--; 
        }

        if (j < 0) {
            // Full match found.
            occurrences.push_back(shift); // Add the index to our list.
            int shiftAfterMatch = m; // Default shift is the pattern length.

            if (m > 0) {
                char nextTextChar = (shift + m < n) ? text[shift + m] : '\0';

                // Apply bad character heuristic for nextTextChar.
                if (badCharTable.count(nextTextChar)) {
                    // Shift so nextTextChar aligns with its last occurrence in the pattern.
                    shiftAfterMatch = m - badCharTable[nextTextChar];
                }
                else {
                    // If nextTextChar is not in the pattern, shift by pattern length.
                    shiftAfterMatch = m;
                }
                // Ensure we always shift forward by at least 1.
                shift += (shiftAfterMatch < 1 ? 1 : shiftAfterMatch);
            }
            else { // Empty pattern case (should be handled by initial check)
                shift += 1;
            }
        }
        else {
            // Mismatch, apply bad character heuristic.
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
    return occurrences;
}
