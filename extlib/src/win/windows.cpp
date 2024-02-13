#include "win/windows.hpp"

#include "win/kernel32.hpp"

namespace extlib::win
{
    handle_t::handle_t( HANDLE hHandle ) : hHandle( hHandle )
    {
    }

    std::shared_ptr< handle_t > handle_t::get( HANDLE hHandle )
    {
        return std::shared_ptr< handle_t >( new handle_t( hHandle ), handle_t::deleter() );
    }

    constexpr HANDLE handle_t::raw() const
    {
        return hHandle;
    }

    void handle_t::deleter::operator()( handle_t* handle ) const
    {
        const auto res = close_handle( handle );    

        if ( !res.has_value() )
            throw std::runtime_error( std::format( "CloseHandle: {}", res.error().what() ) );
    }
}  // namespace extlib::win