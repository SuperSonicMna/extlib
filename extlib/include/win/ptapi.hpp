#pragma once

#include <vector>

#include "win.hpp"

namespace extlib::win
{
    struct handle_t;

    /// <summary>
    /// Wrapper for the windows process and threads API.
    /// </summary>
    class ptapi
    {
       public:
        /// <summary>
        /// Opens an existing local process object.
        /// </summary>
        /// <param name="desired_access">The access to the process object.</param>
        /// <param name="inherit">If this value is true, processes created by this process will inherit the handle.</param>
        /// <param name="id">The identifier of the local process to be opened.</param>
        /// <returns>An open handle to the specified process.</returns>
        static const handle_t& open_process( std::uint64_t desired_access, bool inherit, std::uint64_t id );

        /// <summary>
        /// Tries to open an existing local process object.
        /// </summary>
        /// <param name="handle">An open handle to the specified process.</param>
        /// <param name="desired_access">The access to the process object.</param>
        /// <param name="inherit">If this value is true, processes created by this process will inherit the handle.</param>
        /// <param name="id">The identifier of the local process to be opened.</param>
        /// <returns>If this value is true, `OpenProcess` succeeded.</returns>
        static bool try_open_process( handle_t* handle, std::uint64_t desired_access, bool inherit, std::uint64_t id );

        /// <summary>
        /// Closes an open object handle.
        /// </summary>
        /// <param name="handle">A valid handle to an open object.</param>
        static void close_handle( const handle_t& handle );

       private:
    };

}  // namespace extlib::win