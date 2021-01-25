#ifndef __DATA_BUFFERS_HPP_INCLUDED__
#define __DATA_BUFFERS_HPP_INCLUDED__


#include <safe_egpg_object.hpp>
#include <gpgme.h>
#include <string>


namespace egpgcrypt {


class data_buffer : private safe_egpg_object {

public:
    enum seek_pos {
        SET = SEEK_SET,
        CUR = SEEK_CUR,
        END = SEEK_END
    };

public:
    data_buffer();
    virtual ~data_buffer();

public:
    gpgme_data_t data() const { return data_; }

    bool read(std::string& buffer) const;
    void write(const std::string& buffer);
    void seek(off_t off, seek_pos pos) const;
    off_t tell() const;

    void cat(const data_buffer& from);

    std::string content() const;

private: // Copy not allowed
    data_buffer(const data_buffer&);
    data_buffer& operator=(const data_buffer&);

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
    file_data_buffer(const std::string& filename,
                     const data_buffer& copy_from);

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
