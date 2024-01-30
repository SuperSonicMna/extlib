#pragma once

#include <optional>
#include <vector>

#include "win/win_exception.hpp"

namespace extlib::win
{
    struct handle_t;

    /// <summary>
    /// Wrapper for the windows memory API.
    /// </summary>
    class memapi
    {
       public:
        /// <summary>
        /// Reads process memory at a location.
        /// </summary>
        /// <typeparam name="T">The type to read.</typeparam>
        /// <param name="address">The location to read from.</param>
        /// <returns>The value read.</returns>
        template< typename T >
        static T read_process_memory( const handle_t& handle, std::uintptr_t address, std::size_t* bytes_read = nullptr );

        /// <summary>
        /// Reads process memory at a location.
        /// </summary>
        /// <param name="handle">The handle to the process to read from.</param>
        /// <param name="address">The location to read from.</param>
        /// <param name="length">The number of bytes to read.</param>
        /// <returns>List of bytes read.</returns>
        static std::vector< std::uint8_t >
        read_process_memory( const handle_t& handle, std::uintptr_t address, std::size_t length );

        /// <summary>
        /// Retrieves information about a range of pages within the virtual address space of a specified process.
        /// </summary>
        /// <param name="handle">A handle to the process whose memory information is queried.</param>
        /// <param name="address">The base address of the region of pages to be queried.</param>
        /// <returns>Information about the specified page range.</returns>
        static std::optional< MEMORY_BASIC_INFORMATION > virtual_query_ex( const handle_t& handle, std::uintptr_t address );

       private:
    };
    template< typename T >
    inline T memapi::read_process_memory( const handle_t& handle, std::uintptr_t address, std::size_t* bytes_read )
    {
        T value{};

        if ( !ReadProcessMemory(
                 handle.handle, reinterpret_cast< LPCVOID >( address ), &value, sizeof( value ), bytes_read ) )
            throw win_exception::from_last_error( "ReadProcessMemory" );

        return value;
    }
}  // namespace extlib::win