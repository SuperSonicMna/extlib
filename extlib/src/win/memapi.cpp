#include "win/memapi.hpp"

namespace extlib::win
{
    std::vector< std::uint8_t >
    memapi::read_process_memory( const handle_t& handle, std::uintptr_t address, std::size_t length )
    {
        std::size_t bytes_read;

        std::vector< std::uint8_t > buffer( length );

        if ( !ReadProcessMemory(
                 handle.handle, reinterpret_cast< LPCVOID >( address ), buffer.data(), length, &bytes_read ) )
            throw win_exception::from_last_error( "ReadProcessMemory" );

        return buffer;
    }

    std::optional< MEMORY_BASIC_INFORMATION > memapi::virtual_query_ex( const handle_t& handle, std::uintptr_t address )
    {
        MEMORY_BASIC_INFORMATION mbi;
        if ( !VirtualQueryEx(
                 handle.handle, reinterpret_cast< LPCVOID >( address ), &mbi, sizeof( MEMORY_BASIC_INFORMATION ) ) )
            return std::nullopt;

        return mbi;
    }
}  // namespace extlib::win