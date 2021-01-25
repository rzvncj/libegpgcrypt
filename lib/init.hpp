#ifndef __INIT_HPP_INCLUDED__
#define __INIT_HPP_INCLUDED__


namespace egpgcrypt {


// RAII GPGME-system initializer
class sys_initializer {

public:
    // Init
    sys_initializer();

    // Cleanup
    ~sys_initializer();
};


}


#endif // __INIT_HPP_INCLUDED__


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
