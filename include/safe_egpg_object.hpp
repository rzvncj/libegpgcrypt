#ifndef __SAFE_EGPG_OBJECT_HPP_INCLUDED__
#define __SAFE_EGPG_OBJECT_HPP_INCLUDED__

namespace egpgcrypt {

// Base class for all classes that call at least one
// GPGME function (it makes sure the GPGME system has
// been properly initialized before its use).
class safe_egpg_object {

public:
    safe_egpg_object();

    // Base class, virtual destructor
    virtual ~safe_egpg_object() = default;
};

}

#endif // __SAFE_EGPG_OBJECT_HPP_INCLUDED__
