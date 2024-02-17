#pragma once

// clang-format off
#include <Windows.h>
// clang-format on

#include <TlHelp32.h>

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
            void operator()( handle_t* handle ) const;
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

       protected:
        /// <summary>
        /// Creates a new handle instance.
        /// </summary>
        /// <param name="hHandle">Windows handle.</param>
        explicit handle_t( HANDLE hHandle );

        HANDLE hHandle;
    };

    /// <summary>
    /// A module owned by a process.
    /// </summary>
    struct module_t final
    {
        /// <summary>
        /// Gets the raw module instance.
        /// </summary>
        /// <returns></returns>
        constexpr HMODULE raw() const
        {
            return hModule;
        }

        /// <summary>
        /// Gets the size of the module.
        /// </summary>
        /// <returns>Size of the module in bytes.</returns>
        constexpr std::size_t size() const
        {
            return start - end;
        }

       protected:
        /// <summary>
        /// Creates a new module instance.
        /// </summary>
        /// <param name="handle">The handle to the owning process.</param>
        /// <param name="entry">The module entry.</param>
        explicit module_t( std::shared_ptr< handle_t > handle, const MODULEENTRY32& entry );

        HMODULE hModule;
        std::shared_ptr< handle_t > handle;
        std::uintptr_t start, end;
        std::string_view name;
    };
}  // namespace extlib::win