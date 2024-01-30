#pragma once

#include <memory>
#include <string>

#include "win/psapi.hpp"
#include "win/ptapi.hpp"
#include "win/win_exception.hpp"

namespace extlib
{
    /// <summary>
    /// Wrapper class for windows processes.
    /// </summary>
    class process
    {
       public:
        /// <summary>
        /// Gets a list of all processes with the provided name.
        /// </summary>
        /// <param name="name">The name of the target process.</param>
        /// <returns>A list of processes matching `name`.</returns>
        static std::vector< std::shared_ptr< process > > get_all_by_name( const std::string_view name );

        /// <summary>
        /// Specifies wheter or not the current process is a 64-bit process.
        /// </summary>
        /// <returns>Returns true, if the current process is 64-bit.</returns>
        bool is_64bit() const;

        /// <summary>
        /// Gets a module from the current process.
        /// </summary>
        /// <param name="name">The name of the module to retrieve.</param>
        /// <returns>The module.</returns>
        inline win::module_t operator[]( const std::string_view name ) const
        {
            return get_module_by_name( handle, name );
        }

        /// <summary>
        /// Gets all modules belonging to the current process.
        /// </summary>
        /// <returns>A list of modules.</returns>
        std::vector< win::module_t > get_modules() const;

        /// <summary>
        /// Closes the handle to the current process.
        /// </summary>
        void close();

        /// <summary>
        /// The main module of the current process.
        /// </summary>
        win::module_t main_module;

        /// <summary>
        /// The name of the process (includes .exe).
        /// </summary>
        std::string_view name;

        /// <summary>
        /// If this value is true, the process has already been closed (cannot interact).
        /// </summary>
        bool is_dead;

        /// <summary>
        /// The handle instance to the process.
        /// </summary>
        win::handle_t handle;

       private:
        /// <summary>
        /// Creates a new process instance.
        /// </summary>
        /// <param name="handle">The handle to the process.</param>
        /// <param name="main_module">The main module.</param>
        /// <param name="name">The name of the process.</param>
        process( win::handle_t handle, win::module_t main_module, const char* const name )
            : handle( handle ),
              main_module( main_module ),
              name( name ),
              is_dead( false )
        {
        }

        /// <summary>
        /// Gets a module by it's base name.
        /// </summary>
        /// <param name="handle">The handle to the target process.</param>
        /// <param name="name">The base name of the module.</param>
        /// <returns>The module.</returns>
        static win::module_t get_module_by_name( const win::handle_t& handle, const std::string_view name );
    };
}  // namespace extlib