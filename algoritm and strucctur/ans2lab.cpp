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