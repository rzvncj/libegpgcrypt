#ifndef __CONTEXT_HPP_INCLUDED__
#define __CONTEXT_HPP_INCLUDED__

#include <gpgme.h>
#include <safe_egpg_object.hpp>
#include <string>

namespace egpgcrypt {

// Forward declarations.
class key;

class context : private safe_egpg_object {

public:
    context(gpgme_protocol_t protocol = GPGME_PROTOCOL_OpenPGP, const std::string& keys_directory = "");
    ~context();

    context(const context&) = delete;
    context& operator=(const context&) = delete;

public:
    gpgme_ctx_t      ctx() const { return ctx_; }
    gpgme_protocol_t protocol() const { return protocol_; }

    void set_keys_directory(const std::string& keys_directory);
    void set_armor(bool armor);
    void set_textmode(bool textmode);

    key find_key(const std::string& email, bool secret_key = false, bool for_encryption = true);

private:
    gpgme_protocol_t protocol_;
    gpgme_ctx_t      ctx_;
};

}

#endif // __CONTEXT_HPP_INCLUDED__
