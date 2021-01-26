#ifndef __INIT_HPP_INCLUDED__
#define __INIT_HPP_INCLUDED__

namespace egpgcrypt {

// RAII GPGME-system initializer
class sys_initializer {

public:
    // Init
    sys_initializer();

    // No cleanup needed for the moment.
};

}

#endif // __INIT_HPP_INCLUDED__
