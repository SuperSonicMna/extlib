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
        pe32_t = IMAGE_NT_OPTIONAL_HDR32_MAGIC,

        /// <summary>
        /// Indicates a 64-bit executable.
        /// </summary>
        pe64_t = IMAGE_NT_OPTIONAL_HDR64_MAGIC
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
        /// <param name="bytes_read">The number of bytes the syscall read.</param>
        /// <returns>The value.</returns>
        template< typename T >
        T read( std::uintptr_t address, std::size_t* bytes_read = nullptr ) const;

        /// <summary>
        /// Gets all of the sections in the current module.
        /// </summary>
        /// <returns>List of sections.</returns>
        std::vector< section_t > get_sections() const;

        /// <summary>
        /// The PE format of the current module.
        /// </summary>
        pe_kind kind;

        /// <summary>
        /// The start/end locations of the current module.
        /// </summary>
        std::uintptr_t start, end;

        /// <summary>
        /// The name of the current module.
        /// </summary>
        std::string_view name;

       protected:
        HMODULE hModule;
        std::shared_ptr< handle_t > handle;

        IMAGE_DOS_HEADER dos_header;
        IMAGE_NT_HEADERS nt_headers;

        std::vector< section_t > sections;
    };

    /// <summary>
    /// A section containing information about a module.
    /// </summary>
    struct section_t final
    {
        /// <summary>
        /// Creates a new section with the provded parameters.
        /// </summary>
        /// <param name="parent">The module this section resides in.</param>
        /// <param name="section_header">The section's header.</param>
        explicit section_t( module_t* parent, const IMAGE_SECTION_HEADER& section_header );

        /// <summary>
        /// Returns the size of the section in bytes.
        /// </summary>
        /// <returns>Size of section.</returns>
        constexpr std::size_t size() const;

        /// <summary>
        /// Returns a string representation of the current section.
        /// </summary>
        /// <returns>String representation.</returns>
        std::string to_string() const;

        std::uintptr_t start, end;
        std::string_view name;

       protected:
        module_t parent_module;
    };

    template< typename T >
    inline T module_t::read( std::uintptr_t address, std::size_t* bytes_read ) const
    {
        const auto result = read_process_memory< T >( handle, address, bytes_read );

        if ( result.has_value() )
            return result.value();

        throw error( "ReadProcessMemory failed: {}", result.error().what() );
    }
}  // namespace extlib::win