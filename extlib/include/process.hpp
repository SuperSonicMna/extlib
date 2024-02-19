#pragma once

#include <string>

#include "win/kernel32.hpp"

namespace extlib
{
    /// <summary>
    /// Represents a Windows process.
    /// </summary>
    class process final
    {
        std::shared_ptr< win::handle_t > handle;

        /// <summary>
        /// Creates a new process from a system process entry.
        /// </summary>
        /// <param name="handle">The handle to the current process.</param>
        /// <param name="entry">The process entry.</param>
        explicit process( std::shared_ptr< win::handle_t > handle, const PROCESSENTRY32& entry );

       public:
        std::string_view name;
        std::uint32_t id;
        std::unique_ptr< win::module_t > main_module;

        /// <summary>
        /// Gets a module belonging to a process from its name.
        /// </summary>
        /// <param name="name">The process's name.</param>
        /// <returns>The module.</returns>
        std::unique_ptr< win::module_t > get_module( std::string_view name );

        /// <summary>
        /// Gets all processes with the given file name.
        /// </summary>
        /// <param name="name">The file name of the target process.</param>
        /// <returns>List of processes.</returns>
        static std::vector< std::unique_ptr< process > > get_all( std::string_view name );

        /// <summary>
        /// Gets the first process with the given file name.
        /// </summary>
        /// <param name="name">The file name of the target process.</param>
        /// <returns>A pointer to the process (nullptr if doesn't exist).</returns>
        static std::unique_ptr< process > get( std::string_view name );
    };
}  // namespace extlib