#include <cerrno>
#include <cstddef>

extern "C" int _getentropy(void* buffer, std::size_t length)
{
    (void)buffer;
    (void)length;

    errno = ENOSYS;
    return -1;
}