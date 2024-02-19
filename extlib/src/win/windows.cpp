#include "win/windows.hpp"

#include "win/kernel32.hpp"

namespace extlib::win
{
    handle_t::handle_t( HANDLE hHandle ) : hHandle( hHandle )
    {
    }

    std::shared_ptr< handle_t > handle_t::get( HANDLE handle )
    {
        return std::shared_ptr< handle_t >( new handle_t( handle ), handle_t::deleter{} );
    }

    void handle_t::deleter::operator()( handle_t* handle ) const
    {
        const auto res = close_handle( handle );

        if ( !res.has_value() )
            throw std::runtime_error( std::format( "CloseHandle: {}", res.error().what() ) );

        delete handle;
    }

    constexpr HMODULE module_t::raw() const
    {
        return hModule;
    }

    constexpr std::size_t module_t::size() const
    {
        return end - start;
    }

    std::string module_t::to_string() const
    {
        return std::format( "{} - {} bytes", name, size() );
    }

    module_t::module_t( std::shared_ptr< handle_t > handle, const MODULEENTRY32& entry )
        : hModule( entry.hModule ),
          start( reinterpret_cast< std::uintptr_t >( entry.modBaseAddr ) ),
          end( start + entry.modBaseSize ),
          handle( handle ),
          name( entry.szModule )
    {
        dos_header = read< IMAGE_DOS_HEADER >( start );

        // We store the number of bytes actually read when reading the NT headers so that we know where to start reading
        // section information, aswell as differentiate between a 64-bit and 32-bit module.
        std::size_t nt_header_size;
        nt_headers = read< IMAGE_NT_HEADERS >( start + dos_header.e_lfanew, &nt_header_size );

        kind = static_cast< pe_kind >( nt_headers.OptionalHeader.Magic );

        std::uintptr_t offset = start + dos_header.e_lfanew + nt_header_size;
        for ( std::size_t i = 0; i < nt_headers.FileHeader.NumberOfSections; ++i )
        {
            std::size_t section_header_size;
            const auto section = read< IMAGE_SECTION_HEADER >( offset, &section_header_size );

            offset += section_header_size;
        }
    }

    std::vector< section_t > module_t::get_sections() const
    {
        return sections;
    }

    section_t::section_t( module_t* parent, const IMAGE_SECTION_HEADER& section_header )
        : start( parent->start + section_header.VirtualAddress ),
          end( start + section_header.Misc.VirtualSize ),
          name( reinterpret_cast< const char* >( section_header.Name ), 8 ),
          parent_module( *parent )
    {
    }

    constexpr std::size_t section_t::size() const
    {
        return end - start;
    }

    std::string section_t::to_string() const
    {
        return std::format( "'{}' - {} bytes", name, size() );
    }
}  // namespace extlib::win