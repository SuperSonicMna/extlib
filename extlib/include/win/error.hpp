#pragma once

#include <cstdint>
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
}  // namespace extlib::win