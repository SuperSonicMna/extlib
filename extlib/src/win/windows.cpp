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
    }
}  // namespace extlib::win