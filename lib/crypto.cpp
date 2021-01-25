#include "patterns.hpp"
#include <context.hpp>
#include <crypto.hpp>
#include <cstring>
#include <data_buffers.hpp>
#include <keys.hpp>
#include <stdexcept>
#include <support.hpp>

namespace egpgcrypt {

crypto::crypto(gpgme_protocol_t protocol, const std::string& keys_directory)
    : protocol_(protocol), keys_directory_(keys_directory)
{
}

void crypto::encrypt(const std::set<std::string>& email_addresses, std::set<std::string>& expired_keys,
                     const data_buffer& in, data_buffer& out)
{
    using namespace std;

    context ctx(protocol_, keys_directory_);

    patterns ptns;
    keys     enc_keys;

    expired_keys.clear();

    for (auto&& email_address : email_addresses)
        ptns.add(email_address);

    // Output should be ASCII armoured (I'm working with email)
    if (protocol_ == GPGME_PROTOCOL_OpenPGP)
        ctx.set_armor(true);
    else
        gpgme_data_set_encoding(out.data(), GPGME_DATA_ENCODING_BASE64);

    gpgme_error_t err = gpgme_op_keylist_ext_start(ctx.ctx(), ptns.char_patterns(), 0, 0);

    if (err != GPG_ERR_NO_ERROR)
        throw runtime_error(string("key list error: ") + egpg_gpgme_strerror(err));

    while (err == GPG_ERR_NO_ERROR) {

        gpgme_key_t key;
        err = gpgme_op_keylist_next(ctx.ctx(), &key);

        if (err != GPG_ERR_NO_ERROR)
            break;

        if (!is_valid_key(key, false, true)) {

            if (key->expired && key->uids && key->uids->email)
                expired_keys.insert(key->uids->email);

            gpgme_key_unref(key);
            continue;
        }

        enc_keys.add(key);
    }

    gpgme_op_keylist_end(ctx.ctx());

    if (enc_keys.empty())
        throw runtime_error(
            "key list error: no keys matched the"
            " supplied patterns");

    err = gpgme_op_encrypt(ctx.ctx(), enc_keys.raw_keys(), GPGME_ENCRYPT_ALWAYS_TRUST, in.data(), out.data());

    if (err != GPG_ERR_NO_ERROR)
        throw runtime_error(string("encrypt error: ") + egpg_gpgme_strerror(err));

    gpgme_encrypt_result_t result = gpgme_op_encrypt_result(ctx.ctx());

    if (result->invalid_recipients)
        throw runtime_error(string("invalid recipients: ") + result->invalid_recipients->fpr);
}

void crypto::sign(const std::set<std::string>& email_addresses, const data_buffer& in, data_buffer& out)
{
    using namespace std;

    context ctx(protocol_, keys_directory_);

    // Output should be ASCII armoured (I'm working with email)
    ctx.set_armor(true);
    ctx.set_textmode(true);

    patterns ptns;
    unsigned keys_added = 0;

    for (auto&& email_address : email_addresses)
        ptns.add(email_address);

    gpgme_error_t err = gpgme_op_keylist_ext_start(ctx.ctx(), ptns.char_patterns(), 0, 0);

    if (err != GPG_ERR_NO_ERROR)
        throw runtime_error(string("key list error: ") + egpg_gpgme_strerror(err));

    while (err == GPG_ERR_NO_ERROR) {

        gpgme_key_t key;
        err = gpgme_op_keylist_next(ctx.ctx(), &key);

        if (err != GPG_ERR_NO_ERROR)
            break;

        if (!is_valid_key(key, false, false)) {
            gpgme_key_unref(key);
            continue;
        }

        if (gpgme_signers_add(ctx.ctx(), key) == GPG_ERR_NO_ERROR)
            ++keys_added;
        else
            gpgme_key_unref(key);
    }

    gpgme_op_keylist_end(ctx.ctx());

    if (keys_added == 0)
        throw runtime_error(
            "key list error: no keys matched the"
            " supplied patterns");

    err = gpgme_op_sign(ctx.ctx(), in.data(), out.data(), GPGME_SIG_MODE_DETACH);

    if (err != GPG_ERR_NO_ERROR)
        throw runtime_error(string("sign error: ") + egpg_gpgme_strerror(err));

    gpgme_sign_result_t result = gpgme_op_sign_result(ctx.ctx());

    if (result->invalid_signers)
        throw runtime_error(string("invalid signers: ") + result->invalid_signers->fpr);
}

bool crypto::verify(const data_buffer& signature, const data_buffer& text)
{
    using namespace std;

    context ctx(protocol_, keys_directory_);

    // Output should be ASCII armoured (I'm working with email)
    ctx.set_armor(true);
    ctx.set_textmode(true);

    gpgme_error_t err = gpgme_op_verify(ctx.ctx(), signature.data(), text.data(), nullptr);

    if (err != GPG_ERR_NO_ERROR)
        throw runtime_error(string("signature verification error: ") + egpg_gpgme_strerror(err));

    gpgme_verify_result_t result = gpgme_op_verify_result(ctx.ctx());

    for (auto sig = result->signatures; sig; sig = sig->next) {
        if (sig->status == GPG_ERR_NO_ERROR)
            return true;
    }

    return false;
}

bool crypto::has_public_key(const std::string& email) const
{
    context ctx(protocol_);

    return ctx.find_key(email);
}

bool crypto::has_private_key(const std::string& email) const
{
    context ctx(protocol_);

    return ctx.find_key(email, true, true);
}

bool crypto::is_valid_key(gpgme_key_t key, bool secret, bool for_encryption) const
{
    bool ret = (!key->revoked && !key->expired && !key->invalid && !key->disabled);

    if (for_encryption)
        ret = (ret && key->can_encrypt);

    if (protocol_ != GPGME_PROTOCOL_OpenPGP)
        return ret; // this is probably a hack

    if (secret)
        ret = (ret && key->secret);
    else
        ret = (ret && !key->secret);

    return ret;
}

bool crypto::import_public_key(const std::string& email)
{
    return import_key(email, false);
}

bool crypto::import_private_key(const std::string& email)
{
    return import_key(email, true);
}

bool crypto::import_key(const std::string& email, bool secret)
{
    using namespace std;

    context ctx(protocol_, keys_directory_);
    keys    imp_keys;

    gpgme_keylist_mode_t mode = gpgme_get_keylist_mode(ctx.ctx());

    mode |= GPGME_KEYLIST_MODE_EXTERN; // enable extern lookups
    mode &= ~GPGME_KEYLIST_MODE_LOCAL; // disable local lookups

    gpgme_error_t err = gpgme_set_keylist_mode(ctx.ctx(), mode);

    if (err != GPG_ERR_NO_ERROR)
        throw runtime_error(string("set key list mode error: ") + egpg_gpgme_strerror(err));

    err = gpgme_op_keylist_start(ctx.ctx(), email.c_str(), 0);

    if (err != GPG_ERR_NO_ERROR)
        throw runtime_error(string("key list error: ") + egpg_gpgme_strerror(err));

    while (err == GPG_ERR_NO_ERROR) {

        gpgme_key_t key;
        err = gpgme_op_keylist_next(ctx.ctx(), &key);

        if (err != GPG_ERR_NO_ERROR)
            break;

        if (!is_valid_key(key, secret, false)) {
            gpgme_key_unref(key);
            continue;
        }

        imp_keys.add(key);
    }

    gpgme_op_keylist_end(ctx.ctx());

    if (imp_keys.empty())
        return false;

    err = gpgme_op_import_keys(ctx.ctx(), imp_keys.raw_keys());

    if (err != GPG_ERR_NO_ERROR)
        throw runtime_error(string("key import error: ") + egpg_gpgme_strerror(err));

    gpgme_import_result_t result = gpgme_op_import_result(ctx.ctx());

    if (!result)
        throw runtime_error(string("key import error: nullptr result"));

    return true;
}

} // namespace egpgcrypt
