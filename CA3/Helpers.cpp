#include "Helpers.h"

std::string getNextToken(std::string& str) {
    size_t start = 0;
    while (start < str.length() &&
           (str[start] == ' ' || str[start] == '\t' || str[start] == '\r'))
        start++;

    if (start == str.length()) { str = ""; return ""; }

    size_t end = start;
    while (end < str.length() &&
           str[end] != ' ' && str[end] != '\t' && str[end] != '\r')
        end++;

    std::string token = str.substr(start, end - start);
    str = str.substr(end);
    return token;
}

std::string trimLeading(const std::string& str) {
    size_t start = 0;
    while (start < str.length() &&
           (str[start] == ' ' || str[start] == '\t' || str[start] == '\r'))
        start++;
    return str.substr(start);
}
