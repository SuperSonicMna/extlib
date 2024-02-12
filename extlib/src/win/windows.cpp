#include "win/windows.hpp"

#include "win/kernel32.hpp"

namespace extlib::win
{
    handle_t::handle_t( HANDLE hHandle ) : hHandle( hHandle )
    {
    }

    handle_t::~handle_t()
    {
    }

    constexpr HANDLE handle_t::get() const
    {
        return hHandle;
    }
}  // namespace extlib::win