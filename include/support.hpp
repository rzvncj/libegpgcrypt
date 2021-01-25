#ifndef __SUPPORT_HPP_INCLUDED__
#define __SUPPORT_HPP_INCLUDED__

#include <cstring>
#include <errno.h>
#include <gpgme.h>
#include <string>


namespace egpgcrypt {


inline std::string egpg_gpgme_strerror(gpgme_error_t code)
{
    char raw_buffer[1024];
    raw_buffer[0] = '\0';

    gpgme_strerror_r(code, raw_buffer, sizeof(raw_buffer) - 1);
    raw_buffer[sizeof(raw_buffer) - 1] = '\0';

    return raw_buffer;
}


inline std::string egpg_errno_strerror(int code)
{
    char raw_buffer[1024];
    raw_buffer[0] = '\0';

    strerror_r(code, raw_buffer, sizeof(raw_buffer) - 1);
    raw_buffer[sizeof(raw_buffer) - 1] = '\0';

    return raw_buffer;
}


}


#endif // __SUPPORT_HPP_INCLUDED__


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
