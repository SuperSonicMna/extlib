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
        //std::shared_ptr< win::handle_t > handle;
        std::string_view name;

       public:
        /// <summary>
        /// Gets all processes with the given file name.
        /// </summary>
        /// <param name="name">The file name of the target process.</param>
        /// <returns>List of processes.</returns>
        //static std::vector< std::unique_ptr< process > > get_all( std::string_view name );

        /// <summary>
        /// Creates a new process from a unique identifier.
        /// </summary>
        /// <param name="id">The identifier.</param>
        explicit process( std::uint32_t id );
    };
}  // namespace extlib