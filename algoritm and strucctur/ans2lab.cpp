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
//2)
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
std::vector<int> findAllInRange(const std::string& text, const std::string& pattern, int startIdx, int endIdx) {
    std::vector<int> occurrences;

    // --- Input Parameter Validation ---
    if (pattern.empty()) {
        // An empty pattern is considered to be present at the start of each position within the range.
        for (int i = startIdx; i <= endIdx; ++i) {
            occurrences.push_back(i);
        }
        return occurrences;
    }

    int n = text.length(); // Length of the entire text
    int m = pattern.length(); // Length of the pattern
startIdx = (startIdx < 0) ? 0 : startIdx; // Start index cannot be less than 0.
endIdx = (endIdx >= n) ? (n - 1) : endIdx; // End index cannot be greater than the last valid index.
if (startIdx > endIdx || (endIdx - startIdx + 1) < m) {
    return occurrences; // Return an empty vector.
}
BadCharTable badCharTable = buildBadCharTable(pattern);
int shift = startIdx;
int searchLimit = endIdx - m + 1;

while (shift <= searchLimit) {
    int j = m - 1; // Start comparison from the end of the pattern.

    // Compare characters from right to left.
    while (j >= 0 && pattern[j] == text[shift + j]) {
        j--;
    }
if (j < 0) {
    occurrences.push_back(shift);
int shiftAfterMatch = m;
if (m > 0) {
    char nextTextChar = (shift + m < n) ? text[shift + m] : '\0';
    if (badCharTable.count(nextTextChar)) {
        shiftAfterMatch = m - badCharTable[nextTextChar];
    }
    else {
        shiftAfterMatch = m;
    }
    shift += (shiftAfterMatch < 1 ? 1 : shiftAfterMatch);
}
else {
    shift += 1;
}
        }
 else {
     // Mismatch occurred, apply the bad character heuristic.
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
int main() {
    std::string text;
    std::string pattern;

    // --- User Input ---
    // Prompt for text input
    std::cout << "Enter the text: ";
    std::getline(std::cin, text); // Use getline to read the entire line, including spaces.

    // Prompt for substring input
    std::cout << "Enter the substring to search for: ";
    std::getline(std::cin, pattern);

    // --- Testing findIndex ---
    std::cout << "\n--- Testing findIndex ---" << std::endl;
    int firstIndex = findIndex(text, pattern);
    std::cout << "Text: \"" << text << "\", Pattern: \"" << pattern << "\"" << std::endl;
    if (firstIndex != -1) {
        std::cout << "First occurrence found at index: " << firstIndex << std::endl;
    }
    else {
        std::cout << "Substring not found." << std::endl;
    }

    // --- Testing findAll ---
    std::cout << "\n--- Testing findAll ---" << std::endl;
    std::vector<int> allIndices = findAll(text, pattern);
    std::cout << "Text: \"" << text << "\", Pattern: \"" << pattern << "\"" << std::endl;
    std::cout << "All occurrences: [";
    for (size_t i = 0; i < allIndices.size(); ++i) {
        std::cout << allIndices[i] << (i == allIndices.size() - 1 ? "" : ", ");
    }
    std::cout << "]" << std::endl;

    // --- Testing findAllInRange ---
    std::cout << "\n--- Testing findAllInRange ---" << std::endl;
    int startIdx, endIdx;
    std::cout << "Enter the start index for the search range: ";
    std::cin >> startIdx;
    std::cout << "Enter the end index for the search range: ";
    std::cin >> endIdx;
    // Clear the input buffer after reading numbers to prevent issues with subsequent getline calls.
    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');

    std::vector<int> rangeIndices = findAllInRange(text, pattern, startIdx, endIdx);
    std::cout << "Text: \"" << text << "\", Pattern: \"" << pattern << "\", Range: [" << startIdx << ", " << endIdx << "]" << std::endl;
    std::cout << "Result: [";
    for (size_t i = 0; i < rangeIndices.size(); ++i) {
        std::cout << rangeIndices[i] << (i == rangeIndices.size() - 1 ? "" : ", ");
    }
    std::cout << "]" << std::endl;

    return 0;
}
