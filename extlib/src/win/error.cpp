#include "win/error.hpp"

#include <Windows.h>

#include <string>

namespace extlib::win
{
    error_t error_t::get_last()
    {
        return { GetLastError() };
    }

    std::string_view error_t::what() const
    {
        LPSTR buffer = nullptr;

        // Ask Win32 to give us the string version of that message ID.
        // The parameters we pass in, tell Win32 to create the buffer that holds the message for us (because we don't yet
        // know how long the message string will be).
        size_t size = FormatMessageA(
            FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM | FORMAT_MESSAGE_IGNORE_INSERTS,
            NULL,
            code,
            MAKELANGID( LANG_NEUTRAL, SUBLANG_DEFAULT ),
            ( LPSTR )&buffer,
            0,
            NULL );

        // Copy the win_error message into a std::string.
        std::string_view message{ buffer, size };

        // Free the Win32's string's buffer.
        LocalFree( buffer );

        return message;
    }
}  // namespace extlib::win