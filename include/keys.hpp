#ifndef __KEYS_HPP_INCLUDED__
#define __KEYS_HPP_INCLUDED__

#include <gpgme.h>
#include <memory>
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
    bool        revoked() const;
    bool        expired() const;
    bool        invalid() const;
    bool        disabled() const;
    bool        secret() const;
    bool        can_encrypt() const;
    long        expires() const;
    std::string email() const;

public:
    operator bool() const { return key_ != nullptr; }

private:
    // Custom deleter.
    static void unref_key(gpgme_key_t raw_key);

private:
    std::shared_ptr<_gpgme_key> key_;
};

typedef std::vector<key> keys_t;

class keys {

public:
    keys() = default;
    ~keys();

    keys(const keys&) = delete;
    keys& operator=(const keys&) = delete;

public:
    void         add(gpgme_key_t key);
    gpgme_key_t* raw_keys();

    bool empty() const;

private:
    std::vector<gpgme_key_t> keys_;
};

}

#endif // __KEYS_HPP_INCLUDED__
