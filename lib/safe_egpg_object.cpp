#include "init.hpp"
#include <safe_egpg_object.hpp>


namespace egpgcrypt {


safe_egpg_object::safe_egpg_object()
{
    static sys_initializer init;
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
