#include <context.hpp>
#include <data_buffers.hpp>
#include <errno.h>
#include <gpgme.h>
#include <stdexcept>
#include <support.hpp>
#include <unistd.h>

#include <iostream>

namespace egpgcrypt {


template<typename T>
struct callback {

    static gpgme_error_t passphrase_cb(void *opaque, const char *uid_hint,
                                       const char *passphrase_info,
                                       int /* last_was_bad */, int fd)
    {
        std::cout << "GPGME passphrase callback CALLED" << std::endl;

        T *p_functor = reinterpret_cast<T *>(opaque);

        std::string passphrase = (*p_functor)(uid_hint, passphrase_info);
        passphrase += "\n";

        size_t passlen = passphrase.length();
        size_t off = 0;
        ssize_t ret = 0;

        do {
            ret = write(fd, passphrase.c_str() + off, passlen - off);

            if(ret > 0)
                off += ret;

        } while(ret > 0 && off != passlen);

        return off == passlen ? 0 : gpgme_error_from_errno(errno);
    }

};


template<typename T>
void crypto::decrypt(T passphrase_callback, const data_buffer& in,
                     data_buffer& out)
{
    context ctx(protocol_, keys_directory_);

    // gpgme_set_pinentry_mode(ctx.ctx(), GPGME_PINENTRY_MODE_LOOPBACK);

    if(protocol_ == GPGME_PROTOCOL_OpenPGP)
        gpgme_set_passphrase_cb(ctx.ctx(), callback<T>::passphrase_cb,
                                &passphrase_callback);
    else
        gpgme_data_set_encoding(in.data(), GPGME_DATA_ENCODING_BASE64);

    gpgme_error_t ret = gpgme_op_decrypt(ctx.ctx(), in.data(),
                                         out.data());

    if(ret != GPG_ERR_NO_ERROR)
        throw std::runtime_error(std::string("decrypt error: ")
                                 + egpg_gpgme_strerror(ret));
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
