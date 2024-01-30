#pragma once

#include <memory>
#include <optional>
#include <type_traits>
#include <utility>
#include <vector>

#include "win/win.hpp"

namespace extlib
{
    struct pattern_t;

    /// <summary>
    /// Options for the scanning engine.
    /// </summary>
    struct scanner_options_t
    {
        /// <summary>
        /// Creates new scanner options.
        /// </summary>
        /// <param name="start">The start address for the region.</param>
        /// <param name="end">The end address for the region.</param>
        /// <param name="handle">The handle to the parent process.</param>
        scanner_options_t( std::uintptr_t start, std::uintptr_t end, win::handle_t handle );

        /// <summary>
        /// Creates new scanner options.
        /// </summary>
        /// <param name="section">The section to scan.</param>
        scanner_options_t( const win::section_t& section );

        /// <summary>
        /// Crates new scanner options.
        /// </summary>
        /// <param name="current">The current module to scan.</param>
        scanner_options_t( const win::module_t& current );

        /// <summary>
        /// Creates new scanner options.
        /// </summary>
        scanner_options_t();

        std::uintptr_t start, end;
        std::size_t size;
        win::handle_t handle;
    };

    /// Handles process scanning.
    /// </summary>
    class scanner final
    {
        scanner_options_t options;

       public:
        /// <summary>
        /// Creates a new scanner with the provided options.
        /// </summary>
        /// <param name="options">The options for the scanner.</param>
        explicit scanner( const scanner_options_t& options );

        /// <summary>
        /// Creates a new scanner with default options.
        /// </summary>
        explicit scanner();

        /// <summary>
        /// Finds all instances of a given byte pattern.
        /// </summary>
        /// <param name="pattern">The pattern to look for.</param>
        /// <returns>A list of locations within the process.</returns>
        std::vector< std::uintptr_t > find_all( const pattern_t& pattern ) const;
    };

    /// <summary>
    /// Represtents a byte pattern to scan for.
    /// </summary>
    struct pattern_t
    {
        /// <summary>
        /// Creates a new pattern from a string containing an array of bytes pattern.
        /// </summary>
        /// <param name="aob">The string containing an array of bytes in hex form.</param>
        /// <returns>A new pattern.</returns>
        static pattern_t from_byte_pattern( const std::string_view pattern );

        /// <summary>
        /// Creates a new pattern from a string of characters.
        /// </summary>
        /// <param name="string">The string of characters.</param>
        pattern_t( const std::string& string );

        /// <summary>
        /// Creates a new pattern from an object of type T.
        /// </summary>
        /// <typeparam name="T">The type of the object.</typeparam>
        /// <param name="object">The object to convert.</param>
        template< typename T >
        inline pattern_t( const T& object )
        {
            auto begin = reinterpret_cast< const std::uint8_t* >( std::addressof( object ) );
            const auto end = begin + sizeof( T );

            for ( ; begin != end; ++begin )
                bytes.emplace_back( *begin, false );
        }

        /// <summary>
        /// Finds all instances of the current pattern in the byte vector.
        /// </summary>
        /// <param name="bytes">The list of bytes to search.</param>
        /// <returns>A list of locations.</returns>
        std::vector< std::size_t > find_matches( std::vector< std::uint8_t > page ) const;

        /// <summary>
        /// Represents a list of bytes and mask flag. If the flag is true, the byte is a wildcard.
        /// </summary>
        std::vector< std::pair< std::uint8_t, bool > > bytes;
    };

}  // namespace extlib