#include <cstdlib>
#include <data_buffers.hpp>
#include <fcntl.h>
#include <stdexcept>
#include <support.hpp>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>

namespace egpgcrypt {

file_data_buffer::file_data_buffer() : fd_(-1)
{
    init_fd();
    init_gpgme_data();
}

file_data_buffer::file_data_buffer(const data_buffer& copy_from) : fd_(-1)
{
    init_fd();
    init_gpgme_data();
    cat(copy_from);
}

file_data_buffer::file_data_buffer(const std::string& filename) : fd_(-1)
{
    init_fd(filename);
    init_gpgme_data();
}

file_data_buffer::file_data_buffer(const std::string& filename, const data_buffer& copy_from) : fd_(-1)
{
    init_fd(filename);
    init_gpgme_data();
    cat(copy_from);
}

void file_data_buffer::init_fd()
{
    char temporary_filename[] = "/tmp/egpgcryptXXXXXX";

    fd_ = mkstemp(temporary_filename);

    if (fd_ == -1)
        throw std::runtime_error(std::string("failed to open ") + temporary_filename + ": "
                                 + egpg_errno_strerror(errno));

    unlink(temporary_filename);
}

void file_data_buffer::init_fd(const std::string& filename)
{
    fd_ = open(filename.c_str(), O_RDWR | O_CREAT, S_IRUSR | S_IWUSR);

    if (fd_ == -1)
        throw std::runtime_error(std::string("failed to open ") + filename + ": " + egpg_errno_strerror(errno));
}

void file_data_buffer::init_gpgme_data()
{
    gpgme_error_t ret = gpgme_data_new_from_fd(&data_, fd_);

    if (ret != GPG_ERR_NO_ERROR) {
        throw std::runtime_error(std::string("file data buffer creation failed: ") + egpg_gpgme_strerror(ret));
    }
}

file_data_buffer::~file_data_buffer()
{
    gpgme_data_release(data_);

    if (fd_ != -1)
        close(fd_);
}

} // namespace egpgcrypt
