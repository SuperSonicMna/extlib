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
        std::string_view name;
        std::uint32_t id;

        /// <summary>
        /// Creates a new process from a system process entry.
        /// </summary>
        /// <param name="entry">The process entry.</param>
        explicit process( const PROCESSENTRY32& entry );

       public:
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