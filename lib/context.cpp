#include <context.hpp>
#include <stdexcept>
#include <support.hpp>

namespace egpgcrypt {

context::context(gpgme_protocol_t protocol, const std::string& keys_directory) : protocol_(protocol)
{
    gpgme_error_t ret = gpgme_new(&ctx_);

    if (ret != GPG_ERR_NO_ERROR)
        throw std::runtime_error("context creation failed: " + egpg_gpgme_strerror(ret));

    ret = gpgme_set_protocol(ctx_, protocol);

    if (ret != GPG_ERR_NO_ERROR) {
        gpgme_release(ctx_);
        throw std::runtime_error("could not set context protocol: " + egpg_gpgme_strerror(ret));
    }

    set_keys_directory(keys_directory);
}

context::~context()
{
    gpgme_release(ctx_);
}

void context::set_keys_directory(const std::string& keys_directory)
{
    if (keys_directory.empty())
        return;

    gpgme_engine_info_t info = gpgme_ctx_get_engine_info(ctx_);

    while (info && info->protocol != protocol_)
        info = info->next;

    if (info)
        gpgme_ctx_set_engine_info(ctx_, protocol_, info->file_name, keys_directory.c_str());
}

void context::set_armor(bool armor)
{
    gpgme_set_armor(ctx_, armor ? 1 : 0);
}

void context::set_textmode(bool textmode)
{
    gpgme_set_textmode(ctx_, textmode ? 1 : 0);
}

key context::find_key(const std::string& email, bool secret_key, bool for_encryption)
{
    gpgme_error_t err = gpgme_op_keylist_start(ctx_, email.c_str(), secret_key ? 1 : 0);

    if (err != GPG_ERR_NO_ERROR)
        throw std::runtime_error("key list error: " + egpg_gpgme_strerror(err));

    key ret;

    while (err == GPG_ERR_NO_ERROR) {
        gpgme_key_t k = 0;
        err           = gpgme_op_keylist_next(ctx_, &k);

        if (err == GPG_ERR_NO_ERROR
            && !(k->revoked || k->expired || k->invalid || k->disabled || (for_encryption && !k->can_encrypt))) {
            ret = k;
            break;
        }
    }

    gpgme_op_keylist_end(ctx_);

    return ret;
}

} // namespace egpgcrypt
