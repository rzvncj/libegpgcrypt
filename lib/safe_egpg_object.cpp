#include "init.hpp"
#include <safe_egpg_object.hpp>

namespace egpgcrypt {

safe_egpg_object::safe_egpg_object()
{
    static sys_initializer init;
}

} // namespace egpgcrypt
