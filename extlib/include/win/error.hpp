#pragma once

#include <cstdint>
#include <iostream>
#include <string>

namespace extlib::win
{
    /// <summary>
    /// Represents an error returned by a Windows system call.
    /// </summary>
    struct error_t final
    {
        /// <summary>
        /// Creates a new error using GetLastError().
        /// </summary>
        /// <returns>The error.</returns>
        static error_t get_last();

        /// <summary>
        /// Gets the message from the system error code.
        /// </summary>
        /// <returns>Error message.</returns>
        std::string_view what() const;

        /// <summary>
        /// The Windows error code (https://learn.microsoft.com/en-us/windows/win32/debug/system-error-codes).
        /// </summary>
        std::uintptr_t code;
    };

    /// <summary>
    /// Represents a runtime error thrown by the library.
    /// </summary>
    class error : std::runtime_error
    {
       public:
        /// <summary>
        /// Creates a new throwable error.
        /// </summary>
        /// <param name="e">The windows error.</param>
        explicit error( const error_t& e );

        /// <summary>
        /// Creates anew error from a formattable constructor.
        /// </summary>
        /// <typeparam name="...T">The types for the variadic.</typeparam>
        /// <param name="format">The format string.</param>
        /// <param name="...args">The list of arguments.</param>
        template< class... T >
        explicit error( const std::format_string< T... > format, T&&... args )
            : std::runtime_error( std::vformat( format.get(), std::make_format_args( args... ) ) )
        {
        }
    };

}  // namespace extlib::win