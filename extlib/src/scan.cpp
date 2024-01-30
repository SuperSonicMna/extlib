#include "scan.hpp"

#include <iostream>
#include <sstream>
#include <string>

#include "win/memapi.hpp"

namespace extlib
{
    scanner::scanner( const scanner_options_t& options ) : options( options )
    {
    }

    scanner::scanner() : options{}
    {
    }

    std::vector< std::uintptr_t > scanner::find_all( const pattern_t& pattern ) const
    {
        std::vector< std::uintptr_t > addresses;

        auto start_address = options.start;

        while ( const auto info = win::memapi::virtual_query_ex( options.handle, start_address ) )
        {
            if ( start_address > options.end || !info->RegionSize )
                break;

            const auto base_address = reinterpret_cast< std::uintptr_t >( info->BaseAddress );

            if ( info->State == MEM_COMMIT && ( info->Type == MEM_PRIVATE || info->Type == MEM_IMAGE ) &&
                 !( info->Protect & PAGE_GUARD || info->Protect == PAGE_NOACCESS ) )
            {
                const auto page = win::memapi::read_process_memory( options.handle, base_address, info->RegionSize );

                for ( const auto& index : pattern.find_matches( page ) )
                    addresses.push_back( index + base_address + 1 );
            }

            start_address = base_address + info->RegionSize;
        }

        return addresses;
    }

    pattern_t pattern_t::from_byte_pattern( const std::string_view pattern )
    {
        std::vector< std::pair< std::uint8_t, bool > > bytes;

        for ( auto it = pattern.begin(); it != pattern.end(); ++it )
        {
            if ( std::isspace( *it ) )
                continue;

            auto it_next = it + 1;

            if ( *it == '?' )
            {
                if ( it_next < pattern.end() && *( it_next ) == '?' )
                {
                    // A '??' signifies a wildcard byte. These bytes can be of any value.
                    bytes.push_back( { 0x00, true } );
                    ++it;
                }
                else
                    // A '?' is a shortcut for empty bytes (0x00).
                    bytes.push_back( { 0x00, false } );
            }
            else
            {
                if ( it_next == pattern.end() )
                    break;

                const auto byte_str = std::string{ it - 1, ( ++it ) + 1 };
                const auto byte = static_cast< std::uint8_t >( std::stoul( byte_str, nullptr, 16 ) );

                bytes.emplace_back( byte, false );
            }
        }

        return { bytes };
    }

    pattern_t::pattern_t( const std::string& string )
    {
        for ( std::size_t i = 0; i < string.length(); ++i )
        {
            const auto byte = static_cast< std::uint8_t >( string.at( i ) );

            bytes.emplace_back( byte, false );
        }
    }

    std::vector< std::size_t > pattern_t::find_matches( std::vector< std::uint8_t > page ) const
    {
        std::vector< std::uintptr_t > match_locations;

        for ( std::size_t i = 0; i + bytes.size() < page.size(); )
        {
            bool located = true;
            std::size_t start = i;

            for ( const auto& [ byte, wildcard ] : bytes )
            {
                ++i;

                if ( !wildcard && page[ i ] != byte )
                {
                    located = false;
                    break;
                }
            }

            if ( located )
                match_locations.push_back( start );
        }

        return match_locations;
    }

    scanner_options_t::scanner_options_t( std::uintptr_t start, std::uintptr_t end, win::handle_t handle )
        : start( start ),
          end( end ),
          handle( handle ),
          size( end - start )
    {
    }

    scanner_options_t::scanner_options_t() : start( 0 ), end( 0 ), handle( nullptr ), size( 0 )
    {
    }

    scanner_options_t::scanner_options_t( const win::section_t& section )
        : start( section.start ),
          end( section.end ),
          handle( section.current_module.handle ),
          size( end - start )
    {
    }

    scanner_options_t::scanner_options_t( const win::module_t& current )
        : start( current.start ),
          end( current.end ),
          handle( current.handle ),
          size( end - start )
    {
    }

}  // namespace extlib