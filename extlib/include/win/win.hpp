#pragma once

#include <Windows.h>

#include <filesystem>
#include <string>
#include <vector>

#include "win/memapi.hpp"
#include "win/region.hpp"

// forward declerations.
namespace extlib
{
    struct pattern_t;
}

namespace extlib::win
{
    // System default values.
    constexpr auto MAX_MODULE_COUNT = 512;
    constexpr auto MAX_PROCESSES_COUNT = 1024;

    /// <summary>
    /// Describes the kind of portable executable we have at hand.
    /// </summary>
    enum class pe_kind_t : std::uint16_t;

    struct section_t;
    struct region_t;

    /// <summary>
    /// Handle wrapper structure.
    /// </summary>
    struct handle_t
    {
        /// <summary>
        /// Creates a new self-deallocating windows handle.
        /// </summary>
        /// <param name="hHandle"></param>
        handle_t( HANDLE hHandle ) : handle( hHandle )
        {
        }

        /// <summary>
        /// Default constructor.
        /// </summary>
        handle_t() : handle( nullptr )
        {
        }

        /// <summary>
        /// Determines whether the handle is valid or not.
        /// </summary>
        /// <returns>True if the handle is valid.</returns>
        bool is_valid() const;

        /// <summary>
        /// Retrieves the fully qualified path for the file containing the specified handle.
        /// </summary>
        /// <returns></returns>
        std::filesystem::path path();

        HANDLE handle;
    };

    /// <summary>
    /// Represents a string in the process module.
    /// </summary>
    struct string_t
    {
        string_t( const std::string& value, const std::uintptr_t& address ) : value( value ), address( address )
        {
        }

        std::string value;
        std::uintptr_t address;
    };

    /// <summary>
    /// Module wrapper structure.
    /// </summary>
    struct module_t
    {
        /// <summary>
        /// Creates a new module.
        /// </summary>
        module_t( HANDLE hHandle, HMODULE hModule );

        /// <summary>
        /// Creates a new module.
        /// </summary>
        module_t();

        /// <summary>
        /// Reads from the provided memory location.
        /// </summary>
        /// <typeparam name="T">The type to read.</typeparam>
        /// <param name="address">The location to read from.</param>
        /// <returns>The value.</returns>
        template< typename T >
        T read( std::uintptr_t address, std::size_t* bytes_read = nullptr ) const;

        /// <summary>
        /// Reads a string from the provided memory location.
        /// </summary>
        std::string read_string( std::uintptr_t address, std::size_t max_length = 50 ) const;

        /// <summary>
        /// Reads from the provided memory location.
        /// </summary>
        /// <param name="address">The location to read from.</param>
        /// <param name="length">The number of bytes to read.</param>
        /// <returns>An array of bytes.</returns>
        std::vector< std::uint8_t > read( std::uintptr_t address, std::size_t length ) const;

        /// <summary>
        /// Gets a section in the module by name.
        /// </summary>
        /// <param name="name">The name of the section.</param>
        /// <returns>New section.</returns>
        section_t operator[]( const std::string_view name ) const;

        /// <summary>
        /// Gets all of the sections in the module.
        /// </summary>
        /// <returns>List of sections.</returns>
        std::vector< section_t > get_sections();

        /// <summary>
        /// Gets all locations of strings with the matching name.
        /// </summary>
        /// <param name="name">The name of the strings to search for.</param>
        /// <returns>A list of strings and their locations.</returns>
        std::vector< string_t > get_strings_by_name( std::string_view name ) const;

        /// <summary>
        /// Gets all strings from the .rdata section.
        /// </summary>
        /// <param name="min_length">The minimum size of the byte arrays for them to be considered.</param>
        /// <returns></returns>
        std::vector< string_t > get_all_strings( std::size_t min_length = 0 );

        /// <summary>
        /// Finds all matches for the given pattern in this section.
        /// </summary>
        /// <param name="pattern">The pattern to use.</param>
        /// <returns>A list of locations.</returns>
        std::vector< std::uintptr_t > find_all( const pattern_t& pattern ) const;

        /// <summary>
        /// Checks to see if this module contains the provided address.
        /// </summary>
        /// <param name="address">The address to check.</param>
        /// <returns>True, if it contains the address.</returns>
        constexpr bool contains( std::uintptr_t address ) const
        {
            return !( address < start || address >= end );
        }

        /// <summary>
        /// Gets all the regions.
        /// </summary>
        /// <param name="start">The start address of the region.</param>
        /// <param name="end">The end address of the region.</param>
        /// <returns>A list of pages.</returns>
        std::vector< region_t > get_regions( std::uintptr_t start, std::uintptr_t end ) const;

        /// <summary>
        /// Gets all the regions located within the current module.
        /// </summary>
        std::vector< region_t > get_regions() const;

        /// <summary>
        /// Gets the name of the current module.
        /// </summary>
        std::string get_name() const;

        HMODULE module;
        HANDLE handle;

        std::uintptr_t start, end;

        pe_kind_t kind{};

        IMAGE_DOS_HEADER dos{};
        IMAGE_NT_HEADERS nt{};
        IMAGE_SECTION_HEADER section{};

        std::vector< section_t > sections;
    };

    /// <summary>
    /// Represets a section within the portable executable.
    /// </summary>
    struct section_t
    {
        /// <summary>
        /// Creates a new portable executable section.
        /// </summary>
        /// <param name="module">The module of the process.</param>
        /// <param name="section">The section header.</param>
        section_t( const module_t& module, const IMAGE_SECTION_HEADER& section );

        /// <summary>
        /// Reads from the provided memory location.
        /// </summary>
        /// <typeparam name="T">The type to read.</typeparam>
        /// <param name="address">The location to read from.</param>
        /// <returns>The value.</returns>
        template< typename T >
        T read( std::uintptr_t address, std::size_t* bytes_read = nullptr ) const
        {
            return current_module.read< T >( address, bytes_read );
        }

        /// <summary>
        /// Checks to see if this section contains the provided address.
        /// </summary>
        /// <param name="address">The address to check.</param>
        /// <returns>True, if it contains the address.</returns>
        constexpr bool contains( std::uintptr_t address ) const
        {
            return !( address < start || address >= end );
        }

        /// <summary>
        /// Finds all matches for the given pattern in this section.
        /// </summary>
        /// <param name="pattern">The pattern to use.</param>
        /// <returns>A list of locations.</returns>
        std::vector< std::uintptr_t > find_all( const pattern_t& pattern ) const;

        /// <summary>
        /// Gets all regions within the current section.
        /// </summary>
        std::vector< region_t > get_regions() const;

        std::uintptr_t start, end;
        std::size_t size;
        std::string name;

        module_t current_module;
    };

    /// <summary>
    /// Describes the kind of portable executable we have at hand.
    /// </summary>
    enum class pe_kind_t : std::uint16_t
    {
        /// <summary>
        /// Indicates the binary contains a 32-bit portable executable file.
        /// </summary>
        pe32 = 0x010B,

        /// <summary>
        /// Indicates the binary contains a 64-bit portable executable file.
        /// </summary>
        pe64 = 0x020B
    };

    template< typename T >
    inline T module_t::read( std::uintptr_t address, std::size_t* bytes_read ) const
    {
        return memapi::read_process_memory< T >( handle, address, bytes_read );
    }
}  // namespace extlib::win