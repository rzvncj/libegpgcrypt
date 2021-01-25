#ifndef __PATTERNS_HPP_INCLUDED__
#define __PATTERNS_HPP_INCLUDED__

#include <string>
#include <vector>

namespace egpgcrypt {

class patterns {

public:
    patterns() = default;
    ~patterns();

    patterns(const patterns&) = delete;
    patterns& operator=(const patterns&) = delete;

public:
    void         add(const std::string& pattern);
    const char** char_patterns() const;

private:
    std::vector<char*> char_patterns_;
};

}

#endif // __PATTERNS_HPP_INCLUDED__
