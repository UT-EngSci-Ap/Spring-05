#pragma once
#include <string>

// Extracts and removes the first whitespace-delimited token from str.
std::string getNextToken(std::string& str);

// Returns a copy of str with leading whitespace stripped.
std::string trimLeading(const std::string& str);
