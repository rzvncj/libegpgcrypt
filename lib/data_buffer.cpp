#include <data_buffers.hpp>
#include <stdexcept>
#include <support.hpp>

namespace egpgcrypt {

bool data_buffer::read(std::string& buffer) const
{
    char raw_buffer[4096];

    ssize_t ret = gpgme_data_read(data_, raw_buffer, sizeof(raw_buffer) - 1);

    if (ret == 0)
        return false; // done reading

    if (ret < 0)
        throw std::runtime_error(std::string("read error: ") + egpg_gpgme_strerror(ret));

    buffer.assign(raw_buffer, ret);
    return true;
}

void data_buffer::write(const std::string& buffer)
{
    ssize_t total = buffer.length(), written = 0;

    do {
        ssize_t ret = gpgme_data_write(data_, buffer.c_str() + written, total - written);
        if (ret < 0)
            throw std::runtime_error(std::string("write error: ") + egpg_gpgme_strerror(ret));
        written += ret;

    } while (written < total);
}

void data_buffer::seek(off_t off, seek_pos pos) const
{
    if (gpgme_data_seek(data_, off, pos) < 0)
        throw std::runtime_error(std::string("seek error: ") + egpg_errno_strerror(errno));
}

off_t data_buffer::tell() const
{
    off_t cur = gpgme_data_seek(data_, 0, SEEK_CUR);

    if (cur < 0)
        throw std::runtime_error(std::string("seek error: ") + egpg_errno_strerror(errno));

    return cur;
}

void data_buffer::cat(const data_buffer& from)
{
    if (&from == this)
        return;

    off_t cur = from.tell();

    from.seek(0, SET);

    std::string buffer;

    while (from.read(buffer))
        write(buffer);

    from.seek(cur, SET);
}

std::string data_buffer::content() const
{
    off_t cur = tell();

    seek(0, SET);

    std::string buffer, accumulated;

    while (read(buffer))
        accumulated.append(buffer);

    seek(cur, SET);

    return accumulated;
}

} // namespace egpgcrypt
