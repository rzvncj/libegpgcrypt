#ifndef __SUPPORT_HPP_INCLUDED__
#define __SUPPORT_HPP_INCLUDED__

#include <cstring>
#include <errno.h>
#include <gpgme.h>
#include <string>

namespace egpgcrypt {

inline std::string egpg_gpgme_strerror(gpgme_error_t code)
{
    char raw_buffer[1024] = {};

    gpgme_strerror_r(code, raw_buffer, sizeof(raw_buffer) - 1);

    return raw_buffer;
}

inline std::string egpg_errno_strerror(int code)
{
    char raw_buffer[1024] = {};

    strerror_r(code, raw_buffer, sizeof(raw_buffer) - 1);

    return raw_buffer;
}

}

#endif // __SUPPORT_HPP_INCLUDED__
