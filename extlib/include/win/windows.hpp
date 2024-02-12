#pragma once

#include <Windows.h>

#include <expected>
#include <iostream>
#include <vector>

#include "win/error.hpp"

namespace extlib::win
{
    /// <summary>
    /// The standard result of any native Windows system call.
    /// </summary>
    /// <typeparam name="T">The expected value's type.</typeparam>
    template< typename T >
    using result = std::expected< T, error_t >;

    /// <summary>
    /// A safe self-deallocating handle.
    /// </summary>
    struct handle_t final
    {
        /// <summary>
        /// Creates a new handle instance.
        /// </summary>
        /// <param name="hHandle">Windows handle.</param>
        handle_t( HANDLE hHandle );

        /// <summary>
        /// Get the raw Windows handle.
        /// </summary>
        constexpr HANDLE get() const;

       protected:
        HANDLE hHandle;
    };
}  // namespace extlib::win