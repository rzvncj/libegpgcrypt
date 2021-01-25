#ifndef __DATA_BUFFERS_HPP_INCLUDED__
#define __DATA_BUFFERS_HPP_INCLUDED__

#include <gpgme.h>
#include <safe_egpg_object.hpp>
#include <string>

namespace egpgcrypt {

class data_buffer : private safe_egpg_object {

public:
    enum seek_pos { SET = SEEK_SET, CUR = SEEK_CUR, END = SEEK_END };

public:
    data_buffer()          = default;
    virtual ~data_buffer() = default;

    data_buffer(const data_buffer&) = delete;
    data_buffer& operator=(const data_buffer&) = delete;

public:
    gpgme_data_t data() const { return data_; }

    bool  read(std::string& buffer) const;
    void  write(const std::string& buffer);
    void  seek(off_t off, seek_pos pos) const;
    off_t tell() const;

    void cat(const data_buffer& from);

    std::string content() const;

protected:
    gpgme_data_t data_;
};

class memory_data_buffer : public data_buffer {

public:
    memory_data_buffer(); // Empty buffer
    memory_data_buffer(const std::string& data);
    virtual ~memory_data_buffer();
};

class file_data_buffer : public data_buffer {

public:
    file_data_buffer(); // temporary unique file
    file_data_buffer(const data_buffer& copy_from);

    file_data_buffer(const std::string& filename);
    file_data_buffer(const std::string& filename, const data_buffer& copy_from);

    virtual ~file_data_buffer();

private:
    void init_fd();
    void init_fd(const std::string& filename);
    void init_gpgme_data();

private:
    int fd_;
};

}

#endif // __DATA_BUFFERS_HPP_INCLUDED__
