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

    // Forward declerations.
    struct section_t;

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
    /// Describes the kinds of portable executables (PE).
    /// </summary>
    enum class pe_kind : std::uint16_t
    {
        /// <summary>
        /// Indicates a 32-bit executable.
        /// </summary>
        pe32_t = 0x010B,

        /// <summary>
        /// Indicates a 64-bit executable.
        /// </summary>
        pe64_t = 0x020B
    };

    /// <summary>
    /// A module owned by a process.
    /// </summary>
    struct module_t final
    {
        /// <summary>
        /// Creates a new module instance.
        /// </summary>
        /// <param name="handle">The handle to the owning process.</param>
        /// <param name="entry">The module entry.</param>
        explicit module_t( std::shared_ptr< handle_t > handle, const MODULEENTRY32& entry );

        /// <summary>
        /// Gets the raw module instance.
        /// </summary>
        /// <returns></returns>
        constexpr HMODULE raw() const;

        /// <summary>
        /// Gets the size of the module.
        /// </summary>
        /// <returns>Size of the module in bytes.</returns>
        constexpr std::size_t size() const;

        /// <summary>
        /// Returns a string representation of the module.
        /// </summary>
        /// <returns>Module as string.</returns>
        std::string to_string() const;

        /// <summary>
        /// Reads a value from the module's memory starting at a given address.
        /// </summary>
        /// <typeparam name="T">The type of the value.</typeparam>
        /// <param name="address">The starting address to read from.</param>
        /// <returns>The value.</returns>
        template< typename T >
        T read( std::uintptr_t address ) const;

        /// <summary>
        /// Gets all of the sections in the current module (re-reads PE header).
        /// </summary>
        /// <returns>List of sections.</returns>
        std::vector< section_t > get_sections() const;

        /// <summary>
        /// The PE format of the current module.
        /// </summary>
        pe_kind kind;

       protected:
        HMODULE hModule;
        std::shared_ptr< handle_t > handle;
        std::uintptr_t start, end;
        std::string_view name;

        std::vector< section_t > sections;
    };

    template< typename T >
    inline T module_t::read( std::uintptr_t address ) const
    {
        return read_process_memory< T >( handle, address );
    }
}  // namespace extlib::win