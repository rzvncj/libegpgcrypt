#include <data_buffers.hpp>
#include <stdexcept>
#include <support.hpp>

namespace egpgcrypt {

memory_data_buffer::memory_data_buffer()
{
    gpgme_error_t ret = gpgme_data_new(&data_);

    if (ret != GPG_ERR_NO_ERROR)
        throw std::runtime_error(std::string("file data buffer creation failed: ") + egpg_gpgme_strerror(ret));
}

memory_data_buffer::memory_data_buffer(const std::string& data)
{
    gpgme_error_t ret = gpgme_data_new_from_mem(&data_, data.c_str(), data.length(), 1);

    if (ret != GPG_ERR_NO_ERROR)
        throw std::runtime_error(std::string("file data buffer creation failed: ") + egpg_gpgme_strerror(ret));
}

memory_data_buffer::~memory_data_buffer()
{
    gpgme_data_release(data_);
}

} // namespace egpgcrypt
