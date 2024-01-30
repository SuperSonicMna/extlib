#pragma once

#include <exception>
#include <string>

#include "win.hpp"

namespace extlib::win
{
    class win_exception : public std::exception
    {
       public:
        /// <summary>
        /// Creates a new exceptions using the provided error code.
        /// </summary>
        /// <param name="error_code">The error code.</param>
        /// <param name="function">The name of the function that failed.</param>
        /// <returns></returns>
        static win_exception from_code( std::uintptr_t error_code, const char* const function = nullptr );

        /// <summary>
        /// Creates a new exception using `GetLastError`.
        /// </summary>
        /// <param name="function">The name of the function that failed.</param>
        /// <returns>New error.</returns>
        static win_exception from_last_error( const char* const function = nullptr );

        /// <summary>
        /// Returns the error message.
        /// </summary>
        /// <returns>Error message;</returns>
        virtual const char* what() const
        {
            return message.c_str();
        }

        std::string message, function{};
        std::uintptr_t code;

       private:
        /// <summary>
        /// Creates a new windows exception instance.
        /// </summary>
        /// <param name="code">The error code.</param>
        /// <param name="message">The error message.</param>
        /// <param name="function">The function that errored.</param>
        explicit win_exception( std::uintptr_t code, const std::string& message, const std::string& function )
            : message( message ),
              code( code ),
              function( function )
        {
        }

        /// <summary>
        /// Creates a new windows exception instance.
        /// </summary>
        /// <param name="code">The error code.</param>
        /// <param name="message">The error message.</param>
        explicit win_exception( std::uintptr_t code, const std::string& message ) : message( message ), code( code )
        {
        }
    };
}  // namespace extlib::win