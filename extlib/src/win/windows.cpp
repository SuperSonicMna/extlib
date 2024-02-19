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
          name( entry.szModule ),
          sections( get_sections() )
    {
    }

    std::vector< section_t > module_t::get_sections()
    {
        const auto dos_header = read< IMAGE_DOS_HEADER >( start );
        const auto nt_headers = read< IMAGE_NT_HEADERS >( start + dos_header.e_lfanew ); 
    }
}  // namespace extlib::win