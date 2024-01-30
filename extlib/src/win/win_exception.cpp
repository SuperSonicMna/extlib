#include "win/win_exception.hpp"

namespace extlib::win
{
    win_exception win_exception::from_code( std::uintptr_t error_code, const char* const function )
    {
        std::string message;
        LPSTR MessageBuffer = nullptr;

        // Ask Win32 to give us the string version of that message ID.
        // The parameters we pass in, tell Win32 to create the buffer that holds the message for us (because we don't yet
        // know how long the message string will be).
        size_t size = FormatMessageA(
            FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM | FORMAT_MESSAGE_IGNORE_INSERTS,
            NULL,
            error_code,
            MAKELANGID( LANG_NEUTRAL, SUBLANG_DEFAULT ),
            ( LPSTR )&MessageBuffer,
            0,
            NULL );

        // Copy the win_error message into a std::string.
        message.append( MessageBuffer, size );

        // Free the Win32's string's buffer.
        LocalFree( MessageBuffer );

        return win_exception( error_code, message, function );
    }

    win_exception win_exception::from_last_error( const char* const function )
    {
        return from_code( GetLastError(), function );
    }
}  // namespace extlib::win