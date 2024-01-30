#include "win/ptapi.hpp"

#include "win/win_exception.hpp"

namespace extlib::win
{
    const handle_t& ptapi::open_process( std::uint64_t desired_access, bool inherit, std::uint64_t id )
    {
        HANDLE hProcess = OpenProcess( desired_access, inherit, id );

        if ( !hProcess )
            throw win_exception::from_last_error( "OpenProcess" );

        return hProcess;
    }

    bool ptapi::try_open_process( handle_t* handle, std::uint64_t desired_access, bool inherit, std::uint64_t id )
    {
        HANDLE hProcess = OpenProcess( desired_access, inherit, id );

        if ( !hProcess )
            return false;

        *handle = hProcess;
        return true;
    }

    void ptapi::close_handle( const handle_t& handle )
    {
        if ( !CloseHandle( handle.handle ) )
            throw win_exception::from_last_error( "CloseHandle" );
    }
}  // namespace extlib::win