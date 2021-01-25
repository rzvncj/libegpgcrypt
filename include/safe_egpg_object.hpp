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
