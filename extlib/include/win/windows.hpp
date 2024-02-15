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
    using result_t = std::expected< T, error_t >;

    /// <summary>
    /// A safe self-deallocating handle.
    /// </summary>
    struct handle_t final
    {
        /// <summary>
        /// A custom deleter object for handle pointers.
        /// </summary>
        struct deleter
        {
            /// <summary>
            /// The actual deleter method (deletes pointer & closes handle).
            /// </summary>
            /// <param name="handle"></param>
            void operator()( handle_t *handle ) const;
        };

        /// <summary>
        /// Creates a new safe handle from a raw Windows handle.
        /// </summary>
        /// <param name="hHandle">The raw Windows handle.</param>
        /// <returns>Handle pointer.</returns>
        static std::shared_ptr< handle_t > get( HANDLE handle );

        /// <summary>
        /// Get the raw Windows handle.
        /// </summary>
        constexpr HANDLE raw() const
        {
            return hHandle;
        }

        /// <summary>
        /// Creates a new handle instance.
        /// </summary>
        /// <param name="hHandle">Windows handle.</param>
        handle_t( HANDLE hHandle );

        HANDLE hHandle;
    };
}  // namespace extlib::win