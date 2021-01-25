#include <algorithm>
#include <cstdlib>
#include <cstring>
#include "patterns.hpp"


namespace egpgcrypt {


patterns::patterns()
{
    // nothing
}


patterns::~patterns()
{
    std::for_each(char_patterns_.begin(), char_patterns_.end(), free);
}


void patterns::add(const std::string& pattern)
{
    if(char_patterns_.empty()) {
        char_patterns_.push_back(strdup(pattern.c_str()));
        char_patterns_.push_back(nullptr);
        return;
    }

    char_patterns_[char_patterns_.size() - 1] =
        strdup(pattern.c_str());

    char_patterns_.push_back(nullptr);
}


const char** patterns::char_patterns()
{
    return char_patterns_.empty() ?
        nullptr : const_cast<const char **>(&char_patterns_[0]);
}


} // namespace egpgcrypt


/*
  Local Variables:
  mode: c++
  c-basic-offset: 4
  tab-width: 4
  c-indent-comments-syntactically-p: t
  c-tab-always-indent: t
  indent-tabs-mode: nil
  End:
*/

// vim:shiftwidth=4:autoindent:tabstop=4:expandtab:softtabstop=4
