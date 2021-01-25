#include <keys.hpp>
#include <stdexcept>


namespace egpgcrypt {


key::key()
    : key_(static_cast<gpgme_key_t>(0), unref_key)
{
    // nothing
}


key::key(gpgme_key_t raw_key)
    : key_(raw_key, unref_key)
{
}


void key::unref_key(gpgme_key_t raw_key)
{
    if(raw_key)
        gpgme_key_unref(raw_key);
}


bool key::revoked() const
{
    if(key_)
        return key_->revoked;

    return false;
}


bool key::expired() const
{
    if(key_)
        return key_->expired;

    return false;
}


bool key::invalid() const
{
    if(key_)
        return key_->invalid;

    return false;
}


bool key::disabled() const
{
    if(key_)
        return key_->disabled;

    return false;
}


bool key::can_encrypt() const
{
    if(key_)
        return key_->can_encrypt;

    return false;
}


bool key::secret() const
{
    if(key_ && key_->subkeys)
        return key_->subkeys->secret;

    return false;
}


long int key::expires() const
{
    if(key_ && key_->subkeys)
        return key_->subkeys->expires;

    return 0;
}


std::string key::email() const
{
    if(key_ && key_->uids && key_->uids->email)
        return key_->uids->email;

    return "";
}


keys::~keys()
{
    for(size_t i = 0; i < keys_.size(); ++i) {
        if(keys_[i])
            gpgme_key_unref(keys_[i]);
    }
}


void keys::add(gpgme_key_t key)
{
    if(keys_.empty()) {
        keys_.push_back(key);
        keys_.push_back(nullptr);
        return;
    }

    keys_[keys_.size() - 1] = key;
    keys_.push_back(nullptr);
}


gpgme_key_t* keys::raw_keys()
{
    if(keys_.empty())
        return nullptr;

    return &keys_[0];
}


bool keys::empty() const
{
    return keys_.empty();
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
