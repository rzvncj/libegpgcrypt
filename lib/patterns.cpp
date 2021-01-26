#include "patterns.hpp"
#include <algorithm>
#include <cstdlib>
#include <cstring>

namespace egpgcrypt {

patterns::~patterns()
{
    std::for_each(char_patterns_.begin(), char_patterns_.end(), free);
}

void patterns::add(const std::string& pattern)
{
    if (char_patterns_.empty())
        char_patterns_.push_back(strdup(pattern.c_str()));
    else
        char_patterns_.back() = strdup(pattern.c_str());

    char_patterns_.push_back(nullptr);
}

const char** patterns::char_patterns() const
{
    return char_patterns_.empty() ? nullptr : const_cast<const char**>(&char_patterns_[0]);
}

} // namespace egpgcrypt
