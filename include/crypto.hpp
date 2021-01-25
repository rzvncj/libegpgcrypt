#ifndef __CRYPTO_HPP_INCLUDED__
#define __CRYPTO_HPP_INCLUDED__


#include <context.hpp>
#include <gpgme.h>
#include <safe_egpg_object.hpp>
#include <set>
#include <string>


namespace egpgcrypt {


// Forward declaration
class data_buffer;


class crypto : private safe_egpg_object {

public:
    // Possible values for protocol are GPGME_PROTOCOL_OpenPGP (useful for
    // PGP/MIME), and GPGME_PROTOCOL_CMS (useful for S/MIME).
    crypto(gpgme_protocol_t protocol = GPGME_PROTOCOL_OpenPGP,
           const std::string& keys_directory = "");

public:
    void encrypt(const std::set<std::string>& email_addresses,
                 std::set<std::string>& expired_keys,
                 const data_buffer& in, data_buffer& out);

    template<typename T>
    void decrypt(T passphrase_callback, const data_buffer& in,
                 data_buffer& out);

    void sign(const std::set<std::string>& email_addresses,
              const data_buffer& in, data_buffer& out);

    bool verify(const data_buffer& signature,
                const data_buffer& text);

    bool has_public_key(const std::string& email) const;
    bool has_private_key(const std::string& email) const;

    bool import_public_key(const std::string& email);
    bool import_private_key(const std::string& email);

private:
    bool import_key(const std::string& email, bool secret);
    bool is_valid_key(gpgme_key_t key, bool secret,
                      bool for_encryption) const;

private:
    gpgme_protocol_t protocol_;
    std::string      keys_directory_;
};


}


#include "decrypt_impl.hpp" // actual implementation for decrypt()


#endif // __CRYPTO_HPP_INCLUDED__


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
