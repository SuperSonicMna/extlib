#include "win/win.hpp"

#include <iostream>
#include <sstream>

#include "scan.hpp"
#include "win/psapi.hpp"

namespace extlib::win
{
    std::filesystem::path handle_t::path()
    {
        return win::psapi::get_module_file_name( handle, win::module_t{} );
    }

    module_t::module_t( HANDLE hHandle, HMODULE hModule ) : handle( hHandle ), module( hModule )
    {
        const auto module_info = win::psapi::get_module_information( handle, *this );

        start = reinterpret_cast< std::uintptr_t >( module_info.lpBaseOfDll );
        end = start + module_info.SizeOfImage;

        sections = get_sections();
    }

    module_t::module_t() : handle( nullptr ), module( nullptr ), start( 0 ), end( 0 )
    {
    }

    section_t::section_t( const module_t& module, const IMAGE_SECTION_HEADER& section ) : current_module( module )
    {
        name = std::string( reinterpret_cast< const char* >( section.Name ), 8 );

        start = current_module.start + section.VirtualAddress;
        end = start + section.Misc.VirtualSize;

        size = section.Misc.VirtualSize;
    }

    std::vector< std::uintptr_t > section_t::find_all( const pattern_t& pattern ) const
    {
        scanner scan{ *this };

        return scan.find_all( pattern );
    }

    section_t module_t::operator[]( const std::string_view name ) const
    {
        for ( const auto& section : sections )
        {
            if ( section.name.find( name ) != std::string_view::npos )
                return section;
        }

        std::stringstream msg;
        msg << "Failed to locate section with name '" << name.data() << "'";
        throw std::runtime_error( msg.str() );
    }

    std::vector< section_t > module_t::get_sections()
    {
        std::vector< section_t > sections;

        dos = read< IMAGE_DOS_HEADER >( start );

        std::size_t nt_header_size;
        nt = read< IMAGE_NT_HEADERS >( start + dos.e_lfanew, &nt_header_size );

        kind = static_cast< pe_kind_t >( nt.OptionalHeader.Magic );

        std::uintptr_t offset = start + dos.e_lfanew + nt_header_size;
        for ( std::size_t i = 0; i < nt.FileHeader.NumberOfSections; ++i )
        {
            std::size_t bytes_read;
            const auto section = read< IMAGE_SECTION_HEADER >( offset, &bytes_read );

            sections.emplace_back( *this, section );
            offset += bytes_read;
        }

        return sections;
    }

    std::vector< std::uintptr_t > module_t::find_all( const pattern_t& pattern ) const
    {
        scanner scan{ *this };

        return scan.find_all( pattern );
    }

    std::vector< string_t > module_t::get_strings_by_name( std::string_view name ) const
    {
        std::vector< string_t > strings;

        const auto rdata = ( *this )[ ".rdata" ];

        if ( !rdata.size )
            return strings;

        for ( const auto& instance : rdata.find_all( name ) )
            strings.emplace_back( name.data(), instance );

        return strings;
    }

    std::vector< string_t > module_t::get_all_strings( std::size_t min_length )
    {
        std::vector< string_t > strings;

        const auto rdata = ( *this )[ ".rdata" ];

        if ( !rdata.size )
            return strings;

        const auto bytes = read( rdata.start, rdata.size );
        std::string value;

        for ( auto i = 0; i < bytes.size(); ++i )
        {
            auto byte = bytes.at( i );

            if ( byte < 32 || byte > 127 )
            {
                if ( value.size() >= min_length )
                    strings.emplace_back( value, rdata.start );

                value.clear();
                continue;
            }

            value.push_back( static_cast< char >( byte ) );
        }

        return strings;
    }

    std::vector< std::uint8_t > module_t::read( std::uintptr_t address, std::size_t length ) const
    {
        return memapi::read_process_memory( handle, address, length );
    }

    bool handle_t::is_valid() const
    {
        return handle != INVALID_HANDLE_VALUE;
    }

    std::string module_t::read_string( std::uintptr_t address, std::size_t max_length ) const
    {
        std::string value;

        if ( !contains( address + max_length ) )
            max_length = end - address;

        const auto bytes = read( address, max_length );

        for ( auto it = bytes.begin(); it != bytes.end() && *it != '\0'; ++it )
            value.push_back( *it );

        return value.data();
    }

    std::vector< region_t > module_t::get_regions( std::uintptr_t start, std::uintptr_t end ) const
    {
        std::vector< region_t > regions;

        auto start_address = start;

        while ( const auto info = memapi::virtual_query_ex( handle, start_address ) )
        {
            if ( start_address > end || !info->RegionSize )
                break;

            const auto base_address = reinterpret_cast< std::uintptr_t >( info->BaseAddress );
            const auto end = base_address + info->RegionSize;

            const auto state = static_cast< region_state_t >( info->State );
            const auto type = static_cast< region_type_t >( info->Type );

            // std::cout << info->RegionSize / sinfo.dwregionSize << '\n';
            regions.push_back( { base_address, end, info->RegionSize, info->Protect, state, type } );

            start_address = end;
        }

        return regions;
    }

    std::vector< region_t > module_t::get_regions() const
    {
        return get_regions( start, end );
    }

    std::vector< region_t > section_t::get_regions() const
    {
        return current_module.get_regions( start, end );
    }

    std::string module_t::get_name() const
    {
        return win::psapi::get_module_base_name( std::make_unique< handle_t >( handle ), *this );
    }

}  // namespace extlib::win