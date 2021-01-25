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
    return key_ ? key_->revoked : false;
}


bool key::expired() const
{
    return key_ ? key_->expired : false;
}


bool key::invalid() const
{
    return key_ ? key_->invalid : false;
}


bool key::disabled() const
{
    return key_ ? key_->disabled : false;
}


bool key::can_encrypt() const
{
    return key_ ? key_->can_encrypt : false;
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
    return keys_.empty() ? nullptr : &keys_[0];
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
