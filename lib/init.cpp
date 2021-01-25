#include <gpgme.h>
#include "init.hpp"
#include <locale.h>
#include <stdexcept>
#include <string>
#include <vector>


namespace egpgcrypt {


sys_initializer::sys_initializer()
{
    // See:
    // http://www.gnupg.org/documentation/manuals/gpgme/Multi-Threading.html
    // http://www.gnupg.org/documentation/manuals/gpgme/Library-Version-Check.html

    /* Initialize the locale environment.  */
    setlocale(LC_ALL, "");
    gpgme_check_version(nullptr);
    gpgme_set_locale(nullptr, LC_CTYPE, setlocale (LC_CTYPE, nullptr));
#ifdef LC_MESSAGES
    gpgme_set_locale(nullptr, LC_MESSAGES, setlocale (LC_MESSAGES, nullptr));
#endif

    if(gpgme_engine_check_version(GPGME_PROTOCOL_CMS) != GPG_ERR_NO_ERROR)
        throw std::runtime_error("no S/MIME support");

    if(gpgme_engine_check_version(GPGME_PROTOCOL_OpenPGP) != GPG_ERR_NO_ERROR)
        throw std::runtime_error("no OpenPGP support");
}


sys_initializer::~sys_initializer()
{
    // nothing yet
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
