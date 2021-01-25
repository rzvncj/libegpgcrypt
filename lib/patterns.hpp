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
    void add(const std::string& pattern);
    const char** char_patterns() const;

private:
    std::vector<char *> char_patterns_;
};


}


#endif // __PATTERNS_HPP_INCLUDED__


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
