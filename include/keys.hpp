#ifndef __KEYS_HPP_INCLUDED__
#define __KEYS_HPP_INCLUDED__

#include <boost/shared_ptr.hpp>
#include <gpgme.h>
#include <safe_egpg_object.hpp>
#include <string>
#include <vector>


namespace egpgcrypt {


class key : private safe_egpg_object {

public:
    key();
    key(gpgme_key_t raw_key);

    // No need for operator=(), key_ is a smart pointer.

public:
    bool revoked() const;
    bool expired() const;
    bool invalid() const;
    bool disabled() const;
    bool secret() const;
    bool can_encrypt() const;
    long expires() const;
    std::string email() const;

public:
    operator bool() const { return key_ != 0; }

private:
    // Custom deleter.
    static void unref_key(gpgme_key_t raw_key);
    typedef boost::shared_ptr<_gpgme_key> key_ptr_t;

private:
    key_ptr_t key_;
};


typedef std::vector<key> keys_t;


class keys {

public:
    keys() = default;
    ~keys();

    keys(const keys&) = delete;
    keys& operator=(const keys&) = delete;

public:
    void add(gpgme_key_t key);
    gpgme_key_t* raw_keys();

    bool empty() const;

private:
    std::vector<gpgme_key_t> keys_;
};


}


#endif // __KEYS_HPP_INCLUDED__


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
